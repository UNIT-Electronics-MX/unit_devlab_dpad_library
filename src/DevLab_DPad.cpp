/**
 * @file DevLab_DPad.cpp
 * @brief Implementation of the UNIT DevLab D-Pad module driver.
 *
 * @author Jonathan Mejorado
 * @organization UNIT Electronics MX
 */

#include "DevLab_DPad.h"

bool DevLab_DPad::begin() {
    _verified = false;
    _bus.setClock(_clock);
    _busReady = _bus.begin();
    if (!_busReady) return false;
    return (_verified = _ddp.matchesExpectedDevice(_address, &_info));
}

bool DevLab_DPad::begin(uint8_t sdaPin, uint8_t sclPin, uint32_t clock) {
    _verified = false;
    _clock = clock;
    _bus.setClock(_clock);
    _busReady = _bus.begin(sdaPin, sclPin);
    if (!_busReady) return false;
    return (_verified = _ddp.matchesExpectedDevice(_address, &_info));
}

bool DevLab_DPad::beginRecovered(uint8_t sdaPin, uint8_t sclPin, uint32_t timeoutUs, bool restart) {
    _verified = false;
    _bus.setClock(_clock);
    _busReady = _bus.beginRecovered(sdaPin, sclPin, timeoutUs, restart);
    if (!_busReady) return false;
    return (_verified = _ddp.matchesExpectedDevice(_address, &_info));
}

void DevLab_DPad::printInfo(Print &out) const {
    DevLabDDP::printDeviceInfo(out, _address, _info, _ddp.expectedDeviceId());
}

bool DevLab_DPad::readButtons(uint8_t &buttons) {
    if (!_verified) return false;
    int16_t value = _ddp.readButtons(_address);
    if (value < 0) return false;
    buttons = (uint8_t)value;
    return true;
}

bool DevLab_DPad::update() {
    uint8_t buttons;
    if (!readButtons(buttons)) return false;
    _previous = _state;
    _state = buttons;
    return true;
}
