/* Copyright 2023 dogspace <https://github.com/bobrown101>
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

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

enum custom_keycodes {
    QMKBEST = SAFE_RANGE,
    QMKURL,
    MY_OTHER_MACRO,
};

uint8_t CURRENT_HUE = 0;
uint8_t CURRENT_SATURATION = 255;
uint8_t CURRENT_BRIGHTNESS = 255;

bool set_color_to_red(void){
    CURRENT_HUE = 0;
    CURRENT_SATURATION = 255;
    return true;
}
bool set_color_to_green(void){
    CURRENT_HUE = 85;
    CURRENT_SATURATION = 255;
    return true;
}
bool set_color_to_blue(void){
    CURRENT_HUE = 170;
    CURRENT_SATURATION = 255;
    return true;
}
bool set_color_to_white(void){
    CURRENT_HUE = 0;
    CURRENT_SATURATION = 0;
    return true;
}

bool set_current_color_and_brightness(void) {
    rgblight_sethsv_noeeprom(CURRENT_HUE, CURRENT_SATURATION, CURRENT_BRIGHTNESS);
    return true;
}
bool increase_brightness(void) {
    CURRENT_BRIGHTNESS = MIN(CURRENT_BRIGHTNESS + 10, 255);
    set_current_color_and_brightness();
    return true;
}
bool decrease_brightness(void) {
    CURRENT_BRIGHTNESS = MAX(CURRENT_BRIGHTNESS - 10, 0);
    set_current_color_and_brightness();
    return true;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case QMKBEST:
        if (record->event.pressed) {
            increase_brightness();
            // when keycode QMKBEST is pressed
            // SEND_STRING("QMK is the best thing ever!");
        } else {
            // when keycode QMKBEST is released
        }
        break;

    case QMKURL:
        if (record->event.pressed) {
            decrease_brightness();
            // when keycode QMKURL is pressed
            // SEND_STRING("https://qmk.fm/\n");
        } else {
            // when keycode QMKURL is released
        }
        break;

    case MY_OTHER_MACRO:
        if (record->event.pressed) {
           SEND_STRING(SS_LCTL("ac")); // selects all and copies
        }
        break;
    }
    return true;
};

enum layer_names {
    _LAY0 = 0,
    _LAY1 = 1,
    _LAY2 = 2,
    _LAY3 = 3
};

//hsv are numbers between 0 and 255
//rgblight_sethsv_noeeprom(h, s, v)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_LAY0] = LAYOUT(
                            KC_A, KC_B, TO(_LAY1),
    KC_C, KC_D, KC_E, KC_F,   KC_G,   KC_H,
    KC_I, KC_J, KC_K, KC_L,   KC_M,   KC_N,
    KC_O, KC_U, KC_P, KC_Q,   KC_R,   KC_S,
    KC_T, KC_U, KC_V, KC_Q,   KC_X, KC_Y
    ),

    [_LAY1] = LAYOUT(
                      _______, _______, TO(_LAY2),
    _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______
    ),

    [_LAY2] = LAYOUT(
                      _______, _______, TO(_LAY3),
    _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______
    ),

    [_LAY3] = LAYOUT(
                      _______, _______, TO(_LAY0),
    _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,
    QMKBEST, QMKURL, _______, _______, _______, _______
    ),
    //previously these were RGB_VAI and RGB_VAD
};

#ifdef OLED_ENABLE
/*===========================================    OLED CONFIGURATION    ===========================================*/
#define OLED_ROTATE      true         // OLED rotation (flip 180* from default orientation)
// #define GRAPH_DIRECTION  true         // Graph movement  (true = right to left,  false = left to right)
// #define GRAPH_TOP_WPM    100.0        // Minimum WPM required to reach the top of the graph
// #define GRAPH_REFRESH    1000         // In milliseconds, determines the graph-line frequency
// #define ICON_MED_WPM     10           // WPM required to display the medium snail
// #define ICON_FAST_WPM    25           // WPM required to display the fast snail

// Layer names:  Should be exactly 5 characters in length if vertical display, or 6 characters if horizontal
#define L0_LAYER_NAME     "DEBUG"      // Layer _MA name
#define L1_LAYER_NAME     "VIM  "      // Layer _L1 name
#define L2_LAYER_NAME     "EMPTY"      // Layer _L2 name
#define L3_LAYER_NAME     "COLOR"      // Layer _L3 name

// #define CAPLCK_STR        "CAPLK"       // Caps Lock string
// #define NUMLCK_STR        "NUMLK"       // Num Lock string
// #define SCRLK_STR         "SCRLK"       // Scroll Lock string
// #define EMPTY_STR         "     "       // Empty string

/*================================================================================================================*/

typedef struct oled_params {
    bool first_loop : 1;
} oled_params;

oled_params oled_data;

void oled_init_data(void) {
    // Initialize oled params
    oled_data.first_loop = true;
}

// Set OLED rotation
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    oled_init_data();
    return OLED_ROTATE ? OLED_ROTATION_270 : OLED_ROTATION_90;
}

// Draw static background image to OLED (keyboard with no bottom row)
// static void render_background(void) {
//     static const char PROGMEM nullbits_n_oled[] = {
//         0x00, 0xe0, 0xf0, 0xf0, 0xf8, 0xf8, 0xf0, 0xf0, 0xe0, 0x80, 0x20, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0,
//         0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xe0, 0xe0, 0xe0, 0xc0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
//         0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f,
//         0x1f, 0x1f, 0x1f, 0x1f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfc, 0xf0, 0x00, 0x00,
//         0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00,
//         0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
//         0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
//         0x00, 0x07, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x07, 0x03, 0x00,
//     };
//     oled_write_raw_P(nullbits_n_oled, sizeof(nullbits_n_oled));
// }

// // Toggles pixel on/off, converts horizontal coordinates to vertical equivalent if necessary
// static void write_pixel(uint8_t x, uint8_t y, bool onoff) {
//     oled_write_pixel(y, 127 - x, onoff);
// }

// Write active layer name
static void render_layer_state(void) {
  oled_set_cursor(0, 0);
  switch (get_highest_layer(layer_state)) {
  case _LAY0:
      oled_write_P(PSTR(L0_LAYER_NAME), false);
      break;
  case _LAY1:
      oled_write_P(PSTR(L1_LAYER_NAME), false);
      break;
  case _LAY2:
      oled_write_P(PSTR(L2_LAYER_NAME), false);
      break;
  case _LAY3:
      oled_write_P(PSTR(L3_LAYER_NAME), false);
      break;
  default:
      oled_write("ERROR", false);
      break;
  }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case _LAY0:
            set_color_to_red();
            break;
        case _LAY1:
            set_color_to_green();
            break;
        case _LAY2:
            set_color_to_blue();
            break;
        case _LAY3:
            set_color_to_white();
            break;
        default: // for any other layers, or the default layer
            oled_write("ERR BG", false);
            break;
    }
  set_current_color_and_brightness();
  return state;
}


// Call OLED functions
bool oled_task_user(void) {
    render_layer_state();

    return false;
}
#endif

bool wpm_keycode_user(uint16_t keycode) {
    // Count all keycodes on the macropad
    return false;
}

