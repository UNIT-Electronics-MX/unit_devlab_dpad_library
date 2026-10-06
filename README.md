# DevLab_DPad

Arduino library for the I2C D-Pad module (four buttons, S1 to S4) using the
DevLab Device Protocol (DDP) over I2C.

This library wraps the shared
[`DevLabDDP`](https://github.com/UNIT-Electronics-MX/unit_devlab_ddp_library)
master and the
[`DevLab_Interface`](https://github.com/UNIT-Electronics-MX/unit_devlab_interface_library)
`DevLab_I2C_Orchestrator` bus class into a single `DevLab_DPad` object.
Firmware: `unit_firmware_i2c_d_pad_module`.

Compatible with ESP32, RP2040/RP2350, STM32 (STM32duino core) and AVR.

# Features

- Device identification (Device ID `0x0107`) before any command
- `readButtons()` returns a debounced bitmask of S1..S4 (1 = pressed)
- `update()` with `isPressed` / `wasPressed` / `wasReleased` for edge detection
- I2C address scan and reassignment (`0x08` to `0x77`), default address `0x27`
- Bus recovery (`beginRecovered`) for a slave left mid-transaction

# Button mapping

| Button | Mask | MCU pin |
|---|---|---|
| S1 | `DevLab_DPad::S1` (bit 0) | PA0 |
| S2 | `DevLab_DPad::S2` (bit 1) | PA1 |
| S3 | `DevLab_DPad::S3` (bit 2) | PA2 |
| S4 | `DevLab_DPad::S4` (bit 3) | PB5 |

# Quick Start Example

```cpp
#include <Arduino.h>
#include <Wire.h>
#include <DevLab_DPad.h>

DevLab_DPad dpad(Wire, DevLab_DPad::DEFAULT_ADDRESS, 400000);

void setup() {
  Serial.begin(115200);
  // ESP32: 6/7. RP2040/RP2350: 24/25 (Pulsar) or 12/13.
  // STM32: the board's default Wire pins (SDA, SCL).
  if (!dpad.beginRecovered(6, 7)) {
    Serial.println("D-Pad module not found");
  }
}

void loop() {
  if (dpad.update()) {
    if (dpad.wasPressed(DevLab_DPad::S1)) Serial.println("S1 pressed");
    if (dpad.wasReleased(DevLab_DPad::S1)) Serial.println("S1 released");
  }
  delay(10);
}
```

# API

| Method | Description |
|---|---|
| `DevLab_DPad(wire, address, clock)` | Create the object (default `Wire`, `0x27`, 400 kHz). |
| `begin()` / `begin(sda, scl, clock)` | Start the bus and verify the device. |
| `beginRecovered(sda, scl, timeoutUs, restart)` | Same, clearing a stuck bus first. |
| `readButtons(buttons)` | Read the S1..S4 bitmask in one I2C transaction. |
| `update()` | Read once and keep the previous snapshot for edge detection. |
| `state()` / `isPressed(mask)` | Last snapshot from `update()`. |
| `wasPressed(mask)` / `wasReleased(mask)` | Edge between the last two `update()` calls. |
| `isConnected()` / `busReady()` | Result of the last `begin`. |
| `deviceInfo()` / `printInfo(out)` | Identity reported by the module. |
| `protocol()` / `bus()` | Access the underlying DDP master and I2C bus. |

All calls return `false` on an I2C error or if `begin` has not verified the
device.

# License

MIT, see [LICENSE](LICENSE).
