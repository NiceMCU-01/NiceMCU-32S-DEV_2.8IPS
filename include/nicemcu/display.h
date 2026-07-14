#ifndef NICEMCU_DISPLAY_H
#define NICEMCU_DISPLAY_H

#include <cstdint>

namespace nicemcu {
namespace display {

struct TouchDebugInfo {
  uint16_t raw_x = 0;
  uint16_t raw_y = 0;
  bool pressed = false;
  bool read_ok = false;
};

bool init();
void set_backlight(float level);
TouchDebugInfo get_touch_debug_info();

}  // namespace display
}  // namespace nicemcu

#endif  // NICEMCU_DISPLAY_H
