#include "layer_roller.h"

#include <stdio.h>
#include <string.h>

#include <zmk/display.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/event_manager.h>
#include <zmk/keymap.h>

#include <fonts.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#define LAYER_NAME_BUFFER_SIZE 32

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct layer_roller_state {
    uint8_t index;
    char name[LAYER_NAME_BUFFER_SIZE];
};

static void layer_roller_set_text(lv_obj_t *label, const struct layer_roller_state *state) {
    lv_label_set_text(label, state->name);
}

static void layer_roller_update_cb(struct layer_roller_state state) {
    struct zmk_widget_layer_roller *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        layer_roller_set_text(widget->obj, &state);
    }
}

static struct layer_roller_state layer_roller_get_state(const zmk_event_t *eh) {
    uint8_t index = zmk_keymap_highest_layer_active();
    uint8_t layer_id = zmk_keymap_layer_index_to_id(index);
    const char *layer_name = zmk_keymap_layer_name(layer_id);

    struct layer_roller_state state = {.index = index};
    if (layer_name != NULL && layer_name[0] != '\0') {
        snprintf(state.name, sizeof(state.name), "%s", layer_name);
    } else {
        snprintf(state.name, sizeof(state.name), "LAYER %u", index);
    }

    LOG_INF("Prospector active layer: %u (%s)", index, state.name);
    return state;
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_layer_roller, struct layer_roller_state, layer_roller_update_cb,
                            layer_roller_get_state)
ZMK_SUBSCRIPTION(widget_layer_roller, zmk_layer_state_changed);

int zmk_widget_layer_roller_init(struct zmk_widget_layer_roller *widget, lv_obj_t *parent) {
    widget->obj = lv_label_create(parent);

    lv_label_set_text(widget->obj, "BASE");
    lv_obj_set_width(widget->obj, 224);
    lv_obj_set_style_text_font(widget->obj, &FRAC_Regular_48, LV_PART_MAIN);
    lv_obj_set_style_text_color(widget->obj, lv_color_hex(0xFF8A00), LV_PART_MAIN);
    lv_obj_set_style_text_align(widget->obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(widget->obj, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(widget->obj, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(widget->obj, 0, LV_PART_MAIN);

    sys_slist_append(&widgets, &widget->node);

    widget_layer_roller_init();
    return 0;
}

lv_obj_t *zmk_widget_layer_roller_obj(struct zmk_widget_layer_roller *widget) {
    return widget->obj;
}
