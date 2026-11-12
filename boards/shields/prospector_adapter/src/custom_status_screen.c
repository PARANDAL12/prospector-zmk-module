#include <lvgl.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>

#include "widgets/layer_roller.h"
#include "widgets/modifier_indicator.h"

#include <fonts.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static struct zmk_widget_layer_roller layer_roller_widget;
static struct zmk_widget_modifier_indicator modifier_indicator_widget;

static void create_black_cat(lv_obj_t *parent) {
    lv_obj_t *cat = lv_obj_create(parent);
    lv_obj_set_size(cat, 76, 62);
    lv_obj_align(cat, LV_ALIGN_BOTTOM_LEFT, 8, -7);
    lv_obj_set_style_bg_opa(cat, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(cat, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(cat, 0, LV_PART_MAIN);

    // Ears: rotated charcoal squares sit behind the face.
    lv_obj_t *left_ear = lv_obj_create(cat);
    lv_obj_set_size(left_ear, 18, 18);
    lv_obj_align(left_ear, LV_ALIGN_TOP_LEFT, 13, 5);
    lv_obj_set_style_radius(left_ear, 4, LV_PART_MAIN);
    lv_obj_set_style_bg_color(left_ear, lv_color_hex(0x34303A), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(left_ear, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(left_ear, 0, LV_PART_MAIN);
    lv_obj_set_style_transform_angle(left_ear, 450, LV_PART_MAIN);

    lv_obj_t *right_ear = lv_obj_create(cat);
    lv_obj_set_size(right_ear, 18, 18);
    lv_obj_align(right_ear, LV_ALIGN_TOP_LEFT, 45, 5);
    lv_obj_set_style_radius(right_ear, 4, LV_PART_MAIN);
    lv_obj_set_style_bg_color(right_ear, lv_color_hex(0x34303A), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(right_ear, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(right_ear, 0, LV_PART_MAIN);
    lv_obj_set_style_transform_angle(right_ear, 450, LV_PART_MAIN);

    // Rounded black face.
    lv_obj_t *face = lv_obj_create(cat);
    lv_obj_set_size(face, 54, 42);
    lv_obj_align(face, LV_ALIGN_TOP_LEFT, 11, 14);
    lv_obj_set_style_radius(face, 19, LV_PART_MAIN);
    lv_obj_set_style_bg_color(face, lv_color_hex(0x29242F), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(face, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_color(face, lv_color_hex(0x51445F), LV_PART_MAIN);
    lv_obj_set_style_border_width(face, 1, LV_PART_MAIN);
    lv_obj_set_style_pad_all(face, 0, LV_PART_MAIN);

    // Bright amber eyes and a tiny warm-pink nose.
    lv_obj_t *left_eye = lv_obj_create(cat);
    lv_obj_set_size(left_eye, 7, 7);
    lv_obj_align(left_eye, LV_ALIGN_TOP_LEFT, 25, 29);
    lv_obj_set_style_radius(left_eye, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(left_eye, lv_color_hex(0xFFB52E), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(left_eye, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(left_eye, 0, LV_PART_MAIN);

    lv_obj_t *right_eye = lv_obj_create(cat);
    lv_obj_set_size(right_eye, 7, 7);
    lv_obj_align(right_eye, LV_ALIGN_TOP_LEFT, 45, 29);
    lv_obj_set_style_radius(right_eye, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(right_eye, lv_color_hex(0xFFB52E), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(right_eye, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(right_eye, 0, LV_PART_MAIN);

    lv_obj_t *nose = lv_obj_create(cat);
    lv_obj_set_size(nose, 5, 4);
    lv_obj_align(nose, LV_ALIGN_TOP_LEFT, 36, 39);
    lv_obj_set_style_radius(nose, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(nose, lv_color_hex(0xFF8A9B), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(nose, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(nose, 0, LV_PART_MAIN);
}

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);

    zmk_widget_modifier_indicator_init(&modifier_indicator_widget, screen);
    lv_obj_align(zmk_widget_modifier_indicator_obj(&modifier_indicator_widget),
                 LV_ALIGN_BOTTOM_RIGHT, -10, -10);

    zmk_widget_layer_roller_init(&layer_roller_widget, screen);
    lv_obj_set_size(zmk_widget_layer_roller_obj(&layer_roller_widget), 224, 90);
    lv_obj_align(zmk_widget_layer_roller_obj(&layer_roller_widget), LV_ALIGN_CENTER, 0, 0);

    create_black_cat(screen);

    return screen;
}
