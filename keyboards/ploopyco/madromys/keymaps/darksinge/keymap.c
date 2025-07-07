/* Copyright 2023 Colin Lam (Ploopy Corporation)
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
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

enum custom_keycodes {
    DRAG_SCROLL_TOG = SAFE_RANGE,
};

enum tap_dance_codes {
    TD_DRAG_SCROLL,
};

extern bool is_drag_scroll;
static bool drag_scroll_toggled = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_BTN4, KC_BTN5, TD(TD_DRAG_SCROLL), KC_BTN1,
        MO(1),                                              KC_BTN2
    ),
    [1] = LAYOUT(
        DPI_CONFIG, KC_BTN5, DRAG_SCROLL_TOG, KC_BTN2,
        KC_ENT,                              KC_NO
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case DRAG_SCROLL_TOG:
            if (record->event.pressed) {
                toggle_drag_scroll();
            }
            break;
    }
    return true;
}

void drag_scroll_tap_dance_each(tap_dance_state_t *state, void *user_data) {
    if (state->pressed && !drag_scroll_toggled) {
        is_drag_scroll = true;
    }
}

void drag_scroll_tap_dance_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) {
            // This was a tap - toggle the persistent state
            drag_scroll_toggled = !drag_scroll_toggled;
            is_drag_scroll = drag_scroll_toggled;
        }
        // If still pressed, it's a hold - keep drag scroll on temporarily
    }
}

void drag_scroll_tap_dance_reset(tap_dance_state_t *state, void *user_data) {
    if (!drag_scroll_toggled) {
        is_drag_scroll = false;
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_DRAG_SCROLL] = ACTION_TAP_DANCE_FN_ADVANCED(drag_scroll_tap_dance_each, drag_scroll_tap_dance_finished, drag_scroll_tap_dance_reset),
};

/* const uint16_t PROGMEM boot_combo[] = {KC_LEFT, KC_RIGHT, KC_UP, COMBO_END}; */
/**/
/* combo_t key_combos[] = { */
/*     COMBO(boot_combo, QK_BOOT), */
/* }; */
/**/
/* const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = { */
/*     [0] = LAYOUT( */
/*         LT(2,KC_BTN4), // Tap = Button 4, Hold = Layer 2 */
/*         KC_BTN5, */
/*         DRAG_SCROLL, */
/*         LT(3, KC_BTN2), // Tap = Button 2, Hold = Layer 3 */
/*         KC_BTN1, */
/*         LT(1,KC_ENTER) // Tap = Enter, Hold = Layer 1 */
/*     ), */
/*     [1] = LAYOUT( */
/*         KC_UP,  */
/*         KC_LEFT, */
/*         KC_RIGHT, */
/*         LGUI(KC_TAB), // Windows + tab to switch apps */
/*         KC_DOWN, */
/*         KC_NO */
/*     ), */
/*     [2] = LAYOUT( */
/*         KC_NO, */
/*         DPI_CONFIG, // DPI toggle 300/600 */
/*         KC_NO, */
/*         KC_NO, */
/*         LCTL(KC_C), // Copy */
/*         LCTL(KC_V) // Paste */
/*     ), */
/*     [3] = LAYOUT( */
/*         LCTL(KC_BSPC), // delete word left */
/*         KC_NO, */
/*         KC_NO, */
/*         KC_NO, */
/*         KC_BSPC, */
/*         KC_NO */
/*     ) */
