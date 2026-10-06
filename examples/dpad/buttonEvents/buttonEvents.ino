/**
 * @file buttonEvents.ino
 * @brief Detecta eventos de pulsación y liberación de S1-S4 del módulo D-Pad.
 *
 * Usa update()/wasPressed()/wasReleased() para imprimir cada flanco una vez.
 * Compatible con ESP32, RP2040/RP2350, STM32 y AVR.
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
  constexpr uint32_t I2C_FREQ = 100000UL;  // 400 kHz falla con el level shifter en UNO
#else
#error "Use an ESP32, RP2040, RP2350, STM32, or AVR master"
#endif

constexpr uint8_t DPAD_ADDRESS = 0x27U;
constexpr uint32_t READ_INTERVAL_MS = 10U;

DevLab_DPad dpad(I2C_BUS, DPAD_ADDRESS, I2C_FREQ);

struct Button {
  uint8_t mask;
  const char *name;
};

const Button BUTTONS[] = {
  {DevLab_DPad::S1, "S1"},
  {DevLab_DPad::S2, "S2"},
  {DevLab_DPad::S3, "S3"},
  {DevLab_DPad::S4, "S4"},
};

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

  if (!dpad.update()) {
    Serial.println("ERROR: D-Pad read failed");
    delay(500U);
    return;
  }
  for (const Button &button : BUTTONS) {
    if (dpad.wasPressed(button.mask)) {
      Serial.print(button.name);
      Serial.println(" pressed");
    }
    if (dpad.wasReleased(button.mask)) {
      Serial.print(button.name);
      Serial.println(" released");
    }
  }
  delay(READ_INTERVAL_MS);
}
