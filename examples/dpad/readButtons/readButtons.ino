/**
 * @file readButtons.ino
 * @brief Reads the four buttons (S1-S4) of the D-Pad module over I2C.
 *
 * Prints the state of S1..S4 over Serial only when it changes.
 * Compatible with ESP32, RP2040/RP2350, STM32 y AVR.
 *
 * @author Jonathan Mejorado
 * @organization UNIT Electronics MX
 */

#include <Arduino.h>
#include <Wire.h>
#include <DevLab_DPad.h>

#if defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_RP2350)
  #define I2C_BUS Wire1
  #if defined(PIN_WIRE1_SDA) && defined(PIN_WIRE1_SCL) && \
      PIN_WIRE1_SDA == 24U && PIN_WIRE1_SCL == 25U
    // Pulsar connector pins.
    constexpr int SDA_PIN = 24;
    constexpr int SCL_PIN = 25;
  #else
    // Generic RP2040/RP2350 wiring.
    constexpr int SDA_PIN = 12;
    constexpr int SCL_PIN = 13;
  #endif
  constexpr uint32_t I2C_FREQ = 400000UL;
#elif defined(ARDUINO_ARCH_ESP32)
  #define I2C_BUS Wire
  constexpr int SDA_PIN = 6;
  constexpr int SCL_PIN = 7;
  constexpr uint32_t I2C_FREQ = 400000UL;
#elif defined(ARDUINO_ARCH_STM32)
  // STM32duino: default I2C pins of the selected board.
  #define I2C_BUS Wire
  constexpr int SDA_PIN = SDA;
  constexpr int SCL_PIN = SCL;
  constexpr uint32_t I2C_FREQ = 400000UL;
#elif defined(ARDUINO_ARCH_AVR)
  // AVR has fixed I2C pins (Uno/Nano: A4/A5, Mega: 20/21, Leonardo: 2/3);
  // SDA/SCL come from the board variant and begin() ignores the pin numbers.
  #define I2C_BUS Wire
  constexpr int SDA_PIN = SDA;
  constexpr int SCL_PIN = SCL;
  constexpr uint32_t I2C_FREQ = 100000UL;  // 400 kHz fails with the level shifter on UNO
#else
#error "Use an ESP32, RP2040, RP2350, STM32, or AVR master"
#endif

constexpr uint8_t DPAD_ADDRESS = 0x27U;
constexpr uint32_t READ_INTERVAL_MS = 10U;

DevLab_DPad dpad(I2C_BUS, DPAD_ADDRESS, I2C_FREQ);
uint8_t last_buttons = 0xFFU;

void setup() {
  Serial.begin(115200);
  delay(500);
  if (!dpad.beginRecovered(SDA_PIN, SCL_PIN)) {
    Serial.println("D-Pad module not found; check wiring, power and firmware");
    return;
  }
  dpad.printInfo(Serial);
}

void loop() {
  if (!dpad.isConnected()) return;

  uint8_t buttons = 0;
  if (!dpad.readButtons(buttons)) {
    Serial.println("ERROR: D-Pad read failed");
    delay(500U);
    return;
  }
  if (buttons != last_buttons) {
    Serial.print("S1=");
    Serial.print((buttons & DevLab_DPad::S1) ? 1 : 0);
    Serial.print(" S2=");
    Serial.print((buttons & DevLab_DPad::S2) ? 1 : 0);
    Serial.print(" S3=");
    Serial.print((buttons & DevLab_DPad::S3) ? 1 : 0);
    Serial.print(" S4=");
    Serial.println((buttons & DevLab_DPad::S4) ? 1 : 0);
    last_buttons = buttons;
  }
  delay(READ_INTERVAL_MS);
}
