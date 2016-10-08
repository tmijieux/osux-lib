#ifndef OSUX_KEYS_H
#define OSUX_KEYS_H

#include <glib.h>

G_BEGIN_DECLS

enum taiko_keys {
    KEY_LEFT_DON    =   1,
    KEY_LEFT_KAT    =   2,
    KEY_RIGHT_DON   =   4,
    KEY_RIGHT_KAT   =   8,
    KEY_DON_MASK    = KEY_LEFT_DON | KEY_RIGHT_DON,
    KEY_KAT_MASK    = KEY_LEFT_KAT | KEY_RIGHT_KAT,
};

enum std_keys {
    KEY_MOUSE_LEFT     =  1,
    KEY_MOUSE_RIGHT    =  2,
    KEY_LEFT           =  4 + 1,
    KEY_RIGHT          =  8 + 2,
    KEY_KEYBOARD_SMOKE =  16,

    KEY_HIT_MASK  = KEY_LEFT | KEY_RIGHT,
};

#define HIT_KEY_PRESSED(key) ((key) & KEY_HIT_MASK)

enum ctb_keys {
    CTB_DASH = 1
};

enum mania_keys {
    // This is encoded in the x field; NOT IN the key field
    // keys left to right

    MANIA_1   =  1,
    MANIA_2   =  2,
    MANIA_3   =  4,
    MANIA_4   =  8,
    MANIA_5   =  16,
    MANIA_6   =  32,
    MANIA_7   =  64,
    MANIA_8   =  128,
    MANIA_9   =  256,
};


G_END_DECLS

#endif // OSUX_KEYS_H
