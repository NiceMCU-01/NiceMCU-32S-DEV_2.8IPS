#ifndef NICEMCU_BOARD_CONFIG_H
#define NICEMCU_BOARD_CONFIG_H

#include <cstddef>
#include <cstdint>

namespace nicemcu {
namespace board {

constexpr uint16_t kDisplayWidth = 240;
constexpr uint16_t kDisplayHeight = 320;

constexpr int kDisplayMosi = 13;
constexpr int kDisplayClock = 14;
constexpr int kDisplayChipSelect = 15;
constexpr int kDisplayDataCommand = 12;
constexpr int kSharedReset = 2;
constexpr int kBacklight = 25;
constexpr uint32_t kDisplayClockHz = 24000000;
constexpr std::size_t kDrawBufferPixels =
    static_cast<std::size_t>(kDisplayWidth) * kDisplayHeight / 4;

constexpr int kTouchSda = 33;
constexpr int kTouchScl = 32;
constexpr int kTouchInterrupt = 36;
constexpr uint8_t kTouchAddress = 0x15;
constexpr uint32_t kTouchClockHz = 400000;

}  // namespace board
}  // namespace nicemcu

#endif  // NICEMCU_BOARD_CONFIG_H
