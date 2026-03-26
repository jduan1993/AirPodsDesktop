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
#### v0.5.2
* 🔗 **UI 強一致性架構 (雙訊號解耦)**：
  * **狀態分離**：徹底分離了「連線存在」與「數據可用性」。只要 Windows 系統顯示已連線，UI 將始終保持「已連線」彩色狀態，不再因 BLE 廣播瞬間丟失而導致圖示變灰。
  * **消除 UI 閃爍**：解決了在弱訊號或 Watchdog 後台自愈時，工作列圖示在連線/斷開狀態間頻繁跳變的問題。
* 🛡️ **訊號魯棒性提升**：
  * **超時邏輯優化**：將數據過期判定（`_lostTimer`）從 20 秒延長至 **30 秒**，大幅提升了對 AirPods 深度休眠和環境電磁干擾的容忍度。
  * **受控清理機制**：僅在系統徹底斷開時才重置 UI 狀態，確保視覺回饋的連續性。
* ⚡ **核心 Bug 修復**：
  * **修復啟動崩潰**：解決了初始化階段因回呼競態導致的空指標崩潰（Race Condition），確保程式在各種環境下均能穩定啟動。
  * **生命週期加固**：在所有非同步路徑中增加了主視窗實例的防禦性判空檢查。
* 🐕 **靜默自癒哨兵**：
  * 優化了 BLE Watchdog 的工作流，使其在後台靜默修復數據流，不再觸發干擾使用者的 UI 狀態變更。

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
