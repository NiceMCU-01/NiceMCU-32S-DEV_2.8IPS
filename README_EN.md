# TFT Display Framework Project

中文说明见 [README.md](./README.md)

This repository provides an ESP32 + LVGL display framework for the `NiceMCU-32S-DEV_2.8IPS` TFT development board with an `ST7789 240x320` display.

## Current Status

- The `ST7789 240x320` and `CST816D` paths now use a NiceMCU-specific driver
- The Factory UI and the new driver pass a clean build
- Post-rewrite display, stripe, touch, and backlight checks pass on the target hardware
- The active build environment is `nicemcu-32s-dev`, based on PlatformIO's standard `esp32dev` board
- The verified display fix has been preserved in a reproducible local setup

## Project Positioning

This repository is currently positioned as a stable ESP32 TFT display framework and Factory UI skeleton.

It is not primarily focused on continuing real feature integration for `IO / WiFi / SD`. Instead, the current priorities are:

- keeping the display and touch path stable
- providing a maintainable page structure and theme layer
- preserving a reusable UI skeleton for future content

In other words, this project should be treated as a factory-default UI / display-framework prototype rather than a full product firmware that is actively expanding feature logic.

## Current UI Overview

The current `Factory UI` uses a three-part layout:

- Top: a blue title bar showing `Factory UI`
- Middle: a white rounded information card area with horizontal paging
- Bottom: four fixed entry buttons for `Home / IO / WiFi / SD`

Each bottom tab currently reuses the same horizontally paged card framework. Switching tabs resets to the first page of that tab, and page indicator dots show the active page position.

## Screen Data Notes

The values currently shown on the `Home / IO / WiFi / SD` pages are demo data or placeholder data.

At this stage they are used to validate:

- page layout
- paging interaction
- field density, truncation, and spacing
- overall UI framework stability

They do **not** reflect live runtime board state yet, and real board data has **not** been integrated into the UI. If needed later, those fields can be replaced with actual device status or test results on demand.

## Project Layout

- `src/main.cpp`: application entry, LVGL tick, and UI startup
- `src/nicemcu_display.cpp`: ST7789, CST816D, LVGL display, and touch integration
- `src/factory_ui.cpp`: Factory UI structure, paging logic, and tab switching
- `include/factory_ui/`: UI headers for the app, styles, and theme layers
- `include/nicemcu/`: NiceMCU driver API and target hardware parameters

## Display Driver

The repository contains a minimal display and touch driver independently implemented for the target hardware. Keeping it in-tree makes builds reproducible after clearing `.pio`, reinstalling dependencies, or moving to another development environment.

- Display and touch driver: `src/nicemcu_display.cpp`
- Hardware configuration: `include/nicemcu/board_config.h`
- UI framework: `lvgl/lvgl`, managed by PlatformIO

External dependencies and their license terms are listed in [THIRD_PARTY_NOTICES.md](./THIRD_PARTY_NOTICES.md).

## Build

```powershell
pio run
```

## Upload

```powershell
pio run -t upload
```

## Serial Monitor

```powershell
pio device monitor
```

## Notes

- The current firmware focuses on being a stable display framework, not a full feature-validation firmware
- If new content needs to be shown later, the preferred path is to reuse the existing paged card framework instead of reshaping the whole UI architecture
