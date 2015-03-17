#include "keymap_common.h"
#include "atreus_led.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* 0: mostly letters */
  KEYMAP(KC_Q, KC_W, KC_E,  KC_R,   KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, \
         KC_A, KC_S, KC_FN9,KC_FN4, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, \
         KC_Z, KC_X, KC_C,  KC_V,   KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, \
         KC_ESC, KC_TAB, KC_LGUI,  KC_LSFT, KC_BSPC, KC_LCTL, KC_LALT,     \
         KC_SPC, KC_FN0, KC_MINS,  KC_QUOT, KC_ENT),                     \

  /* 1: punctuation and numbers */
  KEYMAP(SHIFT(KC_1), SHIFT(KC_2), SHIFT(KC_LBRC), SHIFT(KC_RBRC), SHIFT(KC_BSLS), \
         KC_PGUP, KC_7, KC_8, KC_9, SHIFT(KC_8), \
         SHIFT(KC_3), SHIFT(KC_4), SHIFT(KC_9), SHIFT(KC_0), KC_GRAVE, \
         KC_PGDN, KC_4, KC_5, KC_6, SHIFT(KC_EQUAL), \
         SHIFT(KC_5), SHIFT(KC_6), KC_LBRC, KC_RBRC, SHIFT(KC_GRAVE), \
         SHIFT(KC_7), KC_1, KC_2, KC_3, KC_BSLS,                       \
         KC_FN1, SHIFT(KC_INS), KC_LGUI, KC_LSFT, KC_BSPC, KC_LCTL, KC_LALT, \
         KC_SPC, KC_FN0, KC_DOT, KC_0, KC_EQUAL), \
  
  /* 2: arrows and function keys */
  KEYMAP(KC_NO,  KC_NO,   KC_NO,   KC_NO,   KC_PGUP,                   KC_UP,  KC_F7, KC_F8, KC_F9, KC_F10, \
         KC_LCTL,KC_LALT, KC_FN10, KC_LGUI, KC_NO,                     KC_DOWN,KC_F4, KC_F5, KC_F6, KC_F11, \
         KC_NO,  KC_NO,   KC_NO,   KC_NO,   KC_NO,                     KC_NO,  KC_F1, KC_F2, KC_F3, KC_F12, \
         KC_NO,  KC_NO,   KC_LGUI, KC_LSFT, KC_DEL,  KC_LCTL, KC_LALT, KC_SPC, KC_FN2, KC_NO, KC_NO, KC_FN3), \

  /* 3: arrows and more stuff */
  KEYMAP(KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  ,                   KC_NO  , KC_FN6 , KC_FN7 , KC_FN8 , KC_VOLU, \
         KC_LCTL, KC_LALT, KC_LGUI, KC_FN4 , KC_NO  ,                   KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, KC_VOLD, \
         KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  ,                   KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_MUTE, \
         KC_NO  , KC_NO  , KC_NO  , KC_LSFT, KC_BSPC, KC_LCTL, KC_LALT, KC_NO  , KC_NO  , KC_MPRV, KC_MPLY, KC_MNXT) , \
};

enum macro_id {
    NOOP,
    // Umlauts
    AUML, OUML, UUML,
    // Stuff
    ASCII_ARROW_RIGHT,
};

const uint16_t PROGMEM fn_actions[] = {
  [0] = ACTION_LAYER_MOMENTARY(1),  // to Fn overlay
  [1] = ACTION_LAYER_ON(2, 1),  // switch to layer 2
  [2] = ACTION_LAYER_OFF(2, 1),  // switch back to layer 0
  [3] = ACTION_FUNCTION(BOOTLOADER),
  [4] = ACTION_LAYER_TAP_KEY(3, KC_F),
  [5] = ACTION_LAYER_OFF(3, ON_RELEASE),
  [6] = ACTION_MACRO(AUML),
  [7] = ACTION_MACRO(OUML),
  [8] = ACTION_MACRO(UUML),
  [9] = ACTION_LAYER_TAP_KEY(2, KC_D),
  [10] = ACTION_LAYER_OFF(2, ON_RELEASE),
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  if (id == BOOTLOADER) {
    bootloader();
  }
}

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    keyevent_t event = record->event;

    /* TODO: check if shift is pressed and emit capital umlauts/double arrow
     *       active modifiers can be read with get_mods() and the mask for
     *       the shift keys is 0b00100010
    */
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

        case ASCII_ARROW_RIGHT:
            return (event.pressed ?
                    MACRO( T(MINS), D(LSFT), T(DOT), U(LSFT), END ) :
                    MACRO_NONE);
    }
    return MACRO_NONE;
}
