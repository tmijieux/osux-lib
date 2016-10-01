#include <osux/combo.h>

void
osux_combo_init(osux_combo *combo, osux_beatmap *beatmap)
{
    memset(combo, 0, sizeof *combo);
    combo->colours = beatmap->combo_colours;
    combo->current = g_list_last(combo->colours);
}

void
osux_combo_next(osux_combo *combo, osux_hitobject *ho)
{
    if (HIT_OBJECT_IS_NEWCOMBO(ho)) {
        if (combo->current->next == NULL)
            combo->current = combo->colours;
        else
            combo->current = combo->current->next;
        ++ combo->id;
        combo->pos = 1;
    } else
        ++ combo->pos;
}

osux_color*
osux_combo_colour(osux_combo const *combo)
{
    return (osux_color*) combo->current->data;
}
