static const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    KEYMAP(  // layer 0 : default
        // left hand
        ESC, 1,   2,   3,   4,   5,   MINS,
        GRV, Q,   W,   E,   R,   T,   FN2,
        LCTL,A,   S,   D,   F,   G,
        TAB, Z,   X,   C,   V,   B,   LSFT,
        BSLS,LSFT,LCTL,LALT,LGUI,
                                      LEFT,RGHT,
                                             UP,
                                 BSPC, FN1,DOWN,
        // right hand
             EQL, 6,   7,   8,   9,   0,   LBRC,
             ENT, Y,   U,   I,   O,   P,   RBRC,
                  H,   J,   K,   L,   SCLN,QUOT,
             RSFT,N,   M,   COMM,DOT, SLSH,MINS,
                       RGUI,RALT,RCTL,RSFT, EQL,
        FN3,CAPS,
        NO,
        NO,  FN1,SPC
    ),

    KEYMAP(  // layer 1 : function and symbol keys
        // left hand
        TRNS,F1,  F2,  F3,  F4,  F5,  F11,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,FN4,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,
                                      HOME, END,
                                           PGUP,
                                 DEL, TRNS,PGDN,
        // right hand
              F12, F6,  F7,  F8,  F9,  F10,TRNS,
             TRNS,TRNS,TRNS, FN5, FN6, FN7,TRNS,
                  LEFT,DOWN,  UP,RGHT,LBRC,RBRC,
             TRNS,TRNS,TRNS,TRNS, FN8,TRNS,TRNS,
                       TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,
        TRNS,
        TRNS,TRNS,TRNS
    ),

    KEYMAP(  // layer 2 : keyboard functions
        // left hand
        FN0, TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,
                                      TRNS,TRNS,
                                           TRNS,
                                 TRNS,TRNS,TRNS,
        // right hand
             FN4, TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
             TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,VOLU,
                  TRNS,TRNS,TRNS,TRNS,TRNS,VOLD,
             TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,MUTE,
                       TRNS,TRNS,MPRV,MPLY,MNXT,
        TRNS,TRNS,
        TRNS,
        TRNS,TRNS,TRNS
    ),

    KEYMAP(  // layer 3: numpad
        // left hand
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,
                                      TRNS,TRNS,
                                           TRNS,
                                 TRNS,TRNS,TRNS,
        // right hand
             TRNS,NLCK,PSLS,PAST,PAST,PMNS,BSPC,
             TRNS,NO,  P7,  P8,  P9,  PMNS,BSPC,
                  NO,  P4,  P5,  P6,  PPLS,PENT,
             TRNS,NO,  P1,  P2,  P3,  PPLS,PENT,
                       P0,  PDOT,SLSH,PENT,PENT,
        TRNS,TRNS,
        TRNS,
        TRNS,TRNS,TRNS
    ),

};

/* id for user defined functions */
enum function_id {
    TEENSY_KEY,
};

enum macro_id {
    AUML, OUML, UUML,
    ARROW_RIGHT,
};

/*
 * Fn action definition
 */
static const uint16_t PROGMEM fn_actions[] = {
   [0] = ACTION_FUNCTION(TEENSY_KEY),
   [1] = ACTION_LAYER_TAP_TOGGLE(1),
   [2] = ACTION_LAYER_MOMENTARY(2),
   [3] = ACTION_LAYER_TOGGLE(3),
   [4] = ACTION_LAYER_SET(0, ON_PRESS),
   [5] = ACTION_MACRO(AUML),
   [6] = ACTION_MACRO(OUML),
   [7] = ACTION_MACRO(UUML),
   [8] = ACTION_MACRO(ARROW_RIGHT),
};

void action_function(keyrecord_t *event, uint8_t id, uint8_t opt)
{
    if (id == TEENSY_KEY) {
        clear_keyboard();
        print("\n\nJump to bootloader... ");
        _delay_ms(250);
        bootloader_jump(); // should not return
        print("not supported.\n");
    }
}

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    keyevent_t event = record->event;

    switch (id) {
        case AUML:
            return (event.pressed ?
                    MACRO( D(LALT), T(U), U(LALT), T(A), END ) :
                    MACRO_NONE);
        case OUML:
            return (event.pressed ?
                    MACRO( D(LALT), T(U), U(LALT), T(O), END ) :
                    MACRO_NONE);
        case UUML:
            return (event.pressed ?
                    MACRO( D(LALT), T(U), U(LALT), T(U), END ) :
                    MACRO_NONE);

        case ARROW_RIGHT:
            return (event.pressed ?
                    MACRO( T(MINS), D(LSFT), T(DOT), U(LSFT), END ) :
                    MACRO_NONE);
    }
    return MACRO_NONE;
}
