#include "factory_ui/styles.h"

namespace factory_ui::styles {
namespace {

StyleSet g_styles;
bool g_initialized = false;

void init_screen_styles() {
  lv_style_init(&g_styles.screen);
  lv_style_set_bg_color(&g_styles.screen, color(0xF5F7FA));
  lv_style_set_bg_opa(&g_styles.screen, LV_OPA_COVER);
  lv_style_set_pad_all(&g_styles.screen, 0);
  lv_style_set_border_width(&g_styles.screen, 0);
  lv_style_set_radius(&g_styles.screen, 0);

  lv_style_init(&g_styles.content_column);
  lv_style_set_bg_opa(&g_styles.content_column, LV_OPA_TRANSP);
  lv_style_set_pad_top(&g_styles.content_column, 12);
  lv_style_set_pad_bottom(&g_styles.content_column, 12);
  lv_style_set_pad_left(&g_styles.content_column, 12);
  lv_style_set_pad_right(&g_styles.content_column, 12);
  lv_style_set_pad_row(&g_styles.content_column, 10);
  lv_style_set_pad_column(&g_styles.content_column, 0);
  lv_style_set_border_width(&g_styles.content_column, 0);
  lv_style_set_radius(&g_styles.content_column, 0);
}

void init_header_styles() {
  lv_style_init(&g_styles.header);
  lv_style_set_bg_color(&g_styles.header, color(0x3B82F6));
  lv_style_set_bg_grad_color(&g_styles.header, color(0x2563EB));
  lv_style_set_bg_grad_dir(&g_styles.header, LV_GRAD_DIR_HOR);
  lv_style_set_bg_main_stop(&g_styles.header, 0);
  lv_style_set_bg_grad_stop(&g_styles.header, 255);
  lv_style_set_bg_opa(&g_styles.header, LV_OPA_COVER);
  lv_style_set_border_width(&g_styles.header, 0);
  lv_style_set_pad_left(&g_styles.header, 12);
  lv_style_set_pad_right(&g_styles.header, 12);
  lv_style_set_pad_top(&g_styles.header, 0);
  lv_style_set_pad_bottom(&g_styles.header, 0);
  lv_style_set_pad_column(&g_styles.header, 8);
  lv_style_set_radius(&g_styles.header, 0);

  lv_style_init(&g_styles.header_title);
  lv_style_set_text_font(&g_styles.header_title, &lv_font_montserrat_16);
  lv_style_set_text_color(&g_styles.header_title, lv_color_white());

  lv_style_init(&g_styles.header_meta);
  lv_style_set_text_font(&g_styles.header_meta, &lv_font_montserrat_12);
  lv_style_set_text_color(&g_styles.header_meta, lv_color_white());
}

void init_card_styles() {
  lv_style_init(&g_styles.card);
  lv_style_set_bg_color(&g_styles.card, color(0xFFFFFF));
  lv_style_set_bg_opa(&g_styles.card, LV_OPA_COVER);
  lv_style_set_border_width(&g_styles.card, 1);
  lv_style_set_border_color(&g_styles.card, color(0xE5E7EB));
  lv_style_set_pad_top(&g_styles.card, 12);
  lv_style_set_pad_bottom(&g_styles.card, 10);
  lv_style_set_pad_left(&g_styles.card, 12);
  lv_style_set_pad_right(&g_styles.card, 12);
  lv_style_set_pad_row(&g_styles.card, 8);
  lv_style_set_radius(&g_styles.card, 12);
  lv_style_set_shadow_width(&g_styles.card, 10);
  lv_style_set_shadow_color(&g_styles.card, color(0x94A3B8));
  lv_style_set_shadow_opa(&g_styles.card, LV_OPA_10);
  lv_style_set_shadow_spread(&g_styles.card, 0);
  lv_style_set_shadow_ofs_y(&g_styles.card, 3);
  lv_style_set_shadow_ofs_x(&g_styles.card, 0);

  lv_style_init(&g_styles.card_pager);
  lv_style_set_bg_opa(&g_styles.card_pager, LV_OPA_TRANSP);
  lv_style_set_border_width(&g_styles.card_pager, 0);
  lv_style_set_pad_all(&g_styles.card_pager, 0);
  lv_style_set_pad_column(&g_styles.card_pager, 0);

  lv_style_init(&g_styles.card_page);
  lv_style_set_bg_opa(&g_styles.card_page, LV_OPA_TRANSP);
  lv_style_set_border_width(&g_styles.card_page, 0);
  lv_style_set_pad_all(&g_styles.card_page, 0);
  lv_style_set_pad_row(&g_styles.card_page, 6);

  lv_style_init(&g_styles.card_page_title);
  lv_style_set_text_font(&g_styles.card_page_title, &lv_font_montserrat_14);
  lv_style_set_text_color(&g_styles.card_page_title, color(0x111827));

  lv_style_init(&g_styles.card_row);
  lv_style_set_bg_opa(&g_styles.card_row, LV_OPA_TRANSP);
  lv_style_set_border_width(&g_styles.card_row, 0);
  lv_style_set_pad_all(&g_styles.card_row, 0);
  lv_style_set_pad_column(&g_styles.card_row, 8);

  lv_style_init(&g_styles.card_row_label);
  lv_style_set_text_font(&g_styles.card_row_label, &lv_font_montserrat_12);
  lv_style_set_text_color(&g_styles.card_row_label, color(0x6B7280));

  lv_style_init(&g_styles.card_row_value);
  lv_style_set_text_font(&g_styles.card_row_value, &lv_font_montserrat_12);
  lv_style_set_text_color(&g_styles.card_row_value, color(0x111827));

  lv_style_init(&g_styles.page_indicator);
  lv_style_set_bg_color(&g_styles.page_indicator, color(0xCBD5E1));
  lv_style_set_bg_opa(&g_styles.page_indicator, LV_OPA_COVER);
  lv_style_set_radius(&g_styles.page_indicator, LV_RADIUS_CIRCLE);
  lv_style_set_border_width(&g_styles.page_indicator, 0);

  lv_style_init(&g_styles.page_indicator_active);
  lv_style_set_bg_color(&g_styles.page_indicator_active, color(0x2563EB));
  lv_style_set_bg_opa(&g_styles.page_indicator_active, LV_OPA_COVER);

  lv_style_init(&g_styles.card_icon_tile);
  lv_style_set_bg_color(&g_styles.card_icon_tile, color(0xDBEAFE));
  lv_style_set_bg_opa(&g_styles.card_icon_tile, LV_OPA_COVER);
  lv_style_set_radius(&g_styles.card_icon_tile, 12);
  lv_style_set_border_width(&g_styles.card_icon_tile, 0);
  lv_style_set_pad_all(&g_styles.card_icon_tile, 0);

  lv_style_init(&g_styles.card_icon);
  lv_style_set_text_font(&g_styles.card_icon, &lv_font_montserrat_18);
  lv_style_set_text_color(&g_styles.card_icon, color(0x2563EB));

  lv_style_init(&g_styles.card_title);
  lv_style_set_text_font(&g_styles.card_title, &lv_font_montserrat_14);
  lv_style_set_text_color(&g_styles.card_title, color(0x111827));

  lv_style_init(&g_styles.card_meta);
  lv_style_set_text_font(&g_styles.card_meta, &lv_font_montserrat_14);
  lv_style_set_text_color(&g_styles.card_meta, color(0x6B7280));
}

void init_button_styles() {
  lv_style_init(&g_styles.button_grid);
  lv_style_set_bg_opa(&g_styles.button_grid, LV_OPA_TRANSP);
  lv_style_set_border_width(&g_styles.button_grid, 0);
  lv_style_set_pad_all(&g_styles.button_grid, 0);
  lv_style_set_pad_row(&g_styles.button_grid, 0);
  lv_style_set_pad_column(&g_styles.button_grid, 8);

  lv_style_init(&g_styles.button_base);
  lv_style_set_radius(&g_styles.button_base, 10);
  lv_style_set_border_width(&g_styles.button_base, 0);
  lv_style_set_pad_top(&g_styles.button_base, 6);
  lv_style_set_pad_bottom(&g_styles.button_base, 5);
  lv_style_set_pad_left(&g_styles.button_base, 2);
  lv_style_set_pad_right(&g_styles.button_base, 2);
  lv_style_set_pad_row(&g_styles.button_base, 2);
  lv_style_set_pad_column(&g_styles.button_base, 0);
  lv_style_set_text_color(&g_styles.button_base, color(0x1F2937));
  lv_style_set_anim_duration(&g_styles.button_base, 120);
  lv_style_set_transform_scale(&g_styles.button_base, 256);
  lv_style_set_translate_y(&g_styles.button_base, 0);

  lv_style_init(&g_styles.button_pressed);
  lv_style_set_bg_opa(&g_styles.button_pressed, LV_OPA_90);
  lv_style_set_transform_scale(&g_styles.button_pressed, 251);
  lv_style_set_translate_y(&g_styles.button_pressed, 1);

  lv_style_init(&g_styles.button_focus);
  lv_style_set_outline_width(&g_styles.button_focus, 2);
  lv_style_set_outline_color(&g_styles.button_focus, color(0x3B82F6));
  lv_style_set_outline_pad(&g_styles.button_focus, 0);

  lv_style_init(&g_styles.button_selected);
  lv_style_set_bg_color(&g_styles.button_selected, color(0x3B82F6));
  lv_style_set_bg_grad_color(&g_styles.button_selected, color(0x2563EB));
  lv_style_set_bg_grad_dir(&g_styles.button_selected, LV_GRAD_DIR_HOR);
  lv_style_set_text_color(&g_styles.button_selected, lv_color_white());

  lv_style_init(&g_styles.button_home);
  lv_style_set_bg_color(&g_styles.button_home, color(0xEEF2FF));
  lv_style_set_bg_opa(&g_styles.button_home, LV_OPA_COVER);

  lv_style_init(&g_styles.button_io);
  lv_style_set_bg_color(&g_styles.button_io, color(0xEEF2FF));
  lv_style_set_bg_opa(&g_styles.button_io, LV_OPA_COVER);

  lv_style_init(&g_styles.button_wifi);
  lv_style_set_bg_color(&g_styles.button_wifi, color(0xECFDF5));
  lv_style_set_bg_opa(&g_styles.button_wifi, LV_OPA_COVER);

  lv_style_init(&g_styles.button_sd);
  lv_style_set_bg_color(&g_styles.button_sd, color(0xF5F3FF));
  lv_style_set_bg_opa(&g_styles.button_sd, LV_OPA_COVER);

  lv_style_init(&g_styles.button_icon);
  lv_style_set_text_font(&g_styles.button_icon, &lv_font_montserrat_16);

  lv_style_init(&g_styles.button_label);
  lv_style_set_text_font(&g_styles.button_label, &lv_font_montserrat_12);
}

}  // namespace

lv_color_t color(uint32_t hex) {
  return lv_color_hex(hex);
}

void init() {
  if (g_initialized) {
    return;
  }

  init_screen_styles();
  init_header_styles();
  init_card_styles();
  init_button_styles();

  g_initialized = true;
}

const StyleSet& get() {
  init();
  return g_styles;
}

}  // namespace factory_ui::styles
