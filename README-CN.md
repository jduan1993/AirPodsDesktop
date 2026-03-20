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
#### v0.5.1 (当前版本)
* 🛡️ **智能媒体逻辑 (多信号融合)**：
  * **回盒即刻暂停**：融合物理位置信号 (`isBothPodsInCase`) 与电学充电状态，实现耳机放回即刻暂停，彻底规避长防抖带来的延迟感。
  * **长效确认防抖**：非充电状态下的离耳确认维持在 10 秒，有效规避信号瞬间遮挡导致的误触发。
* 🐕 **具备自愈能力的 BLE Watchdog**：
  * **假连接自动检测**：自动识别“系统显示已连接但无电量数据流”的假连接状态。
  * **分级恢复策略**：实现全自动恢复管线（一级：内部扫描器重启；二级：诊断级错误日志提示）。
  * **连接宽限期**：新增 15 秒连接初始化保护期，为 Windows 蓝牙栈握手留出充足时间。
  * **双重确认抗抖动**：需连续两次采样周期无数据才触发恢复，确保在弱信号环境下的稳定性。
* 🚀 **工业级并发与安全**：
  * **核心标志原子化**：连接、自动检测等关键状态全面升级为 `std::atomic`。
  * **生命周期加固**：使用 **`QTimer::singleShot`** 替代不安全的 `detach` 线程处理异步重连，彻底规避悬空指针导致的崩溃风险。
  * **无锁路径优化**：实现了高频广播包处理的无锁快速路径，显著降低 UI 线程竞争。
* 🔒 **内存与数据一致性**：
  * **跨线程快照机制**：解决了 `QString` 在并发环境下的读写隐患。
  * **状态平滑演进**：新状态始终继承自上一个有效快照，彻底消除了 UI 在信号中断时的闪烁问题。
* 🛠️ **稳定性加固**：
  * **完备生命周期管理**：增加了显式析构逻辑，确保所有异步定时器在应用退出时安全停止。

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
