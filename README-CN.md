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
#### v0.5.3
* 🚀 **增强型设备发现 (Explorer 级逻辑)**：
  * **双监视器架构**：同步集成了 `DeviceWatcher` 与 `AdvertisementWatcher`，同时监听底层 BLE 广播包与系统级设备端点 (AEP)。
  * **连接触发重扫**：在耳机连接成功时自动触发完整扫描，确保系统属性和服务的即时同步。
  * **扩展广告支持**：开启了对蓝牙 5.0+ 扩展广告的支持，提升了对新型号 AirPods 在复杂环境下的发现率。
  * **主动扫描模式**：标准化使用主动扫描 (Active Scanning)，以获取更完整的设备元数据。

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
