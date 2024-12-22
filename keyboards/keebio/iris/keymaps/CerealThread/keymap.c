#include QMK_KEYBOARD_H
#include "cerealthread.h"
#include "cerealthreadprivate.h"


bool is_alt_tab_active = false;
int layerNumber = 0;

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  GAMING,
  LOWER,
  RAISE,
  ADJUST,
  ALT_TAB,
  QWERSFT,
  QWERCTL,
  LOWOTH,
  SFTNUM0,
  SFTNUM1,
  SFTNUM2,
  SFTNUM3,
  SFTNUM4,
  SFTNUM5,
  SFTNUM6,
  SFTNUM7,
  SFTNUM8,
  SFTNUM9,
  RGBSPIN,
  RGBSPDE,
  PRIMAC1,
  PRIMAC2,
  PRIMAC3,
  PRIMAC4,
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_TAP
} td_state_t;

typedef struct {
  bool is_press_action;
  td_state_t state;
} td_tap_t;

enum custom_tapdances {
   WINALT = 0,
   LSWAP  = 1,
};


td_state_t cur_dance(qk_tap_dance_state_t *state);

void ql_finished(qk_tap_dance_state_t *state, void *user_data);
void ql_reset(qk_tap_dance_state_t *state, void *state_data);
void ql_each_tap(qk_tap_dance_state_t *state, void *user_data);


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                               KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                               KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_DEL,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                               KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    SH_T(KC_HOME),SH_T(KC_END),KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                  TD(WINALT),TD(LSWAP),KC_ENTER,                 KC_SPC,  RAISE,   KC_RALT
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

    [_GAMING] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______,                            _______ , _______, _______, KC_UP,  _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, KC_LEFT, KC_DOWN, KC_RGHT, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, LOWER,   KC_SPC,                    KC_ENT,  RAISE,   _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_LOWER] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______, _______, _______, _______, _______, _______,                            _______, _______, KC_PDOT, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     ALT_TAB, KC_1,    KC_2,    KC_UP,   KC_4,    KC_5,                               KC_6,    SFTNUM7, SFTNUM8, SFTNUM9, KC_0,    KC_PIPE,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_DEL,  KC_VOLU, KC_LEFT, KC_DOWN, KC_RGHT, KC_LBRC,                            KC_RBRC, SFTNUM4, SFTNUM5, SFTNUM6, KC_PLUS, KC_PAST,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     BL_STEP, KC_VOLD, KC_MPLY, KC_MNXT, XXXXXXX, KC_LCBR, KC_LPRN,          KC_RPRN, KC_RCBR, SFTNUM1, SFTNUM2, SFTNUM3, KC_MINS, KC_PSLS,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    KC_P0,   _______, KC_DEL,                    KC_DEL,  _______, SFTNUM0
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),


  [_RAISE] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     KC_F12,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                              KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______,                            _______, _______, KC_MS_U, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_CAPS, _______, KC_MPLY, KC_MNXT, KC_PGUP, KC_MINS,                            KC_EQL,  KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_U, KC_BSLS,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_MUTE, KC_PSCR, KC_MSTP, KC_MPRV, KC_PGDN, KC_UNDS, KC_LPRN,          _______, KC_PLUS, XXXXXXX, KC_HOME, KC_END,  KC_WH_D,  _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    KC_BTN2, _______, KC_BTN1,                   _______, _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),


  [_ADJUST] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______, _______, _______, _______, _______, PRIMAC1,                            _______, KC_NUM,  _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     QK_BOOT, _______, _______, KC_MS_U, _______, PRIMAC2,                            _______, _______, _______, _______, RGB_M_B, RGB_M_G,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     RGB_TOG, _______, KC_MS_L, KC_MS_D, KC_MS_R, PRIMAC3,                            GAMING , QWERTY,  RGB_HUI, RGB_SAI, RGB_VAI, RGBSPIN,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     RGB_MOD, _______, _______, _______, _______, PRIMAC4, _______,          _______, _______, _______, RGB_HUD, RGB_SAD, RGB_VAD, RGBSPDE,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, _______, _______,                   _______, _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_LASWAP] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
   XXXXXXX,TO(_QWERTY),TG(_CSP),TG(_ZBRSH),TG(_BLEND),XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,          _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, _______, _______,                   _______, _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

    [_CSP] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
  _______,TG(_CSP),G(C(KC_O)),G(C(KC_C)),KC_H,    KC_Y,                               XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, C(KC_D), KC_F,    KC_G,    KC_K,    KC_S,                               XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     QWERSFT, KC_M,    KC_P,    KC_B,    KC_RBRC,RCS(KC_N),                           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     QWERCTL, C(KC_T), KC_E,    KC_A,    KC_LBRC, KC_Q,    C(KC_S),          _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______,LOWOTH,_______,                   _______, _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

    [_ZBRSH] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
  _______,TG(_ZBRSH),G(C(KC_O)),G(C(KC_C)),_______,_______,                           _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______,                            _______ , _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______,_______,_______,LT(_BRUSH,KC_B),_______,     _______, _______, _______, _______, _______, _______, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______,TD(LSWAP),KC_SPC,                    KC_ENT,  _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_BRUSH] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
  _______,TG(_ZBRSH),G(C(KC_O)),G(C(KC_C)),_______,_______,                           _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, KC_5,    KC_6,    KC_7,    _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, KC_4,    KC_3,    KC_2,    KC_1,    _______,                            _______ , _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, KC_8,    KC_9,    KC_1,    _______, _______,          _______, _______, _______, _______, _______, _______, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______,TD(LSWAP),KC_SPC,                    KC_ENT,  _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_BLEND] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
    _______,  _______, _______ , _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______,                            _______ , _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______,LT(_BBRSH,KC_B),_______,    _______, _______, _______, _______, _______, _______, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______,TD(LSWAP),KC_ENT,                    KC_SPC,  _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_BBRSH] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
  _______,TG(_BLEND),  _______ ,_______, _______, _______,                           _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, KC_I,   LSFT(KC_T),KC_C,   _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, KC_P,    LSFT(KC_C),KC_G,  KC_X,    _______,                            _______ , _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______,TD(LSWAP),KC_SPC,                    KC_ENT,  _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  )

};

const keypos_t hand_swap_config[MATRIX_ROWS][MATRIX_COLS] = {
    /* Left hand, matrix positions */
    {{0,5}, {1,5}, {2,5}, {3,5}, {4,5}, {5,5}},
    {{0,6}, {1,6}, {2,6}, {3,6}, {4,6}, {5,6}},
    {{0,7}, {1,7}, {2,7}, {3,7}, {4,7}, {5,7}},
    {{0,8}, {1,8}, {2,8}, {3,8}, {4,8}, {5,8}},
    {{0,9}, {1,9}, {2,9}, {3,9}, {4,9}, {5,9}},
    /* Right hand, matrix positions */
    {{0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}},
    {{0,1}, {1,1}, {2,1}, {3,1}, {4,1}, {5,1}},
    {{0,2}, {1,2}, {2,2}, {3,2}, {4,2}, {5,2}},
    {{0,3}, {1,3}, {2,3}, {3,3}, {4,3}, {5,3}},
    {{0,4}, {1,4}, {2,4}, {3,4}, {4,4}, {5,4}},
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  bool checkBlendLayer = IS_LAYER_ON(_BLEND);
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);
        layer_move(_QWERTY);
      }
      return false;
      break;
    case GAMING:
      if (record->event.pressed) {
        layer_move(_GAMING);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        if(is_alt_tab_active) {
            is_alt_tab_active = false;
            unregister_code(KC_LALT);
        }
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
      if (record->event.pressed) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
      break;
    case ALT_TAB:
      if (record->event.pressed && !is_alt_tab_active) {
        register_code(KC_LALT);
        is_alt_tab_active = true;
        register_code(KC_TAB);
      } else if(record->event.pressed && is_alt_tab_active) {
        register_code(KC_TAB);
      } else {
        unregister_code(KC_TAB);
      }
      return false;
      break;
    case QWERSFT:
      if (record->event.pressed) {
        layerNumber = CheckLayer();
        layer_move(_QWERTY);
        register_code(KC_LSFT);
      } else {
        layer_move(layerNumber);
        unregister_code(KC_LSFT);
      }
      return false;
      break;
    case QWERCTL:
      if (record->event.pressed) {
        layerNumber = CheckLayer();
        layer_move(_QWERTY);
        register_code(KC_LCTL);
      } else {
        layer_move(layerNumber);
        unregister_code(KC_LCTL);
      }
      return false;
      break;
    case LOWOTH:
      if (record->event.pressed) {
        layerNumber = CheckLayer();
        layer_move(_LOWER);
      } else {
        if(is_alt_tab_active) {
            is_alt_tab_active = false;
            unregister_code(KC_LALT);
        }
        layer_move(layerNumber);
      }
      return false;
      break;
    case SFTNUM0:
      if (record->event.pressed) {
        if (get_mods() && MOD_MASK_SHIFT && !checkBlendLayer) {
          register_code(KC_0);
        } else {
          register_code(KC_P0);
        }
      } else {
          unregister_code(KC_0);
          unregister_code(KC_P0);
      }
      return false;
      break;
    case SFTNUM1:
      if (record->event.pressed) {
        if (get_mods() && MOD_MASK_SHIFT && !checkBlendLayer) {
          register_code(KC_1);
        } else {
          register_code(KC_P1);
        }
      } else {
          unregister_code(KC_1);
          unregister_code(KC_P1);
      }
      return false;
      break;
    case SFTNUM2:
      if (record->event.pressed) {
        if (get_mods() && MOD_MASK_SHIFT && !checkBlendLayer) {
          register_code(KC_2);
        } else {
          register_code(KC_P2);
        }
      } else {
          unregister_code(KC_2);
          unregister_code(KC_P2);
      }
      return false;
      break;
    case SFTNUM3:
      if (record->event.pressed) {
        if (get_mods() && MOD_MASK_SHIFT && !checkBlendLayer) {
          register_code(KC_3);
        } else {
          register_code(KC_P3);
        }
      } else {
          unregister_code(KC_3);
          unregister_code(KC_P3);
      }
      return false;
      break;
    case SFTNUM4:
      if (record->event.pressed) {
        if (get_mods() && MOD_MASK_SHIFT && !checkBlendLayer) {
          register_code(KC_4);
        } else {
          register_code(KC_P4);
        }
      } else {
          unregister_code(KC_4);
          unregister_code(KC_P4);
      }
      return false;
      break;
    case SFTNUM5:
      if (record->event.pressed) {
        if (get_mods() && MOD_MASK_SHIFT && !checkBlendLayer) {
          register_code(KC_5);
        } else {
          register_code(KC_P5);
        }
      } else {
          unregister_code(KC_5);
          unregister_code(KC_P5);
      }
      return false;
      break;
    case SFTNUM6:
      if (record->event.pressed) {
        if (get_mods() && MOD_MASK_SHIFT && !checkBlendLayer) {
          register_code(KC_6);
        } else {
          register_code(KC_P6);
        }
      } else {
          unregister_code(KC_6);
          unregister_code(KC_P6);
      }
      return false;
      break;
    case SFTNUM7:
      if (record->event.pressed) {
        if (get_mods() && MOD_MASK_SHIFT && !checkBlendLayer) {
          register_code(KC_7);
        } else {
          register_code(KC_P7);
        }
      } else {
          unregister_code(KC_7);
          unregister_code(KC_P7);
      }
      return false;
      break;
    case SFTNUM8:
      if (record->event.pressed) {
        if (get_mods() && MOD_MASK_SHIFT && !checkBlendLayer) {
          register_code(KC_8);
        } else {
          register_code(KC_P8);
        }
      } else {
          unregister_code(KC_8);
          unregister_code(KC_P8);
      }
      return false;
      break;
    case SFTNUM9:
      if (record->event.pressed) {
        if (get_mods() && MOD_MASK_SHIFT && !checkBlendLayer) {
          register_code(KC_9);
        } else {
          register_code(KC_P9);
        }
      } else {
          unregister_code(KC_9);
          unregister_code(KC_P9);
      }
      return false;
      break;
    case RGBSPIN:
      if (record->event.pressed) {
        rgblight_increase_speed_noeeprom();
      }
      return false;
      break;
    case RGBSPDE:
      if (record->event.pressed) {
        rgblight_decrease_speed_noeeprom();
      }
      return false;
      break;
    case PRIMAC1:
      if (record->event.pressed) {
        PrivateMacro1();
      }
      return false;
      break;
    case PRIMAC2:
      if (record->event.pressed) {
        PrivateMacro2();
      }
      return false;
      break;
    case PRIMAC3:
      if (record->event.pressed) {
        PrivateMacro3();
      }
      return false;
      break;
    case PRIMAC4:
      if (record->event.pressed) {
        PrivateMacro4();
      }
      return false;
      break;
  }
  return true;
}

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    HSV leftHSV = {190, 255, 255};
    HSV rightHSV = {190, 255, 255};

    int rgbmode = rgb_matrix_get_mode();


    if(rgbmode != 17) {                  //check if trans
        if (layer_state_is(_QWERTY)) {
            HSV newhsv = {190, 255, 255};
            leftHSV = newhsv;
            rightHSV = newhsv;
        } else if(layer_state_is(_LOWER)) {
            HSV newhsv = {22, 255, 200};
            leftHSV = newhsv;
        } else if(layer_state_is(_RAISE)) {
            HSV newhsv = {134, 200, 200};
            rightHSV = newhsv;
        } else if(layer_state_is(_GAMING)) {
            HSV newhsv = {0, 255, 255};
            leftHSV = newhsv;
            rightHSV = newhsv;
        } else if(layer_state_is(_ZBRSH)) {
            HSV newhsv = {106, 255, 255};
            leftHSV = newhsv;
            rightHSV = newhsv;
        } else if(layer_state_is(_CSP)) {
            HSV newhsv = {169, 255, 255};
            leftHSV = newhsv;
            rightHSV = newhsv;
        } else if(layer_state_is(_LASWAP)) {
            HSV newhsv = {64, 255, 200};
            leftHSV = newhsv;
            rightHSV = newhsv;
        } else {
            HSV newhsv = {0, 0, 0};
            leftHSV = newhsv;
            rightHSV = newhsv;
        }

        if (leftHSV.v > rgb_matrix_get_val()) {
            leftHSV.v = rgb_matrix_get_val();
        }
        if (rightHSV.v > rgb_matrix_get_val()) {
            rightHSV.v = rgb_matrix_get_val();
        }
        RGB leftRGB = hsv_to_rgb(leftHSV);
        RGB rightRGB = hsv_to_rgb(rightHSV);

        for (uint8_t i = led_min; i < led_max; i++) {
            if (HAS_FLAGS(g_led_config.flags[i], 0x10)) { // 0x01 == LED_FLAG_MODIFIER
                rgb_matrix_set_color(i, leftRGB.r, leftRGB.g, leftRGB.b);
            }
            if (HAS_FLAGS(g_led_config.flags[i], 0x20)) { // 0x01 == LED_FLAG_MODIFIER
                rgb_matrix_set_color(i, rightRGB.r, rightRGB.g, rightRGB.b);
            }
        }
    }
}


td_state_t cur_dance(qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (!state->pressed) return TD_SINGLE_TAP;
    else return TD_SINGLE_HOLD;
  } else if (state->count == 2)
    if (!state->pressed) return TD_DOUBLE_TAP;
    else return TD_DOUBLE_HOLD;
  else return TD_UNKNOWN;
}

static td_tap_t ql_tap_state = {
  .is_press_action = true,
  .state = TD_NONE
};

void ql_finished(qk_tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            layer_on(_LOWER);
            break;
        case TD_SINGLE_HOLD:
            layer_on(_LOWER);
            update_tri_layer(_LOWER, _RAISE, _ADJUST);
            break;
        case TD_DOUBLE_HOLD:
            layer_on(_LASWAP);
            break;
        default:
            break;
    }
}

void ql_reset(qk_tap_dance_state_t *state, void *user_data) {
  if (ql_tap_state.state == TD_SINGLE_TAP) {
    layer_off(_LOWER);
    is_alt_tab_active = false;
    unregister_code(KC_LALT);
  }
  if (ql_tap_state.state == TD_SINGLE_HOLD) {
    layer_off(_LOWER);
    update_tri_layer(_LOWER, _RAISE, _ADJUST);
    is_alt_tab_active = false;
    unregister_code(KC_LALT);
  } else if (ql_tap_state.state == TD_DOUBLE_HOLD) {
    layer_off(_LOWER);
    layer_off(_LASWAP);
  }
  ql_tap_state.state = TD_NONE;
}

void ql_each_tap(qk_tap_dance_state_t *state, void *user_data) {
    layer_on(_LOWER);
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [WINALT] = ACTION_TAP_DANCE_DOUBLE(KC_RALT, KC_LGUI),
  [LSWAP] = ACTION_TAP_DANCE_FN_ADVANCED(ql_each_tap, ql_finished, ql_reset)
};
