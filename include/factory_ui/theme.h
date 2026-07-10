#ifndef FACTORY_UI_THEME_H
#define FACTORY_UI_THEME_H

#include <lvgl.h>

#include "factory_ui/styles.h"

namespace factory_ui::theme {

void init();

void apply_screen(lv_obj_t* obj);
void apply_content_column(lv_obj_t* obj);

void apply_header(lv_obj_t* obj);
void apply_header_title(lv_obj_t* obj);
void apply_header_meta(lv_obj_t* obj);

void apply_card(lv_obj_t* obj);
void apply_card_pager(lv_obj_t* obj);
void apply_card_page(lv_obj_t* obj);
void apply_card_page_title(lv_obj_t* obj);
void apply_card_row(lv_obj_t* obj);
void apply_card_row_label(lv_obj_t* obj);
void apply_card_row_value(lv_obj_t* obj);
void apply_page_indicator(lv_obj_t* obj);
void set_page_indicator_active(lv_obj_t* obj, bool active);
void apply_card_icon_tile(lv_obj_t* obj);
void apply_card_icon(lv_obj_t* obj);
void apply_card_title(lv_obj_t* obj);
void apply_card_meta(lv_obj_t* obj);

void apply_button_grid(lv_obj_t* obj);
void apply_button(lv_obj_t* obj, styles::ButtonTone tone);
void apply_button_icon(lv_obj_t* obj);
void apply_button_label(lv_obj_t* obj);
void set_button_selected(lv_obj_t* obj, bool selected);

}  // namespace factory_ui::theme

#endif  // FACTORY_UI_THEME_H
