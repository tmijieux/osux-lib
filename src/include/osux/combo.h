#ifndef OSUX_COMBO_H
#define OSUX_COMBO_H

#include <glib.h>
#include <stdint.h>

G_BEGIN_DECLS

typedef struct osux_combo_ osux_combo;
struct osux_combo_ {
    uint32_t id;
    uint32_t pos;
    GList *colours;
    GList *current;
};

#include <osux/beatmap.h>
#include <osux/color.h>
#include <osux/hitobject.h>

void osux_combo_init(osux_combo *combo, osux_beatmap *beatmap);
void osux_combo_next(osux_combo *combo, osux_hitobject *ho);
osux_color* osux_combo_colour(osux_combo const *combo);

G_END_DECLS

#endif // OSUX_COMBO_H
