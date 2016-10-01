/*
 *  Copyright (©) 2015 Lucas Maugère, Thomas Mijieux
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <math.h>

#include "osux/beatmap.h"
#include "osux/hitobject.h"
#include "osux/timingpoint.h"
#include "osux/taiko_autoconvert.h"
#include "osux/error.h"

#define OFFSET_EQUAL_PERCENTAGE 0.05

struct tconv {
    osux_hitobject *ho;
    double beat_length; // ms per beat
    double tick_length; // ms per tick

    // full length of the slider in ms, this include repetitions.
    // Use double for more precision as a division is involved
    double length;
};

static inline int
offset_eq(double o1, double o2)
{
    return fabs(1. - o1 / o2) <= OFFSET_EQUAL_PERCENTAGE;
}

static inline int
offset_le(double o1, double o2)
{
    return (o1 <= o2) || offset_eq(o1, o2);
}

// return the full length of the slider in ms (repetitions included)
static double
slider_length(osux_hitobject const *slider)
{
    g_assert( HIT_OBJECT_IS_SLIDER(slider) );
    return slider->end_offset - slider->offset;
}

static osux_hitobject *
hitobject_new_move(osux_hitobject *ho)
{
    osux_hitobject *copy = g_malloc(sizeof*ho);
    osux_hitobject_move(ho, copy);
    return copy;
}

/**
   return the newly created taiko circle!

   if the slider has edge hitsounds the new circle inherit one of them.
   Sometimes, the edge hitsound used may not be correspond the the
   circle offset. This happens, mostly,
   with simple sliders and high tick rate.

   Example:
   with 1/4 (each character represent 1/4 beat) and tick rate = 4
   Simple slider (does not repeat):
   s----
   Slider hitsound: (there are hitsound only at the start and the end)
   d---k
   Circle in taiko:
   dkdkd
   As you can see, the end circle is a don while the slider end
   hitsound is a kat.
*/
static osux_hitobject *
new_taiko_object(int offset,
                 osux_hitobject const *old_slider,
                 int edge_hs_index)
{
    osux_hitobject *ho;
    osux_edgehitsound *edges;
    int sample_type;
    g_assert( HIT_OBJECT_IS_SLIDER(old_slider) );

    ho = g_malloc0(sizeof*ho);
    edges = old_slider->slider.edgehitsounds;
    sample_type = old_slider->hitsound.sample_type;
    if (edges != NULL)
        sample_type = edges[edge_hs_index].sample_type; // see notes above
    char *rep = g_strdup_printf("0,0,%d,%d,%d,0:0:0:0:",
                                offset, HITOBJECT_CIRCLE, sample_type);
    int err = osux_hitobject_init(ho, rep, 12);
    if (err != 0)
        osux_fatal("PROGRAMMING error\n");
    g_free(rep);
    return ho;
}

static void
conv_init(struct tconv *conv, osux_hitobject *object, double tick_rate)
{
    memset(conv, 0, sizeof *conv);
    conv->ho = object;
    conv->beat_length = object->timingpoint->millisecond_per_beat;
    conv->tick_length = conv->beat_length / tick_rate; // also in milliseconds
    conv->length = slider_length(object);
}

void tconv_print(struct tconv const *tc)
{
    fprintf(stderr, "tconv:\n");
    fprintf(stderr, "\toffset: %ld\n", tc->ho->offset);
    fprintf(stderr, "\tbeat_length: %g\n", tc->beat_length);
    fprintf(stderr, "\ttick_length: %g\n", tc->tick_length);
    fprintf(stderr, "\tlength: %g\n", tc->length);
}

static void
slider_to_circles_normal(const struct tconv *conv, GQueue *objects)
{
    if (conv->length <= conv->tick_length) {
        // if slider length is lower than duration of 1 (slider) tick
        // then replace the slider by two circle:
        // positions are start and end of the old slider

        osux_hitobject *ho1, *ho2;
        // start
        ho1 = new_taiko_object(conv->ho->offset, conv->ho, 0);
        g_queue_push_tail(objects, ho1);

        // end
        ho2 = new_taiko_object(conv->ho->offset + conv->length, conv->ho, 1);
        g_queue_push_tail(objects, ho2);

    } else {
        // else, the slider last for more than one tick.
        unsigned i;
        double offset;

        // add a circle for each tick, without exceeding slider length
        // But there is a small margin in the comparison, sliders are
        // considered slightly longer than they are.
        // It is quite hard to guess how peppy is computing this margin
        // so the margin used here is likely to be wrong.
        // See offset_eq and offset_le for the margin computation.
        for (i = 0, offset = 0.;
             offset_le(offset, conv->length);
             ++i, offset += conv->tick_length)
        {
            osux_hitobject *ho;

            // The inherited hitsound is alterning between the start one (0)
            // and the end one (1).
            // Thus i%2 is used as the edge hitsound index.
            ho = new_taiko_object(conv->ho->offset + offset, conv->ho, i % 2);
            g_queue_push_tail(objects, ho);
        }
    }
}

static void
slider_to_circles_repeat(const struct tconv *conv, GQueue *objects)
{
    // compute the length in ms for the slider without repetitions
    double unit = conv->length / conv->ho->slider.repeat;

    // add one circle for each times the slider is repeated
    // circle are disposed with constant spacing ('unit')
    // so that the whole slider length is filled with circles.
    for (unsigned i = 0; i <= conv->ho->slider.repeat; i++) {
        osux_hitobject *ho;

        // the edge hitsound used is the one corresponding to the
        // i-th repeat part.
        ho = new_taiko_object(conv->ho->offset + (i * unit), conv->ho, i);
        g_queue_push_tail(objects, ho);
    }
}

static void
convert(const struct tconv *conv, GQueue *objects)
{
    if (conv->length >= 2*conv->beat_length) {
        // if slider length if big enough, keep the slider
        g_queue_push_tail(objects, hitobject_new_move(conv->ho));
    } else {
        // when the slider is too short, convert it to circles:
        // (two rules according to the slider being repeated or not)
        if (conv->ho->slider.repeat != 1)
            slider_to_circles_repeat(conv, objects);
        else
            slider_to_circles_normal(conv, objects);
        osux_hitobject_free(conv->ho);
    }
}

static GQueue *
taiko_autoconvert_ho_list(const osux_beatmap *bm)
{
    GQueue *new_objects = g_queue_new();

    for (uint32_t i = 0; i < bm->hitobject_count; ++i) {
        osux_hitobject *ho = &bm->hitobjects[i];

        if (HIT_OBJECT_IS_SPINNER(ho) || HIT_OBJECT_IS_CIRCLE(ho)) {
            // keep spinner and circle
            g_queue_push_tail(new_objects, hitobject_new_move(ho));
        } else if (HIT_OBJECT_IS_SLIDER(ho)) {
            struct tconv conv;
            conv_init(&conv, ho, bm->SliderTickRate);
            convert(&conv, new_objects);
        }
    }
    return new_objects;
}

static osux_hitobject *
list_to_object_array(GQueue *objects)
{
    int i;
    GList *it;
    osux_hitobject *array;
    unsigned size = g_queue_get_length(objects);

    array = g_malloc(sizeof array[0] * size);
    for (it = objects->head, i = 0; it != NULL; it = it->next, ++i)
        array[i] = *(osux_hitobject*) it->data;

    return array;
}

int osux_beatmap_taiko_autoconvert(osux_beatmap *bm)
{
    int err = 0;
    if (bm->game_mode != GAME_MODE_STD)
        return -1;

    GQueue *new_objects;
    osux_hitobject *array;
    unsigned hitobject_count;

    new_objects = taiko_autoconvert_ho_list(bm);
    hitobject_count = g_queue_get_length(new_objects);
    array = list_to_object_array(new_objects);

    g_queue_free_full(new_objects, g_free);
    g_free(bm->hitobjects);

    bm->game_mode         = GAME_MODE_TAIKO;
    bm->hitobject_count   = hitobject_count;
    bm->hitobject_bufsize = hitobject_count;
    bm->hitobjects        = array;
    err = osux_beatmap_prepare(bm);
    return err;
}
