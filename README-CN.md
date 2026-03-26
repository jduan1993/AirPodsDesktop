<h1 align="center">
    <a href="https://github.com/jduan1993/AirPodsDesktop"><img src="/Source/Resource/Image/Icon.svg" alt="Icon" width="128"></a>
    <br>
    AirPodsDesktop
</h1>
<p align="center">AirPods 桌面用户体验增强程序</p>
<p align="center">
    <a href="https://github.com/jduan1993/AirPodsDesktop/actions/workflows/windows.yml">
        <img src="https://github.com/jduan1993/AirPodsDesktop/actions/workflows/windows.yml/badge.svg"/>
    </a>
    <a href="https://github.com/jduan1993/AirPodsDesktop/releases">
        <img src="https://img.shields.io/github/v/release/jduan1993/AirPodsDesktop?include_prereleases"/>
    </a>
    <a href="https://github.com/jduan1993/AirPodsDesktop/releases">
        <img src="https://img.shields.io/github/downloads/jduan1993/AirPodsDesktop/total.svg"/>
    </a>
    <a href="https://github.com/jduan1993/AirPodsDesktop/compare">
        <img src="https://img.shields.io/badge/PRs-welcome-brightgreen.svg"/>
    </a>
    <a href="/LICENSE">
        <img src="https://img.shields.io/badge/license-GPLv3-yellow.svg"/>
    </a>
</p>
<p align="center">🌎 <a href="/README.md">English</a> | 🌏 简体中文 | 🌏 <a href="/README-TW.md">繁體中文</a></p>

## 🔍 预览
![Preview Image](/Assets/Preview.gif)

## ✨ 特性
#### v0.5.2
* 🔗 **UI 强一致性架构 (双信号解耦)**：
  * **状态分离**：彻底分离了“连接存在”与“数据可用性”。只要 Windows 系统显示已连接，UI 将始终保持“已连接”彩色状态，不再因 BLE 广播瞬间丢失而导致图标变灰。
  * **消除 UI 闪烁**：解决了在弱信号或 Watchdog 后台自愈时，托盘图标在连接/断开状态间频繁跳变的问题。
* 🛡️ **信号鲁棒性提升**：
  * **超时逻辑优化**：将数据过期判定（`_lostTimer`）从 20 秒延长至 **30 秒**，大幅提升了对 AirPods 深度休眠和环境电磁干扰的容忍度。
  * **受控清理机制**：仅在系统彻底断开时才重置 UI 状态，确保视觉反馈的连续性。
* ⚡ **核心 Bug 修复**：
  * **修复启动崩溃**：解决了初始化阶段因回调竞态导致的空指针崩溃（Race Condition），确保程序在各种环境下均能稳定启动。
  * **生命周期加固**：在所有异步路径中增加了主窗口实例的防御性判空检查。
* 🐕 **静默自愈哨兵**：
  * 优化了 BLE Watchdog 的工作流，使其在后台静默修复数据流，不再触发干扰用户的 UI 状态变更。

## 🛠️ 构建
查看 [构建说明](/Docs/Build.md)。

## 🤝 贡献
*AirPodsDesktop* 是一个开源项目，您可以通过以下方式贡献：
* [打开问题](https://github.com/jduan1993/AirPodsDesktop/issues/new/choose) 来报告错误或建议新功能。
* [提交 PR](https://github.com/jduan1993/AirPodsDesktop/compare) 来修复已知 BUG 或尝试 TODO 列表中的事项。
* [翻译到其他语言](/CONTRIBUTING.md#-translation-guide) 或 [改进现有的翻译](/CONTRIBUTING.md#-translation-guide)。

## 💎 第三方
* [Qt 5.15.2](https://www.qt.io/download-qt-installer) ([LGPLv3 License](https://doc.qt.io/qt-5/lgpl.html))
* [spdlog](https://github.com/gabime/spdlog) ([MIT License](https://github.com/gabime/spdlog/blob/v1.x/LICENSE))
* [cxxopts](https://github.com/jarro2783/cxxopts) ([MIT License](https://github.com/jarro2783/cxxopts/blob/master/LICENSE))
* [cpr](https://github.com/whoshuu/cpr) ([MIT License](https://github.com/whoshuu/cpr/blob/master/LICENSE))
* [json](https://github.com/nlohmann/json) ([MIT License](https://github.com/nlohmann/json/blob/develop/LICENSE.MIT))
* [SingleApplication](https://github.com/itay-grudev/SingleApplication) ([MIT License](https://github.com/itay-grudev/SingleApplication/blob/master/LICENSE))
* [pfr](https://github.com/boostorg/pfr) ([BSL-1.0 License](https://github.com/boostorg/pfr/blob/develop/LICENSE_1_0.txt))
* [magic_enum](https://github.com/Neargye/magic_enum) ([MIT License](https://github.com/Neargye/magic_enum/blob/master/LICENSE))
* [stacktrace](https://github.com/boostorg/stacktrace) ([BSL-1.0 License](https://www.boost.org/LICENSE_1_0.txt))

## 🍺 致谢
* [SpriteOvO](https://github.com/SpriteOvO/AirPodsDesktop)
* [OpenPods](https://github.com/adolfintel/OpenPods)
* [Discontinued Privacy: Personal Data Leaks in Apple Bluetooth-Low-Energy Continuity Protocols](https://hal.inria.fr/hal-02394619/document)
* [MagicPods](https://magicpods.app/)
