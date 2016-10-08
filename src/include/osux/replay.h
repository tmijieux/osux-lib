#ifndef OSUX_REPLAY_H
#define OSUX_REPLAY_H

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
#include <stdint.h>
#include <stdbool.h>
#include <glib.h>
#include <time.h>

typedef struct osux_replay_data_ osux_replay_data;
typedef struct osux_replay_life_ osux_replay_life;

typedef struct osux_replay_ osux_replay;
typedef struct osux_replay_ osux_score;

#include <osux/beatmap.h>
#include <osux/hit.h>

G_BEGIN_DECLS

struct osux_replay_data_ {
    int64_t time_offset;
    int64_t previous_time;
    double x;
    double y;
    uint32_t keys;
};

struct osux_replay_life_ {
    uint64_t time_offset;
    double life_amount;
};

struct osux_replay_ {
    uint8_t game_mode;
    uint32_t game_version;
    char *beatmap_hash;
    char *player_name;
    char *replay_hash;

    uint16_t _300;  // [ taiko: 100%, great, 300pts], mania: 300(not MAX)
    uint16_t _100;  // [taiko: 50%, good, 150pts] , mania: 200
    uint16_t _50;   // ctb: small fruits
    uint16_t _geki; // taiko: big great (100%), mania: MAX
    uint16_t _katu; // taiko: big good (50%), mania 100
    uint16_t _miss;

    uint32_t score;
    uint16_t greatest_combo;
    uint8_t is_full_combo;
    uint32_t mods;  // gameplay mods (i.e HD, FL, DT ...)

    /* the date and time of the play  */
    time_t timestamp;

    uint32_t life_count;
    osux_replay_life *life;

    uint64_t data_count;
    osux_replay_data *data;
};

int osux_replay_init(osux_replay *r, char const *file_path);
void osux_replay_print(osux_replay const *r, FILE *f);
void osux_replay_free(osux_replay *r);
void osux_replay_move(osux_replay *src, osux_replay *dst);
int osux_replay_compute_hit(osux_replay *r, osux_beatmap *b, osux_hit **hits);

G_END_DECLS

#endif // OSUX_REPLAY_H
