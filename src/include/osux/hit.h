#ifndef OSUX_HIT_H
#define OSUX_HIT_H

#include <string.h>
#include <glib.h>

G_BEGIN_DECLS

typedef struct osux_hit_ osux_hit;

typedef enum osux_hit_type_ osux_hit_type;
typedef struct osux_keypress_ osux_keypress;
typedef struct osux_hits_ osux_hits;

#include <osux/util.h>
#include <osux/mods.h>
#include <osux/hitobject.h>
#include <osux/replay.h>

enum osux_hit_type_ {
    HIT_RAINBOW_300 = 0,
    HIT_300,
    HIT_200,
    HIT_100,
    HIT_50,
    HIT_MISS,
    HIT_SPECIAL, // for shaker and roll in taiko
    MAX_HIT_TYPE
};

struct osux_hit_ {
    osux_hit_type hit_type;
    bool hitted; // can be true even if hit_type == hit_miss
};

struct osux_keypress_ {
    int64_t offset;
    uint32_t key;
    bool is_release;
};

struct osux_hits_ {
    int game_mode;
    int mods;
    int overall_difficulty;

    osux_replay_data *data;
    size_t data_count;

    osux_keypress *keypress;
    size_t keypress_count;

    /* public */
    osux_hit *hits;
    size_t hits_size;
};

void osux_get_hit_windows(
    double window[], // Array of size MAX_HIT_TYPE to be filled by this function
    double od, // map overall difficulty
    int mods); // game mods, only EZ,HR,,HT,DT(or NC) have influence on hit window
int osux_get_approach_time(double ar, int mods);

int osux_hits_init(osux_hits *hits,
                   osux_beatmap const *beatmap, osux_replay const *replay);
void osux_hits_print_keypress(osux_hits *hits);
int osux_hits_free(osux_hits *hits);

G_END_DECLS

#endif // OSUX_HIT_H
