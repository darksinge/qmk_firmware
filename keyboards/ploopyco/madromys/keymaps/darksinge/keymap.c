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
    KC_SNIPER,
};

enum tap_dance_codes {
    TD_DRAG_SCROLL,
    TD_SNIPER_DRAG_SCROLL,
};

extern bool is_drag_scroll;
static bool drag_scroll_toggled = false;
static bool sniper_mode_active = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_BTN4, LT(1, KC_BTN5), TD(TD_DRAG_SCROLL), KC_BTN1,
        KC_BTN1,                                     TD(TD_SNIPER_DRAG_SCROLL)
    ),
    [1] = LAYOUT(
        KC_ENT,      KC_NO, LGUI(KC_C), LGUI(KC_V),
        KC_SNIPER,                     DPI_CONFIG
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t original_dpi;

    switch (keycode) {
        case DRAG_SCROLL_TOG:
            if (record->event.pressed) {
                toggle_drag_scroll();
            }
            break;
        case KC_SNIPER:
            if (record->event.pressed) {
                sniper_mode_active = !sniper_mode_active;
                if (sniper_mode_active) {
                    original_dpi = pointing_device_get_cpi();
                    pointing_device_set_cpi(original_dpi / 2);
                } else {
                    pointing_device_set_cpi(original_dpi);
                }
            }
            return false;
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

void sniper_dance_each(tap_dance_state_t *state, void *user_data) {
    if (state->pressed) {
        is_drag_scroll = true;
    }
}

void sniper_dance_finished(tap_dance_state_t *state, void *user_data) {
    static uint16_t original_dpi;

    if (state->count == 1) {
        if (state->interrupted || !state->pressed) {
            // This was a tap - toggle sniper mode
            sniper_mode_active = !sniper_mode_active;
            if (sniper_mode_active) {
                original_dpi = pointing_device_get_cpi();
                pointing_device_set_cpi(original_dpi / 2);
            } else {
                pointing_device_set_cpi(original_dpi);
            }
        }
    }
}

void sniper_dance_reset(tap_dance_state_t *state, void *user_data) {
    is_drag_scroll = false;
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_DRAG_SCROLL] = ACTION_TAP_DANCE_FN_ADVANCED(
        drag_scroll_tap_dance_each,
        drag_scroll_tap_dance_finished,
        drag_scroll_tap_dance_reset
    ),
    [TD_SNIPER_DRAG_SCROLL] = ACTION_TAP_DANCE_FN_ADVANCED(
        sniper_dance_each,
        sniper_dance_finished,
        sniper_dance_reset
    ),
};

