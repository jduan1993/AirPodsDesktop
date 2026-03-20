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
#### v0.5.1 (目前版本)
* 🛡️ **智能媒體邏輯 (多訊號融合)**：
  * **回盒即刻暫停**：融合物理位置訊號 (`isBothPodsInCase`) 與電學充電狀態，實現耳機放回即刻暫停，徹底規避長防抖帶來的延遲感。
  * **長效確認防抖**：非充電狀態下的離耳確認維持在 10 秒，有效規避訊號瞬間遮擋導致的誤觸發。
* 🐕 **具備自癒能力的 BLE Watchdog**：
  * **假連線自動偵測**：自動識別「系統顯示已連線但無電量數據流」的假連線狀態。
  * **分級恢復策略**：實現全自動恢復管線（一級：內部掃描器重啟；二級：診斷級錯誤日誌提示）。
  * **連線寬限期**：新增 15 秒連線初始化保護期，為 Windows 藍牙棧握手留出充足時間。
  * **雙重確認抗抖動**：需連續兩次採樣週期無數據才觸發恢復，確保在弱訊號環境下的穩定性。
* 🚀 **工業級併發與安全**：
  * **核心標誌原子化**：連線、自動偵測等關鍵狀態全面升級為 `std::atomic`。
  * **生命週期加固**：使用 **`QTimer::singleShot`** 替代不安全的 `detach` 執行緒處理非同步重連，徹底規避懸空指標導致的崩潰風險。
  * **無鎖路徑優化**：實現了高頻廣播包處理的無鎖快速路徑，顯著降低 UI 執行緒競爭。
* 🔒 **記憶體與數據一致性**：
  * **跨執行緒快照機制**：解決了 `QString` 在併發環境下的讀寫隱患。
  * **狀態平滑演進**：新狀態始終繼承自上一個有效快照，徹底消除了 UI 在訊號中斷時的閃爍問題。
* 🛠️ **穩定性加固**：
  * **完備生命週期管理**：增加了顯式析構邏輯，確保所有非同步定時器在應用退出時安全停止。

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
