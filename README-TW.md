<h1 align="center">
    <a href="https://github.com/jduan1993/AirPodsDesktop"><img src="/Source/Resource/Image/Icon.svg" alt="Icon" width="128"></a>
    <br>
    AirPodsDesktop
</h1>
<p align="center">AirPods 桌面使用者體驗增強程式</p>
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
<p align="center">🌎 <a href="/README.md">English</a> | 🌏 <a href="/README-CN.md">简体中文</a> | 🌏 繁體中文</p>

## 🔍 預覽
![Preview Image](/Assets/Preview.gif)

## ✨ 特性
#### v0.5.3
* 🚀 **增強型裝置發現 (Explorer 級邏輯)**：
  * **雙監視器架構**：同步集成了 `DeviceWatcher` 與 `AdvertisementWatcher`，同時監聽底層 BLE 廣播包與系統級裝置端點 (AEP)。
  * **連線觸發重掃**：在耳機連線成功時自動觸發完整掃描，確保系統屬性與服務的即時同步。
  * **擴展廣告支援**：開啟了對藍牙 5.0+ 擴展廣告的支援，提升了對新型號 AirPods 在複雜環境下的發現率。
  * **主動掃描模式**：標準化使用主動掃描 (Active Scanning)，以獲取更完整的裝置元數據。

## 🛠️ 建置
查看 [建置說明](/Docs/Build.md)。

## 🤝 貢獻
*AirPodsDesktop* 是一個開源專案，您可以透過以下方式貢獻：
* [打開問題](https://github.com/jduan1993/AirPodsDesktop/issues/new/choose) 來報告錯誤或建議新功能。
* [提交 PR](https://github.com/jduan1993/AirPodsDesktop/compare) 來修復已知 BUG 或嘗試 TODO 列表中的事項。
* [翻譯到其他語言](/CONTRIBUTING.md#-translation-guide) 或 [改進現有的翻譯](/CONTRIBUTING.md#-translation-guide)。

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

## 🍺 致謝
* [SpriteOvO](https://github.com/SpriteOvO/AirPodsDesktop)
* [OpenPods](https://github.com/adolfintel/OpenPods)
* [Discontinued Privacy: Personal Data Leaks in Apple Bluetooth-Low-Energy Continuity Protocols](https://hal.inria.fr/hal-02394619/document)
* [MagicPods](https://magicpods.app/)
