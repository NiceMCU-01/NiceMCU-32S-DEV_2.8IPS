#ifndef FACTORY_UI_STYLES_H
#define FACTORY_UI_STYLES_H

#include <cstdint>

#include <lvgl.h>

namespace factory_ui::styles {

enum class ButtonTone : uint8_t {
  Home = 0,
  Io,
  Wifi,
  Sd,
};

struct StyleSet {
  lv_style_t screen;
  lv_style_t content_column;

  lv_style_t header;
  lv_style_t header_title;
  lv_style_t header_meta;

  lv_style_t card;
  lv_style_t card_pager;
  lv_style_t card_page;
  lv_style_t card_page_title;
  lv_style_t card_row;
  lv_style_t card_row_label;
  lv_style_t card_row_value;
  lv_style_t page_indicator;
  lv_style_t page_indicator_active;
  lv_style_t card_icon_tile;
  lv_style_t card_icon;
  lv_style_t card_title;
  lv_style_t card_meta;

  lv_style_t button_grid;
  lv_style_t button_base;
  lv_style_t button_pressed;
  lv_style_t button_focus;
  lv_style_t button_selected;

  lv_style_t button_home;
  lv_style_t button_io;
  lv_style_t button_wifi;
  lv_style_t button_sd;

  lv_style_t button_icon;
  lv_style_t button_label;
};

void init();
const StyleSet& get();
lv_color_t color(uint32_t hex);

}  // namespace factory_ui::styles

#endif  // FACTORY_UI_STYLES_H
