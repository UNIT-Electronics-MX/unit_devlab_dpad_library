#ifndef DEVLAB_DPAD_H
#define DEVLAB_DPAD_H

#pragma once

#include "DevLabDDP.h"
#include "DevLabDDPConsole.h"
#include "DevLab_I2C_Orchestrator.h"

class DevLab_DPad
{
public:
    /* Factory I2C address of the D-Pad module (STARTUP_I2C_ADDRESS). */
    static constexpr uint8_t DEFAULT_ADDRESS = 0x27U;

    /* Button masks (1 = pressed). Combine with | and test with &. */
    static constexpr uint8_t S1 = DevLabDDP::DPAD_S1;  /* PA0 */
    static constexpr uint8_t S2 = DevLabDDP::DPAD_S2;  /* PA1 */
    static constexpr uint8_t S3 = DevLabDDP::DPAD_S3;  /* PA2 */
    static constexpr uint8_t S4 = DevLabDDP::DPAD_S4;  /* PB5 */
    static constexpr uint8_t ALL = S1 | S2 | S3 | S4;

    explicit DevLab_DPad(TwoWire &wire = Wire, uint8_t address = DEFAULT_ADDRESS, uint32_t clock = 400000UL)
    : _bus(wire, clock), _ddp(_bus, DevLabDDP::DEVICE_DPAD), _address(address), _clock(clock) {}

    bool begin();
    bool begin(uint8_t sdaPin, uint8_t sclPin, uint32_t clock = 400000UL);
    bool beginRecovered(uint8_t sdaPin, uint8_t sclPin, uint32_t timeoutUs = 20000, bool restart = false);

    /* Reads the debounced button snapshot into `buttons` (bitmask of S1..S4).
     * Returns false on an I2C error. */
    bool readButtons(uint8_t &buttons);

    /* Polling helpers: update() reads the module once and keeps the previous
     * snapshot, so wasPressed/wasReleased report edges between two update()
     * calls. Returns false on an I2C error (state is left unchanged). */
    bool update();
    uint8_t state() const { return _state; }
    bool isPressed(uint8_t mask) const { return (_state & mask) == mask; }
    bool wasPressed(uint8_t mask) const { return ((_state & ~_previous) & mask) != 0U; }
    bool wasReleased(uint8_t mask) const { return ((_previous & ~_state) & mask) != 0U; }

    bool busReady() const { return _busReady; }
    bool isConnected() const { return _verified; }
    uint8_t address() const { return _address; }
    const DevLabDDP::DeviceInfo &deviceInfo() const { return _info; }
    void printInfo(Print &out = Serial) const;

    DevLabDDP::Master &protocol() { return _ddp; }
    DevLab_I2C_Orchestrator &bus() { return _bus; }

private:
    DevLab_I2C_Orchestrator _bus;
    DevLabDDP::Master _ddp;
    uint8_t _address;
    uint32_t _clock;
    bool _busReady = false;
    bool _verified = false;
    uint8_t _state = 0U;
    uint8_t _previous = 0U;
    DevLabDDP::DeviceInfo _info;
};

#endif
