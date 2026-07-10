#include "factory_ui/theme.h"

namespace factory_ui::theme {
namespace {

const lv_style_t& tone_style(styles::ButtonTone tone) {
  const auto& style = styles::get();
  switch (tone) {
    case styles::ButtonTone::Home:
      return style.button_home;
    case styles::ButtonTone::Io:
      return style.button_io;
    case styles::ButtonTone::Wifi:
      return style.button_wifi;
    case styles::ButtonTone::Sd:
      return style.button_sd;
  }

  return style.button_home;
}

}  // namespace

void init() {
  styles::init();
}

void apply_screen(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.screen, LV_PART_MAIN);
}

void apply_content_column(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.content_column, LV_PART_MAIN);
}

void apply_header(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.header, LV_PART_MAIN);
}

void apply_header_title(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.header_title, LV_PART_MAIN);
}

void apply_header_meta(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.header_meta, LV_PART_MAIN);
}

void apply_card(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.card, LV_PART_MAIN);
}

void apply_card_pager(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.card_pager, LV_PART_MAIN);
}

void apply_card_page(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.card_page, LV_PART_MAIN);
}

void apply_card_page_title(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.card_page_title, LV_PART_MAIN);
}

void apply_card_row(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.card_row, LV_PART_MAIN);
}

void apply_card_row_label(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.card_row_label, LV_PART_MAIN);
}

void apply_card_row_value(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.card_row_value, LV_PART_MAIN);
}

void apply_page_indicator(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.page_indicator, LV_PART_MAIN);
}

void set_page_indicator_active(lv_obj_t* obj, bool active) {
  const auto& style = styles::get();
  if(active) {
    lv_obj_add_style(obj, &style.page_indicator_active, LV_PART_MAIN);
  }
  else {
    lv_obj_remove_style(obj, &style.page_indicator_active, LV_PART_MAIN);
  }
}

void apply_card_icon_tile(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.card_icon_tile, LV_PART_MAIN);
}

void apply_card_icon(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.card_icon, LV_PART_MAIN);
}

void apply_card_title(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.card_title, LV_PART_MAIN);
}

void apply_card_meta(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.card_meta, LV_PART_MAIN);
}

void apply_button_grid(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.button_grid, LV_PART_MAIN);
}

void apply_button(lv_obj_t* obj, styles::ButtonTone tone) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.button_base, LV_PART_MAIN);
  lv_obj_add_style(obj, &tone_style(tone), LV_PART_MAIN);
  lv_obj_add_style(obj, &style.button_pressed, LV_PART_MAIN | LV_STATE_PRESSED);
  lv_obj_add_style(obj, &style.button_focus, LV_PART_MAIN | LV_STATE_FOCUSED);
}

void apply_button_icon(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.button_icon, LV_PART_MAIN);
}

void apply_button_label(lv_obj_t* obj) {
  const auto& style = styles::get();
  lv_obj_add_style(obj, &style.button_label, LV_PART_MAIN);
}

void set_button_selected(lv_obj_t* obj, bool selected) {
  const auto& style = styles::get();
  if (selected) {
    lv_obj_add_style(obj, &style.button_selected, LV_PART_MAIN);
  } else {
    lv_obj_remove_style(obj, &style.button_selected, LV_PART_MAIN);
  }
}

}  // namespace factory_ui::theme
