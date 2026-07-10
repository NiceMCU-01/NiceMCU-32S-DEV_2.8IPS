# TFT 显示框架项目

English version: [README_EN.md](./README_EN.md)

这是一个面向 `NiceMCU-32S-DEV_2.8IPS` `ST7789 240x320` TFT 开发板的 ESP32 + LVGL 显示框架项目。

## 当前状态

- `ST7789 240x320` 与 `CST816D` 已迁移到 NiceMCU 专用驱动
- 当前 `Factory UI` 和新驱动已通过干净构建
- 驱动重写后的显示、横纹、触摸与背光已通过目标板实机回归测试
- 当前构建环境为 `nicemcu-32s-dev`，基于 PlatformIO 标准 `esp32dev` 板卡
- 已保留并固化本地可复现的显示修复方案

## 项目定位

这个仓库当前定位为一个稳定的 ESP32 TFT 显示框架和 Factory UI 骨架原型。

它的重点不是继续扩展 `IO / WiFi / SD` 的真实功能逻辑，而是：

- 保持当前显示与触摸链路稳定
- 提供可维护的页面结构与主题样式
- 为后续按需填充内容预留统一的展示骨架

因此，这个项目更适合作为“出厂默认 UI / 显示框架原型”，而不是一个正在持续堆叠功能的完整产品固件。

## 当前界面说明

当前 `Factory UI` 采用三段式布局：

- 顶部：蓝色标题栏，显示 `Factory UI`
- 中部：白色圆角信息卡片，支持左右滑动分页
- 底部：固定四个入口按钮 `Home / IO / WiFi / SD`

每个底部 tab 当前都使用同一套横向分页卡片框架。切换 tab 时会回到该 tab 的第一页，卡片底部通过页码圆点指示当前分页位置。

## 屏幕显示数据说明

当前屏幕中展示的 `Home / IO / WiFi / SD` 各类字段，主要是演示数据或占位数据。

这些内容当前用于：

- 验证页面布局
- 验证分页交互
- 验证字段密度、截断和留白
- 验证整体显示框架是否稳定

它们目前**不代表真实运行时硬件状态**，也**尚未接入实时板级数据**。后续如果有明确需求，再按需把对应页面替换为真实状态或实际检测结果。

## 项目结构

- `src/main.cpp`：应用入口、LVGL tick 与 UI 启动
- `src/nicemcu_display.cpp`：ST7789、CST816D、LVGL 显示与触摸适配
- `src/factory_ui.cpp`：Factory UI 主界面结构、分页逻辑与 tab 切换
- `include/factory_ui/`：UI 头文件，包含 app、styles、theme 的接口声明
- `include/nicemcu/`：NiceMCU 驱动接口与目标硬件参数

## 显示驱动

项目包含针对当前硬件独立实现的最小显示与触摸驱动，以保证清理 `.pio`、重装依赖或迁移开发环境后仍可稳定复现。

- 显示与触摸驱动：`src/nicemcu_display.cpp`
- 硬件参数：`include/nicemcu/board_config.h`
- UI 框架：PlatformIO 管理的 `lvgl/lvgl`

外部依赖及许可证记录在 [THIRD_PARTY_NOTICES.md](./THIRD_PARTY_NOTICES.md) 中。

## 构建

```powershell
pio run
```

## 上传

```powershell
pio run -t upload
```

## 串口监视

```powershell
pio device monitor
```

## 备注

- 当前固件的重点是稳定的显示框架，而不是完整功能验证
- 如果后续只需要展示新内容，优先复用现有卡片分页框架，而不是重新调整整体 UI 架构
