/* Copyright 2024 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "lemokey_common.h"

// clang-format off
enum layers {
    BASE,
    MIDI,
    FN,
    L3,
};

enum custom_keycodes {
    CC_1 = SAFE_RANGE,
    CC_2,
    CC_3,
    CC_4,
    CC_5,
    CC_6,
    CC_7,
    CC_8,
    CC_9,
    CC_10,
    CC_11,
    CC_12,
    CC_13,
    CC_14,
    CC_15,
    CC_16,
    MIDI_SEND_MINUS_1,
    MIDI_SEND_0,
    MIDI_SEND_PLUS_1,
    MIDI_NEXT_LAYER,
    MIDI_PREV_LAYER,

// Alias

    MIDI_0  = MIDI_SEND_0,
    MIDI_P1 = MIDI_SEND_PLUS_1,
    MIDI_M1 = MIDI_SEND_MINUS_1,
    MIDI_NL = MIDI_NEXT_LAYER,
    MIDI_PL = MIDI_PREV_LAYER,
    CC_LAST = CC_16,
};



#define MIDI_BASE_CC_STEP 1
#define MIDI_MAX_CC_STEP 15
#define MIDI_LAYERS 3
#define MIDI_ACCEL_FAST 20
#define MIDI_ACCEL_MEDIUM 60

extern MidiDevice midi_device;

const uint8_t cc_led_map[] = {
    1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, -1, -1, -1, -1, 
};
uint8_t current_midi_cc = 1;
uint8_t current_midi_layer = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     [BASE] = LAYOUT_ansi_82(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_DEL,             KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_HOME,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_PGUP,
        MO(FN),   KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,             KC_PGDN,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,            KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                       KC_RALT,  MO(FN),   KC_RCTL,            KC_LEFT,  KC_DOWN,  KC_RGHT),

    [MIDI] = LAYOUT_ansi_82(
        _______,  CC_1,     CC_2,     CC_3,     CC_4,     CC_5,     CC_6,     CC_7,     CC_8,     CC_9,     CC_10,    CC_11,    CC_12,    MIDI_NL,            MIDI_0,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            _______,
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,                                _______,                      _______,  _______,  _______,            _______,  _______,  _______),

    [FN] = LAYOUT_ansi_82(
        QK_BOOT,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_SLEP,            RGB_TOG,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_END,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  TG(MIDI), _______,  _______,  _______,  _______,            KC_MS_U,
        _______,  GU_TOGG,  _______,                                KC_BTN1,                      _______,  _______,  _______,            KC_MS_L,  KC_MS_D,  KC_MS_R),

    [L3] = LAYOUT_ansi_82(
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            _______,
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,                                _______,                      _______,  _______,  _______,            _______,  _______,  _______)

};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [FN] = {ENCODER_CCW_CW(KC_WH_U, KC_WH_D)},
    [MIDI] = {ENCODER_CCW_CW(MIDI_SEND_MINUS_1, MIDI_SEND_PLUS_1)},
    [L3] = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)}
};
#endif

uint8_t calc_cc_value(void) {
    static uint16_t last_encoder_time = 0;
    uint8_t cc_step = MIDI_BASE_CC_STEP;

    uint16_t now = timer_read();
    uint16_t delta = now - last_encoder_time;

    last_encoder_time = now;
    if (delta < MIDI_ACCEL_FAST) {
        cc_step = MIDI_MAX_CC_STEP;
    } else if (delta < MIDI_ACCEL_MEDIUM) {
        cc_step = MIDI_BASE_CC_STEP * 4;
    } else {
        cc_step = MIDI_BASE_CC_STEP;
    }

    return cc_step;
}

void midi_send_cc_relative(MidiDevice* device, uint8_t chan, uint8_t num, int8_t val) {
    midi_send_cc(&midi_device, chan, num, 64 + val);
}

bool rgb_matrix_indicators_user(void) {
    if (layer_state_is(MIDI)) {
        uint8_t led_idx = cc_led_map[current_midi_cc - 1];
        if(led_idx != -1) {
            switch(current_midi_layer) {
                case 0:
                    rgb_matrix_set_color(led_idx, RGB_GREEN);
                    break;
                case 1:
                    rgb_matrix_set_color(led_idx, RGB_RED);
                    break;
                case 2:
                    rgb_matrix_set_color(led_idx, RGB_BLUE);
                    break;
            }
        }
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        uint8_t computed_cc_val = current_midi_cc + 16 * current_midi_layer;

        switch (keycode) {
            case CC_1 ... CC_LAST:
                current_midi_cc = keycode - CC_1 + 1;
                return false;
            case MIDI_SEND_0:
                midi_send_cc_relative(&midi_device, 0, computed_cc_val, 0);
                return false;
            case MIDI_SEND_PLUS_1:
                midi_send_cc_relative(&midi_device, 0, computed_cc_val, calc_cc_value());
                return false;
            case MIDI_SEND_MINUS_1:
                midi_send_cc_relative(&midi_device, 0, computed_cc_val, -calc_cc_value());
                return false;
            case MIDI_NEXT_LAYER:
                current_midi_layer = (current_midi_layer + 1) % MIDI_LAYERS;
                return false;
            case MIDI_PREV_LAYER:
                current_midi_layer = ((int16_t) current_midi_layer - 1) % MIDI_LAYERS;
                return false;
        }
    }

    if(!process_record_lemokey_common(keycode, record)) {
        return false;
    }
    return true;
}
