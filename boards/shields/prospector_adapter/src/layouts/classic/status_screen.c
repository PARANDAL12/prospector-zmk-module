#include <lvgl.h>

#include "layer_roller.h"
#include <fonts.h>

#include <zmk/display.h>
#include <zmk/events/caps_word_state_changed.h>
#include <zmk/event_manager.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static struct zmk_widget_layer_roller layer_roller_widget;
static lv_obj_t *caps_word_label;

struct caps_word_display_state {
    bool active;
};

static void caps_word_display_update(struct caps_word_display_state state) {
    if (caps_word_label == NULL) {
        return;
    }

    lv_obj_set_style_text_color(caps_word_label,
                                lv_color_hex(state.active ? 0xFF8A00 : 0x303030),
                                LV_PART_MAIN);
}

static struct caps_word_display_state caps_word_display_get_state(const zmk_event_t *eh) {
    const struct zmk_caps_word_state_changed *ev = as_zmk_caps_word_state_changed(eh);
    return (struct caps_word_display_state){.active = ev != NULL && ev->active};
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_caps_word_display, struct caps_word_display_state,
                            caps_word_display_update, caps_word_display_get_state)
ZMK_SUBSCRIPTION(widget_caps_word_display, zmk_caps_word_state_changed);

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);

    zmk_widget_layer_roller_init(&layer_roller_widget, screen);
    lv_obj_set_size(zmk_widget_layer_roller_obj(&layer_roller_widget), 224, 90);
    lv_obj_align(zmk_widget_layer_roller_obj(&layer_roller_widget), LV_ALIGN_CENTER, 0, 0);

    caps_word_label = lv_label_create(screen);
    lv_label_set_text(caps_word_label, "CAPS WORD");
    lv_obj_set_width(caps_word_label, 160);
    lv_obj_set_style_text_font(caps_word_label, &DINish_Medium_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(caps_word_label, lv_color_hex(0x303030), LV_PART_MAIN);
    lv_obj_set_style_text_align(caps_word_label, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(caps_word_label, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(caps_word_label, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(caps_word_label, 0, LV_PART_MAIN);
    lv_obj_align(caps_word_label, LV_ALIGN_BOTTOM_RIGHT, -10, -10);

    return screen;
}
