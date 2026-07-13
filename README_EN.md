# NiceMCU 2.8 IPS Factory UI

[中文说明](./README.md)

An ESP32 + LVGL display framework and Factory UI example for the `NiceMCU-32S-DEV_2.8IPS` development board. The project is built with PlatformIO; its configuration is in `platformio.ini`.

## UI Preview

![Factory UI preview](./docs/images/factory-ui.png)

## Features

- Provides a Factory UI example built with LVGL
- Includes Home, IO, WiFi, and SD page entries
- Supports horizontal paging with page indicators
- Includes ST7789 display and CST816D touch integration

## Hardware and Software Stack

- Development board: NiceMCU-32S-DEV_2.8IPS
- MCU: ESP32
- Display: ST7789, 240 × 320
- Touch controller: CST816D
- GUI framework: LVGL 9.5.0
- Development environment: PlatformIO with the Arduino framework

## Project Scope

This project provides a working ESP32 TFT display framework and an extensible Factory UI prototype.

Its current focus is the display and touch pipeline, page structure, and theme styling. The `IO / WiFi / SD` pages do not yet include complete application logic.

It can serve as a reference for a factory-default UI or display framework, and as a UI foundation for further feature development.

## UI Overview

The `Factory UI` uses a three-part layout:

- Top: a blue title bar labeled `Factory UI`
- Center: white rounded information cards with horizontal paging
- Bottom: four fixed entry buttons for `Home / IO / WiFi / SD`

All four entries reuse the same horizontally paged card framework. Switching entries returns to the first page, and dots at the bottom of each card indicate the active page.

## Notes and Limitations

The fields on the `Home / IO / WiFi / SD` pages currently contain demo or placeholder data. They are used to validate:

- page layout
- paging interactions
- field density, truncation, and spacing
- overall display framework stability

They do **not** represent live hardware state, and real-time board data has **not** been integrated. Replace them with actual status or test results as needed.

## Project Structure

- `src/main.cpp`: application entry, LVGL tick handling, and UI startup
- `src/nicemcu_display.cpp`: ST7789, CST816D, LVGL display, and touch integration
- `src/factory_ui.cpp`: Factory UI structure, paging logic, and tab switching
- `include/factory_ui/`: UI headers for the app, styles, and theme layers
- `include/nicemcu/`: NiceMCU driver API and target hardware parameters

## Display and Touch Driver

The project provides a standalone minimal display and touch driver implementation for this board. It can be reproduced after clearing `.pio`, reinstalling dependencies, or moving to a different development environment.

- Display and touch driver: `src/nicemcu_display.cpp`
- Hardware configuration: `include/nicemcu/board_config.h`
- UI framework: PlatformIO-managed `lvgl/lvgl`

## License

This project is licensed under the [MIT License](./LICENSE).
