#include "nicemcu/display.h"

#include <Arduino.h>
#include <Wire.h>
#include <driver/spi_master.h>
#include <esp_heap_caps.h>
#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_vendor.h>
#include <lvgl.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>

#include "nicemcu/board_config.h"

namespace nicemcu {
namespace display {
namespace {

constexpr spi_host_device_t kDisplaySpiHost = SPI3_HOST;
constexpr int kBacklightChannel = 15;
constexpr uint32_t kBacklightFrequency = 400;
constexpr uint8_t kBacklightResolution = 8;
constexpr uint32_t kBacklightMaximum = (1U << kBacklightResolution) - 1U;
constexpr uint8_t kTouchDataRegister = 0x02;

esp_lcd_panel_handle_t g_panel = nullptr;
lv_display_t* g_display = nullptr;
lv_indev_t* g_touch = nullptr;
void* g_draw_buffer = nullptr;

bool check(esp_err_t result, const char* operation) {
  if(result == ESP_OK) {
    return true;
  }
  Serial.printf("[display] %s failed: %s\n", operation, esp_err_to_name(result));
  return false;
}

void reset_panel_and_touch() {
  pinMode(board::kSharedReset, OUTPUT);
  digitalWrite(board::kSharedReset, LOW);
  delay(20);
  digitalWrite(board::kSharedReset, HIGH);
  delay(50);
}

bool on_color_transfer_done(esp_lcd_panel_io_handle_t, esp_lcd_panel_io_event_data_t*,
                            void* context) {
  auto* display = static_cast<lv_display_t*>(context);
  if(display != nullptr) {
    lv_display_flush_ready(display);
  }
  return false;
}

void flush_display(lv_display_t* display, const lv_area_t* area, uint8_t* pixel_data) {
  auto* pixels = reinterpret_cast<uint16_t*>(pixel_data);
  const uint32_t pixel_count = lv_area_get_size(area);
  for(uint32_t i = 0; i < pixel_count; ++i) {
    pixels[i] = static_cast<uint16_t>((pixels[i] >> 8) | (pixels[i] << 8));
  }

  const esp_err_t result = esp_lcd_panel_draw_bitmap(
      g_panel, area->x1, area->y1, area->x2 + 1, area->y2 + 1, pixel_data);
  if(result != ESP_OK) {
    Serial.printf("[display] flush failed: %s\n", esp_err_to_name(result));
    lv_display_flush_ready(display);
  }
}

bool init_display() {
  g_display = lv_display_create(board::kDisplayWidth, board::kDisplayHeight);
  if(g_display == nullptr) {
    Serial.println("[display] unable to create LVGL display");
    return false;
  }

  const std::size_t draw_buffer_bytes = board::kDrawBufferPixels * sizeof(lv_color_t);
  g_draw_buffer = heap_caps_malloc(draw_buffer_bytes, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
  if(g_draw_buffer == nullptr) {
    Serial.println("[display] unable to allocate DMA draw buffer");
    return false;
  }
  lv_display_set_buffers(g_display, g_draw_buffer, nullptr, draw_buffer_bytes,
                         LV_DISPLAY_RENDER_MODE_PARTIAL);

  spi_bus_config_t bus_config = {};
  bus_config.mosi_io_num = board::kDisplayMosi;
  bus_config.miso_io_num = GPIO_NUM_NC;
  bus_config.sclk_io_num = board::kDisplayClock;
  bus_config.quadwp_io_num = GPIO_NUM_NC;
  bus_config.quadhd_io_num = GPIO_NUM_NC;
  bus_config.max_transfer_sz = static_cast<int>(draw_buffer_bytes);
  if(!check(spi_bus_initialize(kDisplaySpiHost, &bus_config, SPI_DMA_CH_AUTO),
            "SPI bus initialization")) {
    return false;
  }

  esp_lcd_panel_io_spi_config_t io_config = {};
  io_config.cs_gpio_num = board::kDisplayChipSelect;
  io_config.dc_gpio_num = board::kDisplayDataCommand;
  io_config.spi_mode = 0;
  io_config.pclk_hz = board::kDisplayClockHz;
  io_config.trans_queue_depth = 10;
  io_config.on_color_trans_done = on_color_transfer_done;
  io_config.user_ctx = g_display;
  io_config.lcd_cmd_bits = 8;
  io_config.lcd_param_bits = 8;

  esp_lcd_panel_io_handle_t panel_io = nullptr;
  if(!check(esp_lcd_new_panel_io_spi(
                 reinterpret_cast<esp_lcd_spi_bus_handle_t>(kDisplaySpiHost), &io_config,
                 &panel_io),
            "ST7789 panel IO creation")) {
    return false;
  }

  esp_lcd_panel_dev_config_t panel_config = {};
  panel_config.reset_gpio_num = GPIO_NUM_NC;
  panel_config.color_space = ESP_LCD_COLOR_SPACE_RGB;
  panel_config.bits_per_pixel = 16;
  if(!check(esp_lcd_new_panel_st7789(panel_io, &panel_config, &g_panel),
            "ST7789 panel creation") ||
     !check(esp_lcd_panel_init(g_panel), "ST7789 initialization") ||
     !check(esp_lcd_panel_invert_color(g_panel, true), "ST7789 color inversion") ||
     !check(esp_lcd_panel_disp_on_off(g_panel, true), "ST7789 display enable")) {
    return false;
  }

  lv_display_set_user_data(g_display, g_panel);
  lv_display_set_flush_cb(g_display, flush_display);
  return true;
}

bool read_touch_packet(uint8_t* data, std::size_t length) {
  Wire.beginTransmission(board::kTouchAddress);
  Wire.write(kTouchDataRegister);
  if(Wire.endTransmission(false) != 0) {
    return false;
  }

  const std::size_t received = Wire.requestFrom(
      static_cast<uint8_t>(board::kTouchAddress), static_cast<uint8_t>(length));
  if(received != length) {
    while(Wire.available() > 0) {
      Wire.read();
    }
    return false;
  }

  for(std::size_t i = 0; i < length; ++i) {
    data[i] = static_cast<uint8_t>(Wire.read());
  }
  return true;
}

void read_touch(lv_indev_t*, lv_indev_data_t* data) {
  uint8_t packet[5] = {};
  if(!read_touch_packet(packet, sizeof(packet)) || packet[0] == 0) {
    data->state = LV_INDEV_STATE_RELEASED;
    return;
  }

  const uint16_t x = static_cast<uint16_t>(((packet[1] & 0x0F) << 8) | packet[2]);
  const uint16_t y = static_cast<uint16_t>(((packet[3] & 0x0F) << 8) | packet[4]);
  data->point.x = std::min<uint16_t>(x, board::kDisplayWidth - 1);
  data->point.y = std::min<uint16_t>(y, board::kDisplayHeight - 1);
  data->state = LV_INDEV_STATE_PRESSED;
}

bool init_touch() {
  pinMode(board::kTouchInterrupt, INPUT);
  if(!Wire.begin(board::kTouchSda, board::kTouchScl, board::kTouchClockHz)) {
    Serial.println("[display] unable to initialize touch I2C bus");
    return false;
  }

  g_touch = lv_indev_create();
  if(g_touch == nullptr) {
    Serial.println("[display] unable to create LVGL touch device");
    return false;
  }
  lv_indev_set_type(g_touch, LV_INDEV_TYPE_POINTER);
  lv_indev_set_display(g_touch, g_display);
  lv_indev_set_read_cb(g_touch, read_touch);
  return true;
}

}  // namespace

void set_backlight(float level) {
  const float clamped = std::max(0.0f, std::min(1.0f, level));
  const uint32_t duty = static_cast<uint32_t>(clamped * kBacklightMaximum + 0.5f);
  ledcWrite(kBacklightChannel, duty);
}

bool init() {
  reset_panel_and_touch();

  pinMode(board::kBacklight, OUTPUT);
  digitalWrite(board::kBacklight, LOW);
  ledcSetup(kBacklightChannel, kBacklightFrequency, kBacklightResolution);
  ledcAttachPin(board::kBacklight, kBacklightChannel);

  lv_init();
  if(!init_display() || !init_touch()) {
    set_backlight(0.0f);
    return false;
  }
  set_backlight(0.5f);
  return true;
}

}  // namespace display
}  // namespace nicemcu
