//
// AirPodsDesktop - AirPods Desktop User Experience Enhancement Program.
// Copyright (C) 2021-2022 SpriteOvO
// Copyright (C) 2026 Hugo Duan
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#pragma once

#include <atomic>
#include <functional>
#include <chrono>

#include "Bluetooth.h"
#include "AppleCP.h"

namespace Core::AirPods {

//
// Structures
//

namespace Details {

struct BasicState {
    Battery battery;
    bool isCharging{false};

    bool operator==(const BasicState &rhs) const = default;
};
} // namespace Details

struct PodState : Details::BasicState {
    bool isInEar{false};

    bool operator==(const PodState &rhs) const = default;
};

struct CaseState : Details::BasicState {
    bool isBothPodsInCase{false};
    bool isLidOpened{false};

    bool operator==(const CaseState &rhs) const = default;
};

struct PodsState {
    PodState left, right;

    bool operator==(const PodsState &rhs) const = default;
};

struct State {
    Model model{Model::Unknown};
    PodsState pods;
    CaseState caseBox;
    QString displayName;

    bool operator==(const State &rhs) const = default;
};

//
// Classes
//

namespace Details {

class Advertisement
{
public:
    using AddressType = decltype(Bluetooth::AdvertisementWatcher::ReceivedData::address);

    struct AdvState : AirPods::State {
        Side side;
    };

    static bool IsDesiredAdv(const Bluetooth::AdvertisementWatcher::ReceivedData &data);

    Advertisement(const Bluetooth::AdvertisementWatcher::ReceivedData &data);

    int16_t GetRssi() const;
    const auto &GetTimestamp() const;
    AddressType GetAddress() const;
    std::vector<uint8_t> GetDesensitizedData() const;
    const AdvState &GetAdvState() const;

private:
    Bluetooth::AdvertisementWatcher::ReceivedData _data;
    AppleCP::AirPods _protocol;
    AdvState _state;

    const std::vector<uint8_t> &GetMfrData() const;
};

class StateManager
{
public:
    struct UpdateEvent {
        std::optional<State> oldState;
        State newState;
    };

    using FnStateChanged = std::function<void(UpdateEvent)>;

    StateManager();
    ~StateManager();

    std::optional<State> GetCurrentState() const;

    std::optional<UpdateEvent> OnAdvReceived(Advertisement adv);
    void Disconnect();

    void SetCallback(FnStateChanged cb)
    {
        _cbStateChanged = std::move(cb);
    }

private:
    using Clock = std::chrono::steady_clock;
    using Timestamp = std::chrono::time_point<Clock>;

    mutable std::mutex _mutex;

    Helper::Timer _lostTimer;
    Helper::Sides<Helper::Timer> _stateResetTimer;
    Helper::Sides<std::optional<std::pair<Advertisement, Timestamp>>> _adv;
    std::optional<State> _cachedState;
    FnStateChanged _cbStateChanged;

    bool IsPossibleDesiredAdv(const Advertisement &adv) const;
    void UpdateAdv(Advertisement adv);
    std::optional<UpdateEvent> UpdateState();
    void ResetAll();

    void DoLost();
    void DoStateReset(Side side);
};
} // namespace Details

class Manager
{
public:
    Manager();
    ~Manager();

    void StartScanner();
    void StopScanner();

    void OnAutomaticEarDetectionChanged(bool enable);
    void OnBoundDeviceAddressChanged(uint64_t address);

private:
    std::mutex _mutex;
    Bluetooth::AdvertisementWatcher _adWatcher;
    Details::StateManager _stateMgr;
    std::optional<Bluetooth::Device> _boundDevice;
    QString _deviceName;

    // Core state flags (Atomic for thread-safety)
    std::atomic<bool> _deviceConnected{false};
    std::atomic<bool> _automaticEarDetection{false};
    std::atomic<bool> _lastReportedInEar{false};

    // BLE Watchdog
    Helper::Timer _bleWatchdogTimer;
    std::atomic<std::chrono::steady_clock::time_point> _lastAdvTime;
    std::chrono::steady_clock::time_point _connectedAt; 
    std::atomic<bool> _bleHealthy{true};
    std::atomic<int> _recoveryStage{0}; // 0: Normal, 1: Scanner Restarted, 2: Device Reconnected
    std::atomic<int> _bleMissCount{0};

    Helper::Timer _inEarDebounceTimer;
    std::chrono::steady_clock::time_point _lastScannerRestart;

    void LogStateChanges(const std::optional<State> &oldState, const State &newState);
    void OnBoundDeviceConnectionStateChanged(Bluetooth::DeviceState state);
    void OnStateChanged(Details::StateManager::UpdateEvent updateEvent);
    void OnLidOpened(bool opened);
    void OnBothInEar(const State &state);
    void CheckBleHealth();
    void TryRecover();
    bool OnAdvertisementReceived(const Bluetooth::AdvertisementWatcher::ReceivedData &data);
    void OnAdvWatcherStateChanged(
        Bluetooth::AdvertisementWatcher::State state, const std::optional<std::string> &optError);
};

std::vector<Core::Bluetooth::Device> GetDevices();

} // namespace Core::AirPods
