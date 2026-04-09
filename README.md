<h1 align="center">
    <a href="https://github.com/jduan1993/AirPodsDesktop"><img src="/Source/Resource/Image/Icon.svg" alt="Icon" width="128"></a>
    <br>
    AirPodsDesktop
</h1>
<p align="center">AirPods desktop user experience enhancement program</p>
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
<p align="center">🌎 English | 🌏 <a href="/README-CN.md">简体中文</a> | 🌏 <a href="/README-TW.md">繁體中文</a></p>

## 🔍 Preview
![Preview Image](/Assets/Preview.gif)

## ✨ Features
#### v0.5.3
* 🚀 **Enhanced Device Discovery (Explorer-Grade Logic)**:
  * **Dual-Watcher Architecture**: Integrated `DeviceWatcher` alongside `AdvertisementWatcher` to monitor both BLE packets and system-level device endpoints simultaneously.
  * **Connection-Triggered Rescan**: Automatically initiates a full scan upon successful connection to ensure immediate synchronization of system properties.
  * **Extended Advertisement Support**: Enabled support for Bluetooth 5.0+ Extended Advertisements, improving detection for newer AirPods models.
  * **Active Scanning**: Standardized on Active Scanning mode to retrieve more complete device metadata.

## 🛠️ Build
See [Build Instructions](/Docs/Build.md).

## 🤝 Contributing
*AirPodsDesktop* is an open-source project, and you can contribute by:
* [Opening an issue](https://github.com/jduan1993/AirPodsDesktop/issues/new/choose) to report bugs or suggest new features.
* [Submitting a PR](https://github.com/jduan1993/AirPodsDesktop/compare) to fix known bugs or try items in the TODO list.
* [Translating to other languages](/CONTRIBUTING.md#-translation-guide) or [improving existing translations](/CONTRIBUTING.md#-translation-guide).

## 💎 Third Party
* [Qt 5.15.2](https://www.qt.io/download-qt-installer) ([LGPLv3 License](https://doc.qt.io/qt-5/lgpl.html))
* [spdlog](https://github.com/gabime/spdlog) ([MIT License](https://github.com/gabime/spdlog/blob/v1.x/LICENSE))
* [cxxopts](https://github.com/jarro2783/cxxopts) ([MIT License](https://github.com/jarro2783/cxxopts/blob/master/LICENSE))
* [cpr](https://github.com/whoshuu/cpr) ([MIT License](https://github.com/whoshuu/cpr/blob/master/LICENSE))
* [json](https://github.com/nlohmann/json) ([MIT License](https://github.com/nlohmann/json/blob/develop/LICENSE.MIT))
* [SingleApplication](https://github.com/itay-grudev/SingleApplication) ([MIT License](https://github.com/itay-grudev/SingleApplication/blob/master/LICENSE))
* [pfr](https://github.com/boostorg/pfr) ([BSL-1.0 License](https://github.com/boostorg/pfr/blob/develop/LICENSE_1_0.txt))
* [magic_enum](https://github.com/Neargye/magic_enum) ([MIT License](https://github.com/Neargye/magic_enum/blob/master/LICENSE))
* [stacktrace](https://github.com/boostorg/stacktrace) ([BSL-1.0 License](https://www.boost.org/LICENSE_1_0.txt))

## 🍺 Credits
* [SpriteOvO](https://github.com/SpriteOvO/AirPodsDesktop)
* [OpenPods](https://github.com/adolfintel/OpenPods)
* [Discontinued Privacy: Personal Data Leaks in Apple Bluetooth-Low-Energy Continuity Protocols](https://hal.inria.fr/hal-02394619/document)
* [MagicPods](https://magicpods.app/)
