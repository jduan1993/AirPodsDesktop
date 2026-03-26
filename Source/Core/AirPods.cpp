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

#include "AirPods.h"

#include <mutex>
#include <chrono>
#include <thread>
#include <QVector>
#include <QTimer>
#include <QMetaObject>
#include <QMessageBox>

#include <Config.h>
#include "Bluetooth.h"
#include "GlobalMedia.h"
#include "../Helper.h"
#include "../Logger.h"
#include "../Assert.h"
#include "../Application.h"
#include "../Gui/MainWindow.h"

using namespace Core;
using namespace std::chrono_literals;

namespace Core::AirPods {
namespace Details {

//
// Advertisement
//

bool Advertisement::IsDesiredAdv(const Bluetooth::AdvertisementWatcher::ReceivedData &data)
{
    auto iter = data.manufacturerDataMap.find(AppleCP::VendorId);
    if (iter == data.manufacturerDataMap.end()) {
        return false;
    }

    const auto &manufacturerData = (*iter).second;
    if (!AppleCP::AirPods::IsValid(manufacturerData)) {
        return false;
    }

    return true;
}

Advertisement::Advertisement(const Bluetooth::AdvertisementWatcher::ReceivedData &data)
{
    APD_ASSERT(IsDesiredAdv(data));
    _data = data;

    auto protocol = AppleCP::As<AppleCP::AirPods>(GetMfrData());
    APD_ASSERT(protocol.has_value());
    _protocol = std::move(protocol.value());

    // Store state
    _state.model = _protocol.GetModel();
    _state.side = _protocol.GetBroadcastedSide();

    _state.pods.left.battery = _protocol.GetLeftBattery();
    _state.pods.left.isCharging = _protocol.IsLeftCharging();
    _state.pods.left.isInEar = _protocol.IsLeftInEar();

    _state.pods.right.battery = _protocol.GetRightBattery();
    _state.pods.right.isCharging = _protocol.IsRightCharging();
    _state.pods.right.isInEar = _protocol.IsRightInEar();

    _state.caseBox.battery = _protocol.GetCaseBattery();
    _state.caseBox.isCharging = _protocol.IsCaseCharging();

    _state.caseBox.isBothPodsInCase = _protocol.IsBothPodsInCase();
    _state.caseBox.isLidOpened = _protocol.IsLidOpened();

    if (_state.pods.left.battery.Available()) {
        _state.pods.left.battery = _state.pods.left.battery.Value() * 10;
    }
    if (_state.pods.right.battery.Available()) {
        _state.pods.right.battery = _state.pods.right.battery.Value() * 10;
    }
    if (_state.caseBox.battery.Available()) {
        _state.caseBox.battery = _state.caseBox.battery.Value() * 10;
    }
}

int16_t Advertisement::GetRssi() const
{
    return _data.rssi;
}

const auto &Advertisement::GetTimestamp() const
{
    return _data.timestamp;
}

auto Advertisement::GetAddress() const -> AddressType
{
    return _data.address;
}

std::vector<uint8_t> Advertisement::GetDesensitizedData() const
{
    auto desensitizedData = _protocol.Desensitize();

    std::vector<uint8_t> result(sizeof(desensitizedData), 0);
    std::memcpy(result.data(), &desensitizedData, sizeof(desensitizedData));
    return result;
}

auto Advertisement::GetAdvState() const -> const AdvState &
{
    return _state;
}

const std::vector<uint8_t> &Advertisement::GetMfrData() const
{
    auto iter = _data.manufacturerDataMap.find(AppleCP::VendorId);
    APD_ASSERT(iter != _data.manufacturerDataMap.end());

    return (*iter).second;
}

//
// StateManager
//

StateManager::StateManager()
{
    _lostTimer.Start(20s, [this] {
        std::lock_guard<std::mutex> lock{_mutex};
        DoLost();
    });

    _stateResetTimer.left.Start(20s, [this] {
        std::lock_guard<std::mutex> lock{_mutex};
        DoStateReset(Side::Left);
    });

    _stateResetTimer.right.Start(20s, [this] {
        std::lock_guard<std::mutex> lock{_mutex};
        DoStateReset(Side::Right);
    });
}

StateManager::~StateManager()
{
    _lostTimer.Stop();
    _stateResetTimer.left.Stop();
    _stateResetTimer.right.Stop();
}

std::optional<State> StateManager::GetCurrentState() const
{
    std::lock_guard<std::mutex> lock{_mutex};
    return _cachedState;
}

auto StateManager::OnAdvReceived(Advertisement adv) -> std::optional<UpdateEvent>
{
    std::lock_guard<std::mutex> lock{_mutex};

    if (!IsPossibleDesiredAdv(adv)) {
        return std::nullopt;
    }

    UpdateAdv(std::move(adv));
    return UpdateState();
}

void StateManager::Disconnect()
{
    std::lock_guard<std::mutex> lock{_mutex};
    LOG(Info, "StateManager: Explicit Disconnect.");
    ResetAll();
}

bool StateManager::IsPossibleDesiredAdv(const Advertisement &adv) const
{
    const auto &advState = adv.GetAdvState();
    auto &lastAdv = advState.side == Side::Left ? _adv.left : _adv.right;

    if (lastAdv.has_value() && lastAdv->first.GetAddress() != adv.GetAddress()) {
        const auto &lastAdvState = lastAdv->first.GetAdvState();
        if (advState.model != lastAdvState.model)
            return false;

        Battery::ValueType leftDiff = 0, rightDiff = 0, caseDiff = 0;
        using SignedVal = std::make_signed_t<Battery::ValueType>;

        if (advState.pods.left.battery.Available() && lastAdvState.pods.left.battery.Available())
            leftDiff = std::abs(
                static_cast<SignedVal>(advState.pods.left.battery.Value()) -
                static_cast<SignedVal>(lastAdvState.pods.left.battery.Value()));
        if (advState.pods.right.battery.Available() && lastAdvState.pods.right.battery.Available())
            rightDiff = std::abs(
                static_cast<SignedVal>(advState.pods.right.battery.Value()) -
                static_cast<SignedVal>(lastAdvState.pods.right.battery.Value()));
        if (advState.caseBox.battery.Available() && lastAdvState.caseBox.battery.Available())
            caseDiff = std::abs(
                static_cast<SignedVal>(advState.caseBox.battery.Value()) -
                static_cast<SignedVal>(lastAdvState.caseBox.battery.Value()));

        if (leftDiff > 5 || rightDiff > 5 || caseDiff > 5)
            return false;
    }
    return true;
}

void StateManager::UpdateAdv(Advertisement adv)
{
    _lostTimer.Reset();
    const auto &advState = adv.GetAdvState();
    if (advState.side == Side::Left) {
        _stateResetTimer.left.Reset();
        _adv.left = std::make_pair(std::move(adv), Clock::now());
    }
    else {
        _stateResetTimer.right.Reset();
        _adv.right = std::make_pair(std::move(adv), Clock::now());
    }
}

auto StateManager::UpdateState() -> std::optional<UpdateEvent>
{
    auto pickSide = [&](auto available_cond_fn) -> std::optional<Advertisement::AdvState> {
        bool leftAvailable =
            _adv.left.has_value() && available_cond_fn(_adv.left->first.GetAdvState());
        bool rightAvailable =
            _adv.right.has_value() && available_cond_fn(_adv.right->first.GetAdvState());

        if (leftAvailable && rightAvailable) {
            return (_adv.left->second > _adv.right->second) ? _adv.left->first.GetAdvState()
                                                            : _adv.right->first.GetAdvState();
        }
        if (leftAvailable)
            return _adv.left->first.GetAdvState();
        if (rightAvailable)
            return _adv.right->first.GetAdvState();
        return std::nullopt;
    };

    // Maintain state continuity by inheriting from cache
    State newState = _cachedState.value_or(State{});

    if (auto s = pickSide([](const auto &s) { return s.model != Model::Unknown; }))
        newState.model = s->model;
    if (auto s = pickSide([](const auto &s) { return s.pods.left.battery.Available(); }))
        newState.pods.left = s->pods.left;
    if (auto s = pickSide([](const auto &s) { return s.pods.right.battery.Available(); }))
        newState.pods.right = s->pods.right;
    if (auto s = pickSide([](const auto &s) { return s.caseBox.battery.Available(); }))
        newState.caseBox = s->caseBox;

    if (newState == _cachedState)
        return std::nullopt;
    auto oldState = std::move(_cachedState);
    _cachedState = std::move(newState);

    return UpdateEvent{.oldState = std::move(oldState), .newState = _cachedState.value()};
}

void StateManager::ResetAll()
{
    _adv.left.reset();
    _adv.right.reset();
    _cachedState.reset();
}

void StateManager::DoLost()
{
    if (_cachedState.has_value()) {
        LOG(Info, "StateManager: Data stream lost (20s), clearing state.");
        _adv.left.reset();
        _adv.right.reset();
        auto event = UpdateState();
        if (event && _cbStateChanged)
            _cbStateChanged(*event);
    }
}

void StateManager::DoStateReset(Side side)
{
    auto &adv = side == Side::Left ? _adv.left : _adv.right;
    if (adv.has_value()) {
        LOG(Info, "StateManager: Side {} reset due to timeout.", Helper::ToString(side));
        adv.reset();
        auto event = UpdateState();
        if (event && _cbStateChanged)
            _cbStateChanged(*event);
    }
}
} // namespace Details

//
// Manager
//

Manager::Manager()
{
    _stateMgr.SetCallback([this](auto event) { OnStateChanged(std::move(event)); });
    _adWatcher.CbReceived() +=
        [this](auto &&...args) { OnAdvertisementReceived(std::forward<decltype(args)>(args)...); };
    _adWatcher.CbStateChanged() += [this](auto &&...args) {
        std::lock_guard<std::mutex> lock{_mutex};
        OnAdvWatcherStateChanged(std::forward<decltype(args)>(args)...);
    };

    // BLE Watchdog: Check health every 5s
    _bleWatchdogTimer.Start(5s, [this] { CheckBleHealth(); });
}

Manager::~Manager()
{
    _bleWatchdogTimer.Stop();
    _inEarDebounceTimer.Stop();
}

void Manager::StartScanner()
{
    if (!_adWatcher.Start()) {
        LOG(Warn, "Bluetooth AdvWatcher start failed.");
    }
    else {
        LOG(Info, "Bluetooth AdvWatcher started.");
    }
}

void Manager::StopScanner()
{
    if (!_adWatcher.Stop()) {
        LOG(Warn, "Bluetooth AdvWatcher stop failed.");
    }
    else {
        LOG(Info, "Bluetooth AdvWatcher stopped.");
    }
}

void Manager::OnAutomaticEarDetectionChanged(bool enable)
{
    _automaticEarDetection = enable;
}

void Manager::OnBoundDeviceAddressChanged(uint64_t address)
{
    std::unique_lock<std::mutex> lock{_mutex};
    _boundDevice.reset();
    _deviceConnected = false;
    this->_lastReportedInEar = false;
    _stateMgr.Disconnect();

    // Reset Watchdog on device change
    auto now = std::chrono::steady_clock::now();
    _lastAdvTime = now;
    _connectedAt = now;
    _bleHealthy = true;
    _recoveryStage = 0;
    _bleMissCount = 0;

    if (address == 0) {
        LOG(Info, "Device address cleared (unbound).");
        return;
    }

    auto optDevice = Bluetooth::DeviceManager::FindDevice(address);
    if (!optDevice.has_value()) {
        LOG(Error, "Device binding failed: Bluetooth address {} not found in system paired list.",
            address);
        QMessageBox::warning(nullptr, Config::ProgramName, QObject::tr("No paired device found."));
        return;
    }

    _boundDevice = std::move(optDevice);
    _deviceName = QString::fromStdString(_boundDevice->GetName());
    _boundDevice->CbConnectionStatusChanged() += [this](auto &&...args) {
        std::lock_guard<std::mutex> lock{_mutex};
        OnBoundDeviceConnectionStateChanged(std::forward<decltype(args)>(args)...);
    };
    LOG(Info, "Successfully bound to device: '{}' ({})", _deviceName.toStdString(), address);
    OnBoundDeviceConnectionStateChanged(_boundDevice->GetConnectionState());
}

void Manager::OnBoundDeviceConnectionStateChanged(Bluetooth::DeviceState state)
{
    bool newConnected = (state == Bluetooth::DeviceState::Connected);
    if (_deviceConnected != newConnected) {
        _deviceConnected = newConnected;
        LOG(Info, "System Connection changed: Connected={}", bool(_deviceConnected));
        
        // Reset Watchdog state and record connection time
        auto now = std::chrono::steady_clock::now();
        _recoveryStage = 0;
        _bleHealthy = true;
        _lastAdvTime = now;
        _connectedAt = now;
        _bleMissCount = 0;

        if (!_deviceConnected) {
            this->_lastReportedInEar = false;
            _stateMgr.Disconnect();
            if (ApdApp->GetMainWindow()) {
                ApdApp->GetMainWindow()->DisconnectSafely();
            }
        }
        else if (_boundDevice.has_value()) {
            State forceState;
            forceState.model = AppleCP::AirPods::GetModel(_boundDevice->GetProductId());
            forceState.displayName = _deviceName;
            if (ApdApp->GetMainWindow()) {
                ApdApp->GetMainWindow()->UpdateStateSafely(forceState);
            }
        }
    }
}

void Manager::LogStateChanges(const std::optional<State> &oldState, const State &newState)
{
    auto bVal = [](const Battery &b) { return b.Available() ? std::to_string(b.Value()) : "N/A"; };

    if (!oldState.has_value()) {
        LOG(Info,
            "[Initial State] Lid:{}, BothInCase:{}, L:(B:{}%, C:{}, E:{}), R:(B:{}%, C:{}, E:{}), "
            "Case:(B:{}%, C:{})",
            newState.caseBox.isLidOpened ? "Open" : "Closed",
            newState.caseBox.isBothPodsInCase ? "Yes" : "No", bVal(newState.pods.left.battery),
            newState.pods.left.isCharging, newState.pods.left.isInEar,
            bVal(newState.pods.right.battery), newState.pods.right.isCharging,
            newState.pods.right.isInEar, bVal(newState.caseBox.battery),
            newState.caseBox.isCharging);
        return;
    }

    const auto &os = oldState.value();
    const auto &ns = newState;

    if (os.caseBox.isLidOpened != ns.caseBox.isLidOpened)
        LOG(Info, "[Lid] {}", ns.caseBox.isLidOpened ? "Opened" : "Closed");

    if (os.caseBox.isBothPodsInCase != ns.caseBox.isBothPodsInCase)
        LOG(Info, "[Case] Pods are now: {}",
            ns.caseBox.isBothPodsInCase ? "Both inside case" : "Removed from case");

    if (os.pods.left.isInEar != ns.pods.left.isInEar)
        LOG(Info, "[In-Ear] Left Pod: {}", ns.pods.left.isInEar ? "Worn" : "Removed");
    if (os.pods.right.isInEar != ns.pods.right.isInEar)
        LOG(Info, "[In-Ear] Right Pod: {}", ns.pods.right.isInEar ? "Worn" : "Removed");

    if (os.pods.left.battery != ns.pods.left.battery ||
        os.pods.left.isCharging != ns.pods.left.isCharging)
        LOG(Info, "[Power] Left Pod: {}% ({})", bVal(ns.pods.left.battery),
            ns.pods.left.isCharging ? "Charging" : "Discharging");
    if (os.pods.right.battery != ns.pods.right.battery ||
        os.pods.right.isCharging != ns.pods.right.isCharging)
        LOG(Info, "[Power] Right Pod: {}% ({})", bVal(ns.pods.right.battery),
            ns.pods.right.isCharging ? "Charging" : "Discharging");
    if (os.caseBox.battery != ns.caseBox.battery || os.caseBox.isCharging != ns.caseBox.isCharging)
        LOG(Info, "[Power] Case: {}% ({})", bVal(ns.caseBox.battery),
            ns.caseBox.isCharging ? "Charging" : "Discharging");
}

void Manager::OnStateChanged(Details::StateManager::UpdateEvent updateEvent)
{
    if (!_deviceConnected)
        return;

    LogStateChanges(updateEvent.oldState, updateEvent.newState);

    auto &newState = updateEvent.newState;
    QString deviceNameSnapshot;
    {
        std::lock_guard<std::mutex> lock{_mutex};
        deviceNameSnapshot = _deviceName;
    }

    if (!deviceNameSnapshot.isEmpty()) {
        deviceNameSnapshot.remove(" - Find My");
        newState.displayName = deviceNameSnapshot;
    }
    else {
        newState.displayName = Helper::ToString(newState.model);
    }

    if (ApdApp->GetMainWindow()) {
        ApdApp->GetMainWindow()->UpdateStateSafely(newState);
    }

    bool newLidOpened = newState.caseBox.isLidOpened && newState.caseBox.isBothPodsInCase;
    if (!updateEvent.oldState.has_value() ||
        (updateEvent.oldState->caseBox.isLidOpened != newState.caseBox.isLidOpened))
        OnLidOpened(newLidOpened);

    if (!updateEvent.oldState.has_value()) {
        bool newAnyInEar = newState.pods.left.isInEar || newState.pods.right.isInEar;
        this->_lastReportedInEar = !newAnyInEar;
        LOG(Debug, "System Consistency: State recovered. AnyInEar={}, SyncFlag={}", newAnyInEar,
            bool(this->_lastReportedInEar));
    }

    OnBothInEar(newState);
}

void Manager::OnLidOpened(bool opened)
{
    if (ApdApp->GetMainWindow()) {
        auto &mw = ApdApp->GetMainWindow();
        if (opened)
            mw->ShowSafely();
        else
            mw->HideSafely();
    }
}

void Manager::OnBothInEar(const State &state)
{
    bool isAnyInEar = state.pods.left.isInEar || state.pods.right.isInEar;

    if (isAnyInEar) {
        _inEarDebounceTimer.Stop();
        if (!this->_lastReportedInEar.exchange(true)) {
            if (_automaticEarDetection) {
                LOG(Info, "[Action] Trigger Play - Any pod in ear.");
                Core::GlobalMedia::Play();
            }
        }
    }
    else {
        // Optimization: Multi-signal fusion for immediate pause
        // Level 1 & 2: Strong signals for immediate pause
        bool bothInCase = state.caseBox.isBothPodsInCase;
        bool bothCharging = state.pods.left.isCharging && state.pods.right.isCharging;

        if (bothInCase || bothCharging) {
            if (_lastReportedInEar.exchange(false)) {
                _inEarDebounceTimer.Stop();
                if (_automaticEarDetection) {
                    LOG(Info, "[Action] Immediate Pause - Pods returned to case (Fusion: InCase={}, Charging={}).", bothInCase, bothCharging);
                    Core::GlobalMedia::Pause();
                }
            }
            return;
        }

        // Level 3: Weak/Jittery signal, use 10s debounce
        if (this->_lastReportedInEar) {
            _inEarDebounceTimer.Start(10s, [this] {
                if (this->_lastReportedInEar.exchange(false)) {
                    if (_automaticEarDetection) {
                        LOG(Info, "[Action] Trigger Pause - All pods out (10s confirmed).");
                        Core::GlobalMedia::Pause();
                    }
                }
            });
        }
    }
}

void Manager::CheckBleHealth()
{
    if (!_deviceConnected)
        return;

    auto now = std::chrono::steady_clock::now();
    
    // Grace period: Skip health check for the first 15s after connection
    if (now - _connectedAt < 15s)
        return;

    auto last = _lastAdvTime.load();

    // 10s Threshold for "No BLE Data" while connected
    if (now - last > 10s) {
        _bleMissCount++;
        if (_bleHealthy) {
            LOG(Warn, "BLE Watchdog: No data for 10s (Miss count: {}).", (int)_bleMissCount);
            _bleHealthy = false;
        }
        
        // Require 2 consecutive misses (effectively 15-20s of no data) before recovery
        if (_bleMissCount >= 2) {
            TryRecover();
        }
    } else {
        _bleMissCount = 0;
    }
}

void Manager::TryRecover()
{
    // Stage 1: Restart BLE Scanner (Internal)
    if (_recoveryStage == 0) {
        LOG(Warn, "[Recovery Stage 1] Restarting BLE scanner...");
        StopScanner();
        StartScanner();
        _recoveryStage = 1;
        return;
    }

    if (_recoveryStage >= 1) {
        // Note: Stage 2 (System Reconnection) is not directly supported by current Device API.
        // Stage 3: Give up to prevent flapping, wait for manual action or next valid Adv
        static auto lastErrorLog = std::chrono::steady_clock::now();
        if (std::chrono::steady_clock::now() - lastErrorLog > 1min) {
            LOG(Error, "[Recovery] BLE remains dead after internal scanner restart. Please check your Bluetooth driver.");
            lastErrorLog = std::chrono::steady_clock::now();
        }
    }
}

bool Manager::OnAdvertisementReceived(const Bluetooth::AdvertisementWatcher::ReceivedData &data)
{
    if (!_deviceConnected)
        return false;

    // Reset Watchdog on any valid data receipt
    auto now = std::chrono::steady_clock::now();
    _lastAdvTime = now;
    
    if (!_bleHealthy) {
        LOG(Info, "[Recovery] BLE data stream restored (Advertisement received).");
        _bleHealthy = true;
    }
    
    _recoveryStage = 0;
    _bleMissCount = 0;

    uint32_t boundProductId = 0;
    {
        std::lock_guard<std::mutex> lock{_mutex};
        if (_boundDevice.has_value())
            boundProductId = _boundDevice->GetProductId();
    }

    if (!Details::Advertisement::IsDesiredAdv(data))
        return false;
    Details::Advertisement adv{data};

    if (boundProductId != 0) {
        Model boundModel = AppleCP::AirPods::GetModel(boundProductId);
        if (boundModel != Model::Unknown && adv.GetAdvState().model != boundModel)
            return false;
    }

    auto event = _stateMgr.OnAdvReceived(std::move(adv));
    if (event) {
        OnStateChanged(std::move(*event));
    }
    return true;
}

void Manager::OnAdvWatcherStateChanged(
    Bluetooth::AdvertisementWatcher::State state, const std::optional<std::string> &error)
{
    if (state == Core::Bluetooth::AdvertisementWatcher::State::Started) {
        if (ApdApp->GetMainWindow()) {
            ApdApp->GetMainWindow()->AvailableSafely();
        }
    }
    else if (state == Core::Bluetooth::AdvertisementWatcher::State::Stopped) {
        if (ApdApp->GetMainWindow()) {
            ApdApp->GetMainWindow()->UnavailableSafely();
        }
        if (error.has_value()) {
            LOG(Error, "Bluetooth Watcher stopped: {}. Throttling restart...", *error);

            auto now = std::chrono::steady_clock::now();
            if (now - _lastScannerRestart > 5s) {
                _lastScannerRestart = now;
                QMetaObject::invokeMethod(ApdApp, [this] { StartScanner(); }, Qt::QueuedConnection);
            }
        }
    }
}

std::vector<Bluetooth::Device> GetDevices()
{
    auto devices = Bluetooth::DeviceManager::GetDevicesByState(Bluetooth::DeviceState::Paired);
    devices.erase(
        std::remove_if(
            devices.begin(), devices.end(),
            [](const auto &d) {
                return d.GetVendorId() != AppleCP::VendorId ||
                       AppleCP::AirPods::GetModel(d.GetProductId()) == AirPods::Model::Unknown;
            }),
        devices.end());
    return devices;
}

} // namespace Core::AirPods
