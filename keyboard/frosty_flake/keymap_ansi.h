
static const uint8_t PROGMEM keymaps[][MATRIX_COLS][MATRIX_ROWS] = {
    /* Layer 0: Standard ANSI layer */
    KEYMAP(\
         ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11,  F12,        PSCR,SLCK,PAUS, \
         GRV, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS, EQL,BSPC,   INS, HOME,PGUP, \
         TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSLS,   DEL, END, PGDN, \
         CAPS,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,      ENT,                   \
         LSFT,NUBS,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,     RSFT,        UP,        \
         LCTL,LGUI,LALT,               SPC,                RALT,RGUI, FN0,RCTL,   LEFT,DOWN,RGHT),\
    /* Layer 1: Function layer */
    KEYMAP(\
         CALC,MYCM,WSCH,WHOM,MAIL,VOLD,VOLU,MSEL,MSTP,MPLY,MPRV,MNXT,TRNS,        WAKE, PWR,SLEP, \
         TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,   TRNS,TRNS,TRNS, \
         TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,   TRNS,TRNS,TRNS, \
         TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,                   \
         TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,        TRNS,      \
         TRNS,TRNS,TRNS,               TRNS,               TRNS,TRNS,TRNS,TRNS,   TRNS,TRNS,TRNS)
};

static const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_MOMENTARY(1)
};
