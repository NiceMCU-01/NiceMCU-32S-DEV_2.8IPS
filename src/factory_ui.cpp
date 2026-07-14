#include "factory_ui/app.h"

#include <cstddef>
#include <cstdio>
#include <cstdint>
#include <lvgl.h>

#include "factory_ui/styles.h"
#include "factory_ui/theme.h"

namespace factory_ui {
namespace {

enum class FactoryPage : uint8_t {
  Home = 0,
  Io,
  Wifi,
  Sd,
};

struct CardRow {
  const char* label;
  const char* value;
};

struct CardPage {
  const char* title;
  const CardRow* rows;
  size_t row_count;
};

struct TabDescriptor {
  FactoryPage page;
  styles::ButtonTone tone;
  const char* tab_label;
  const char* tab_icon;
  const CardPage* pages;
  size_t page_count;
};

constexpr CardRow kHomeDeviceRows[] = {
    {"Board", "NiceMCU-32S-DEV_2.8IPS"},
    {"GitHub", "NiceMCU-01"},
    // {"CPU", "ESP32"},
    // {"Flash", "4MB"},
    // {"PSRAM", "None"},
    {"Display", "ST7789 240x320"},
    {"Touch", "CST816D"},
    // {"LVGL", "9.5.0"},
};

constexpr CardRow kHomeStatusRows[] = {
    {"Uptime", "02:15:36"},
    // {"Free Heap", "198.6 KB"},
    // {"CPU Usage", "12%"},
    {"Battery", "4.12V"},
    // {"Temperature", "36.5 C"},
    {"Temp", "36.5 C"},
};

constexpr CardRow kIoMapRows[] = {
    {"LED RGB", "GPIO4/16/17"},
    {"Key", "GPIO35"},
    // {"Speaker", "GPIO26"},
    {"VBAT", "GPIO34"},
    // {"Backlight", "GPIO25"},
    {"BL", "GPIO25"},
    // {"Touch I2C", "32/33/36"},
};

constexpr CardRow kIoCheckRows[] = {
    {"RGB LED", "Ready"},
    {"Button", "Input only"},
    // {"Speaker", "PWM output"},
    // {"Battery ADC", "Sampling path"},
    // {"Touch Reset", "Shared GPIO2"},
    {"Panel SPI", "Stable"},
};

constexpr CardRow kWifiSummaryRows[] = {
    {"Mode", "Station"},
    {"WiFi SSID", "FactoryNet"},
    // {"IP Address", "192.168.4.1"},
    // {"Gateway", "192.168.4.1"},
    // {"Signal", "-48 dBm"},
    // {"Status", "Connected"},
};

constexpr CardRow kWifiConnRows[] = {
    {"Hostname", "factory-ui"},
    // {"MAC", "24:6F:28:AA:10:32"},
    // {"Channel", "6"},
    // {"DHCP", "Enabled"},
    // {"DNS", "114.114.114.114"},
    // {"Last Scan", "2 min ago"},
};

constexpr CardRow kSdSummaryRows[] = {
    {"Slot", "TF / MicroSD"},
    {"Bus", "HSPI"},
    {"Mount", "Ready"},
    // {"FS Type", "FAT32"},
    // {"Capacity", "7.4 GB"},
    // {"Free Space", "6.1 GB"},
};

constexpr CardRow kSdFsRows[] = {
    {"Root Dir", "/"},
    // {"Read Test", "Pass"},
    // {"Write Test", "Pending"},
    // {"List Files", "Enabled"},
    {"Card Detect", "GPIO5"},
    {"Status", "Idle"},
};

constexpr CardPage kHomePages[] = {
    {"Device Information", kHomeDeviceRows, sizeof(kHomeDeviceRows) / sizeof(kHomeDeviceRows[0])},
    {"System Status", kHomeStatusRows, sizeof(kHomeStatusRows) / sizeof(kHomeStatusRows[0])},
};

constexpr CardPage kIoPages[] = {
    {"Board IO Map", kIoMapRows, sizeof(kIoMapRows) / sizeof(kIoMapRows[0])},
    {"IO Test Checklist", kIoCheckRows, sizeof(kIoCheckRows) / sizeof(kIoCheckRows[0])},
};

constexpr CardPage kWifiPages[] = {
    {"Network Summary", kWifiSummaryRows, sizeof(kWifiSummaryRows) / sizeof(kWifiSummaryRows[0])},
    {"Connection Info", kWifiConnRows, sizeof(kWifiConnRows) / sizeof(kWifiConnRows[0])},
};

constexpr CardPage kSdPages[] = {
    {"Storage Summary", kSdSummaryRows, sizeof(kSdSummaryRows) / sizeof(kSdSummaryRows[0])},
    {"File System Status", kSdFsRows, sizeof(kSdFsRows) / sizeof(kSdFsRows[0])},
};

constexpr TabDescriptor kTabs[] = {
    {FactoryPage::Home, styles::ButtonTone::Home, "Home", LV_SYMBOL_HOME, kHomePages,
     sizeof(kHomePages) / sizeof(kHomePages[0])},
    {FactoryPage::Io, styles::ButtonTone::Io, "IO", LV_SYMBOL_SETTINGS, kIoPages,
     sizeof(kIoPages) / sizeof(kIoPages[0])},
    {FactoryPage::Wifi, styles::ButtonTone::Wifi, "WiFi", LV_SYMBOL_WIFI, kWifiPages,
     sizeof(kWifiPages) / sizeof(kWifiPages[0])},
    {FactoryPage::Sd, styles::ButtonTone::Sd, "SD", LV_SYMBOL_DIRECTORY, kSdPages,
     sizeof(kSdPages) / sizeof(kSdPages[0])},
};

constexpr size_t kTabCount = sizeof(kTabs) / sizeof(kTabs[0]);
constexpr size_t kMaxIndicatorDots = 4;

struct TouchZone {
  FactoryPage page;
  lv_coord_t x;
  lv_coord_t y;
  lv_coord_t width;
  lv_coord_t height;
};

// These transparent zones enlarge the navigation hit targets without changing
// the visual button layout. The coordinates are in the 240 x 320 screen space.
constexpr TouchZone kTabTouchZones[] = {
    {FactoryPage::Home, 2, 252, 58, 67},
    {FactoryPage::Io, 68, 252, 48, 67},
    {FactoryPage::Wifi, 124, 252, 48, 67},
    {FactoryPage::Sd, 180, 252, 56, 67},
};

struct ViewState {
  lv_obj_t* card_pager = nullptr;
  lv_obj_t* indicator_row = nullptr;
  lv_obj_t* indicator_dots[kMaxIndicatorDots] = {};
  lv_obj_t* buttons[kTabCount] = {};
  lv_obj_t* touch_debug_label = nullptr;
  size_t current_page_count = 0;
  size_t current_page_index = 0;
  FactoryPage active_page = FactoryPage::Home;
  uint16_t touch_x = 0;
  uint16_t touch_y = 0;
  bool touch_pressed = false;
  bool touch_read_ok = false;
  bool touch_debug_initialized = false;
};

ViewState g_view;

const TabDescriptor& tab_for(FactoryPage page) {
  for(size_t i = 0; i < kTabCount; ++i) {
    if(kTabs[i].page == page) {
      return kTabs[i];
    }
  }

  return kTabs[0];
}

lv_obj_t* make_label(lv_obj_t* parent, const char* text) {
  auto* label = lv_label_create(parent);
  lv_label_set_text(label, text);
  return label;
}

void set_indicator_page(size_t page_index) {
  if(g_view.current_page_count == 0) {
    g_view.current_page_index = 0;
    return;
  }

  if(page_index >= g_view.current_page_count) {
    page_index = g_view.current_page_count - 1;
  }

  g_view.current_page_index = page_index;
  for(size_t i = 0; i < kMaxIndicatorDots; ++i) {
    if(g_view.indicator_dots[i] == nullptr) {
      continue;
    }
    const bool visible = i < g_view.current_page_count;
    const bool active = visible && i == page_index;
    theme::set_page_indicator_active(g_view.indicator_dots[i], active);
    lv_obj_set_size(g_view.indicator_dots[i], active ? 10 : 6, 6);
    if(!visible) {
      lv_obj_add_flag(g_view.indicator_dots[i], LV_OBJ_FLAG_HIDDEN);
    }
    else {
      lv_obj_remove_flag(g_view.indicator_dots[i], LV_OBJ_FLAG_HIDDEN);
    }
  }
}

size_t current_pager_page() {
  if(g_view.card_pager == nullptr || g_view.current_page_count == 0) {
    return 0;
  }

  const int32_t page_width = lv_obj_get_content_width(g_view.card_pager);
  if(page_width <= 0) {
    return 0;
  }

  const int32_t scroll_x = lv_obj_get_scroll_x(g_view.card_pager);
  size_t index = static_cast<size_t>((scroll_x + (page_width / 2)) / page_width);
  if(index >= g_view.current_page_count) {
    index = g_view.current_page_count - 1;
  }
  return index;
}

void on_card_scroll_end(lv_event_t* event) {
  if(lv_event_get_code(event) != LV_EVENT_SCROLL_END) {
    return;
  }

  set_indicator_page(current_pager_page());
}

void build_card_page(lv_obj_t* parent, const CardPage& page) {
  auto* page_obj = lv_obj_create(parent);
  lv_obj_remove_style_all(page_obj);
  theme::apply_card_page(page_obj);
  lv_obj_add_flag(page_obj, LV_OBJ_FLAG_SNAPPABLE);
  lv_obj_remove_flag(page_obj, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_size(page_obj, LV_PCT(100), LV_PCT(100));
  lv_obj_set_layout(page_obj, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(page_obj, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(page_obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

  auto* title = make_label(page_obj, page.title);
  theme::apply_card_page_title(title);
  lv_obj_set_width(title, LV_PCT(100));
  lv_label_set_long_mode(title, LV_LABEL_LONG_DOT);

  for(size_t i = 0; i < page.row_count; ++i) {
    auto* row = lv_obj_create(page_obj);
    lv_obj_remove_style_all(row);
    theme::apply_card_row(row);
    lv_obj_remove_flag(row, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_width(row, LV_PCT(100));
    lv_obj_set_height(row, LV_SIZE_CONTENT);
    lv_obj_set_style_min_height(row, 16, LV_PART_MAIN);
    lv_obj_set_layout(row, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

    auto* label = make_label(row, page.rows[i].label);
    theme::apply_card_row_label(label);
    lv_obj_set_width(label, 54);
    lv_label_set_long_mode(label, LV_LABEL_LONG_DOT);

    auto* value = make_label(row, page.rows[i].value);
    theme::apply_card_row_value(value);
    lv_obj_set_width(value, 120);
    lv_label_set_long_mode(value, LV_LABEL_LONG_DOT);
  }
}

void rebuild_card_pages(const TabDescriptor& tab) {
  lv_obj_clean(g_view.card_pager);
  lv_obj_clean(g_view.indicator_row);
  g_view.current_page_count = tab.page_count;
  g_view.current_page_index = 0;

  for(size_t i = 0; i < tab.page_count; ++i) {
    build_card_page(g_view.card_pager, tab.pages[i]);
  }

  for(size_t i = 0; i < kMaxIndicatorDots; ++i) {
    if(i >= tab.page_count) {
      g_view.indicator_dots[i] = nullptr;
      continue;
    }

    auto* dot = lv_obj_create(g_view.indicator_row);
    g_view.indicator_dots[i] = dot;
    lv_obj_remove_style_all(dot);
    theme::apply_page_indicator(dot);
    lv_obj_remove_flag(dot, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(dot, i == 0 ? 10 : 6, 6);
  }

  lv_obj_update_layout(g_view.card_pager);
  lv_obj_scroll_to_x(g_view.card_pager, 0, LV_ANIM_OFF);
  set_indicator_page(0);
}

void set_active_page(FactoryPage page) {
  g_view.active_page = page;
  const auto& tab = tab_for(page);

  for(size_t i = 0; i < kTabCount; ++i) {
    const bool selected = kTabs[i].page == page;
    theme::set_button_selected(g_view.buttons[i], selected);
  }

  rebuild_card_pages(tab);
}

void on_button_pressed(lv_event_t* event) {
  if (lv_event_get_code(event) != LV_EVENT_PRESSED) {
    return;
  }

  auto page = static_cast<FactoryPage>(reinterpret_cast<uintptr_t>(lv_event_get_user_data(event)));
  set_active_page(page);
}

void build_header(lv_obj_t* parent) {
  auto* header = lv_obj_create(parent);
  lv_obj_remove_style_all(header);
  theme::apply_header(header);
  lv_obj_remove_flag(header, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_size(header, LV_PCT(100), 40);
  lv_obj_set_layout(header, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(header, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(header, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);

  auto* title = make_label(header, "Factory UI");
  theme::apply_header_title(title);

  g_view.touch_debug_label = make_label(header, "T: WAIT");
  theme::apply_header_meta(g_view.touch_debug_label);
}

void build_device_card(lv_obj_t* parent) {
  auto* card = lv_obj_create(parent);
  lv_obj_remove_style_all(card);
  theme::apply_card(card);
  lv_obj_remove_flag(card, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_size(card, LV_PCT(100), 188);
  lv_obj_set_layout(card, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(card, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(card, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

  g_view.card_pager = lv_obj_create(card);
  lv_obj_remove_style_all(g_view.card_pager);
  theme::apply_card_pager(g_view.card_pager);
  lv_obj_set_width(g_view.card_pager, LV_PCT(100));
  lv_obj_set_flex_grow(g_view.card_pager, 1);
  lv_obj_set_scroll_dir(g_view.card_pager, LV_DIR_HOR);
  lv_obj_set_scrollbar_mode(g_view.card_pager, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_scroll_snap_x(g_view.card_pager, LV_SCROLL_SNAP_CENTER);
  lv_obj_add_flag(g_view.card_pager, LV_OBJ_FLAG_SCROLL_ONE);
  lv_obj_set_layout(g_view.card_pager, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(g_view.card_pager, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(g_view.card_pager, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_START);
  lv_obj_add_event_cb(g_view.card_pager, on_card_scroll_end, LV_EVENT_SCROLL_END, nullptr);

  g_view.indicator_row = lv_obj_create(card);
  lv_obj_remove_style_all(g_view.indicator_row);
  lv_obj_remove_flag(g_view.indicator_row, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_opa(g_view.indicator_row, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(g_view.indicator_row, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(g_view.indicator_row, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(g_view.indicator_row, 6, LV_PART_MAIN);
  lv_obj_set_size(g_view.indicator_row, LV_PCT(100), 8);
  lv_obj_set_layout(g_view.indicator_row, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(g_view.indicator_row, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(g_view.indicator_row, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
}

void build_button_grid(lv_obj_t* parent) {
  auto* grid = lv_obj_create(parent);
  lv_obj_remove_style_all(grid);
  theme::apply_button_grid(grid);
  lv_obj_remove_flag(grid, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_size(grid, LV_PCT(100), 56);
  lv_obj_set_layout(grid, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(grid, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(grid, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);

  for(size_t i = 0; i < kTabCount; ++i) {
    auto* button = lv_button_create(grid);
    g_view.buttons[i] = button;
    lv_obj_remove_style_all(button);
    theme::apply_button(button, kTabs[i].tone);
    lv_obj_remove_flag(button, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(button, 48, 56);
    lv_obj_set_layout(button, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(button, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(button, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    auto* icon = make_label(button, kTabs[i].tab_icon);
    theme::apply_button_icon(icon);

    auto* label = make_label(button, kTabs[i].tab_label);
    theme::apply_button_label(label);
  }
}

void build_touch_zones(lv_obj_t* screen) {
  for(const auto& zone : kTabTouchZones) {
    auto* target = lv_obj_create(screen);
    lv_obj_remove_style_all(target);
    lv_obj_add_flag(target, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_add_flag(target, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(target, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_pos(target, zone.x, zone.y);
    lv_obj_set_size(target, zone.width, zone.height);
    lv_obj_add_event_cb(target, on_button_pressed, LV_EVENT_PRESSED,
                        reinterpret_cast<void*>(static_cast<uintptr_t>(zone.page)));
  }
}

}  // namespace

void build() {
  theme::init();

  auto* screen = lv_screen_active();
  lv_obj_remove_style_all(screen);
  theme::apply_screen(screen);
  lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_layout(screen, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(screen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

  build_header(screen);

  auto* content = lv_obj_create(screen);
  lv_obj_remove_style_all(content);
  theme::apply_content_column(content);
  lv_obj_remove_flag(content, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_width(content, LV_PCT(100));
  lv_obj_set_flex_grow(content, 1);
  lv_obj_set_layout(content, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(content, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_CENTER);

  build_device_card(content);
  build_button_grid(content);
  build_touch_zones(screen);

  set_active_page(FactoryPage::Home);
}

void set_touch_debug(uint16_t raw_x, uint16_t raw_y, bool pressed, bool read_ok) {
  if(g_view.touch_debug_label == nullptr) {
    return;
  }

  if(g_view.touch_debug_initialized && g_view.touch_x == raw_x && g_view.touch_y == raw_y &&
     g_view.touch_pressed == pressed && g_view.touch_read_ok == read_ok) {
    return;
  }

  g_view.touch_x = raw_x;
  g_view.touch_y = raw_y;
  g_view.touch_pressed = pressed;
  g_view.touch_read_ok = read_ok;
  g_view.touch_debug_initialized = true;

  char text[32] = {};
  if(!read_ok) {
    std::snprintf(text, sizeof(text), "T: ERR %03u,%03u", raw_x, raw_y);
  }
  else {
    std::snprintf(text, sizeof(text), "T: %s %03u,%03u", pressed ? "DOWN" : "UP", raw_x,
                  raw_y);
  }
  lv_label_set_text(g_view.touch_debug_label, text);
}

}  // namespace factory_ui
