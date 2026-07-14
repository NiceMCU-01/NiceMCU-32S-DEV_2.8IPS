#ifndef FACTORY_UI_APP_H
#define FACTORY_UI_APP_H

#include <cstdint>

namespace factory_ui {

void build();
void set_touch_debug(uint16_t raw_x, uint16_t raw_y, bool pressed, bool read_ok);

}  // namespace factory_ui

#endif  // FACTORY_UI_APP_H
