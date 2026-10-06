# DevLab_DPad examples

| Example | Purpose |
|---|---|
| `dpad/readButtons` | Print the state of S1..S4 whenever it changes. |
| `dpad/buttonEvents` | Print one "pressed" / "released" event per button edge. |
| `i2c/changeAddress` | Scan the bus and change the I2C address of a D-Pad module (default `0x27`). |

All examples verify Device ID `0x0107` before sending any command.
