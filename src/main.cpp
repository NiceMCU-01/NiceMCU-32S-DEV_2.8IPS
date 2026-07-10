#include <Arduino.h>
#include <lvgl.h>

#include "factory_ui/app.h"
#include "nicemcu/display.h"

auto lv_last_tick = millis();

void setup() {
  Serial.begin(115200);

  if(!nicemcu::display::init()) {
    Serial.println("Display initialization failed");
    return;
  }
  nicemcu::display::set_backlight(1.0f);

  auto display = lv_display_get_default();
  lv_display_set_rotation(display, LV_DISPLAY_ROTATION_0);
  // lv_display_set_rotation(display, LV_DISPLAY_ROTATION_90);
  // lv_display_set_rotation(display, LV_DISPLAY_ROTATION_180);
  // lv_display_set_rotation(display, LV_DISPLAY_ROTATION_270);

  factory_ui::build();
}

void loop() {
  auto now = millis();
  lv_tick_inc(now - lv_last_tick);
  lv_last_tick = now;

  lv_timer_handler();
}
