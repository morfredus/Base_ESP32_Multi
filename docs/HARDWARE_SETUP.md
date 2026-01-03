# Hardware Setup Guide

**Version:** 0.9.0
**Last Updated:** 2026-01-03

This guide explains how to configure and wire hardware peripherals for Base_ESP32_Multi. Learn about supported boards, GPIO pin assignments, and how to enable/disable features.

---

## 📋 Table of Contents

1. [Supported Boards](#supported-boards)
2. [Quick Configuration](#quick-configuration)
3. [GPIO Pin Mappings](#gpio-pin-mappings)
4. [Peripheral Wiring Guides](#peripheral-wiring-guides)
5. [Enabling/Disabling Features](#enablingdisabling-features)
6. [Safety and Best Practices](#safety-and-best-practices)
7. [Troubleshooting](#troubleshooting)

---

## Supported Boards

Base_ESP32_Multi supports multiple ESP32 development boards with pre-configured GPIO mappings.

### ESP32-S3 DevKitC-1 (Recommended)

**Variants:**
- **N16R8** - 16MB Flash + 8MB PSRAM (best performance)
- **N8R8** - 8MB Flash + 8MB PSRAM

**Features:**
- USB-C connector
- Built-in USB-to-Serial (no external chip needed)
- More GPIO pins available
- Support for PSRAM (fast external RAM)
- Dual-core 240MHz processor

**PlatformIO Environment:**
```ini
[platformio]
default_envs = esp32s3_n16r8
```

---

### ESP32 DevKitC (Classic)

**Variant:**
- **4MB Flash, no PSRAM**

**Features:**
- Micro-USB connector
- Widely available and cheaper
- Mature ecosystem
- Dual-core 240MHz processor

**PlatformIO Environment:**
```ini
[platformio]
default_envs = esp32devkitc
```

---

## Quick Configuration

### Step 1: Edit board_config.h

Open `include/board_config.h` and enable/disable features based on your hardware:

```cpp
// ==========================================================
// HARDWARE FEATURE FLAGS
// ==========================================================

// Enable only the features your board has connected

// --- Displays ---
#define HAS_OLED           // OLED SSD1306 display via I2C
#define HAS_ST7789         // ST7789 TFT display via SPI
//#define HAS_ILI9341      // ILI9341 TFT display via SPI (alternative)

// --- LEDs ---
#define HAS_LED_BUILTIN    // Built-in LED (always available)
#define HAS_LED_RGB        // RGB LED (3 separate LEDs)
#define HAS_NEOPIXEL       // NeoPixel/WS2812B LED strip
//#define HAS_NEOPIXEL_MATRIX  // NeoPixel 8x8 Matrix

// --- Sensors ---
//#define HAS_DHT          // DHT11/DHT22 temperature sensor
//#define HAS_PIR          // PIR motion sensor
//#define HAS_ULTRASONIC   // HC-SR04 distance sensor
//#define HAS_LDR          // Light sensor (LDR)

// --- Input Devices ---
//#define HAS_ROTARY       // Rotary encoder (HW-040)
//#define HAS_GPS          // GPS module (UART)

// --- Output Devices ---
//#define HAS_BUZZER       // Piezo buzzer
//#define HAS_SD_CARD      // SD Card reader (SPI)
```

**Important:** Uncomment (remove `//`) to enable a feature, add `//` to disable.

### Step 2: Build and Upload

```bash
pio run --target upload
```

---

## GPIO Pin Mappings

### ESP32-S3 DevKitC-1

#### Display Interfaces

**OLED SSD1306 (I2C):**
| Signal | GPIO | Description |
|--------|------|-------------|
| SDA | 15 | I2C Data (needs 4.7kΩ pull-up to 3.3V) |
| SCL | 16 | I2C Clock (needs 4.7kΩ pull-up to 3.3V) |
| VCC | 3.3V | Power supply |
| GND | GND | Ground |

**ST7789/ILI9341 TFT (SPI):**
| Signal | GPIO | Alias | Description |
|--------|------|-------|-------------|
| MOSI | 11 | DIN/SDA/SDI | Master Out Slave In (data) |
| SCLK | 12 | SCK/CLK | SPI Clock |
| CS | 10 | TCS | Chip Select |
| DC | 9 | A0/RS | Data/Command select |
| RST | 14 | RESET/RES | Reset (active low) |
| BL | 7 | LED | Backlight control (PWM capable) |
| VCC | 3.3V or 5V | - | Check your TFT module specs |
| GND | GND | - | Ground |

**Note:** MISO (GPIO 13) is optional, only needed for reading from TFT.

---

#### LEDs

**RGB LED (3 separate LEDs):**
| LED | GPIO | Resistor | Description |
|-----|------|----------|-------------|
| Red | 21 | 220-470Ω | Red LED with current-limiting resistor |
| Green | 41 | 220-470Ω | Green LED with current-limiting resistor |
| Blue | 42 | 220-470Ω | Blue LED with current-limiting resistor |
| Common | GND | - | Common cathode (or anode via resistor) |

**NeoPixel/WS2812B Strip:**
| Signal | GPIO | Description |
|--------|------|-------------|
| DIN | 48 | Data input (3.3V signal, LED powered by 5V) |
| VCC | 5V | Power supply (external source recommended) |
| GND | GND | Common ground with ESP32 |

**NeoPixel Matrix 8×8 (WS2812B-64):**
| Signal | GPIO | Description |
|--------|------|-------------|
| DIN | 3 | Data input to matrix |
| VCC | 5V | Power supply (3A minimum for full brightness) |
| GND | GND | Common ground |

**⚠️ Power Warning:** 64 LEDs at full white brightness draw ~3.8A. Use external 5V power supply rated for at least 3-4A. Connect ESP32 GND to power supply GND!

---

#### Buttons

| Button | GPIO | Type | Pull-up | Description |
|--------|------|------|---------|-------------|
| BOOT | 0 | Built-in | External | Strapping pin, active LOW |
| Button 1 | 38 | External | Internal | User button 1 |
| Button 2 | 39 | External | Internal | User button 2 |

**Wiring (active LOW):**
```
Button → GPIO pin
Button → GND
```

Internal pull-up enabled in code via `pinMode(BUTTON_X, INPUT_PULLUP)`.

---

#### Sensors

**DHT11/DHT22 Temperature:**
| Signal | GPIO | Pull-up | Description |
|--------|------|---------|-------------|
| DATA | 5 | 10kΩ to 3.3V | Data line |
| VCC | 3.3V | - | Power |
| GND | GND | - | Ground |

**PIR Motion Sensor:**
| Signal | GPIO | Description |
|--------|------|-------------|
| OUT | 46 | Digital output (HIGH when motion detected) |
| VCC | 5V | Power (check your PIR module) |
| GND | GND | Ground |

**⚠️ Voltage Warning:** If PIR outputs 5V, use a voltage divider (10kΩ + 20kΩ) to bring it down to 3.3V!

**HC-SR04 Ultrasonic Distance:**
| Signal | GPIO | Voltage Divider | Description |
|--------|------|-----------------|-------------|
| TRIG | 2 | No | Trigger (3.3V output OK) |
| ECHO | 35 | **Yes** | Echo (outputs 5V → needs divider!) |
| VCC | 5V | - | Power |
| GND | GND | - | Ground |

**ECHO Voltage Divider:**
```
HC-SR04 ECHO → 10kΩ → GPIO 35
                 ↓
               20kΩ → GND

Vout = 5V × (20k / 30k) = 3.3V ✓
```

**LDR Light Sensor:**
| Component | Connection | Description |
|-----------|------------|-------------|
| LDR | 3.3V → LDR → GPIO 4 | Photoresistor |
| Resistor | GPIO 4 → 10kΩ → GND | Pull-down resistor |

**Reading:** Analog value via `analogRead(LIGHT_SENSOR)` (0-4095).

---

#### Other Peripherals

**Buzzer (Passive or Active):**
| Signal | GPIO | Description |
|--------|------|-------------|
| + | 6 | Control pin (via transistor if >12mA) |
| - | GND | Ground |

**Recommended circuit (if buzzer draws >12mA):**
```
GPIO 6 → 1kΩ → Base (NPN transistor)
Collector → Buzzer+ → 5V
Emitter → GND
```

**Rotary Encoder (HW-040):**
| Signal | GPIO | Debounce Cap | Description |
|--------|------|--------------|-------------|
| CLK (A) | 47 | 10nF | Channel A |
| DT (B) | 45 | 10nF | Channel B |
| SW | 40 | - | Push button (internal pull-up) |
| VCC | 3.3V | - | Power |
| GND | GND | - | Ground |

**Optional hardware debounce:** Add 10nF capacitor between CLK/DT and GND.

**GPS Module (UART):**
| Signal | GPIO | Voltage Divider | Description |
|--------|------|-----------------|-------------|
| GPS TX | 18 (RXD) | **Yes if 5V** | GPS transmit → ESP32 receive |
| GPS RX | 17 (TXD) | No | ESP32 transmit → GPS receive |
| PPS | 8 | No | 1 Pulse Per Second (optional) |
| VCC | 5V or 3.3V | - | Check your GPS module |
| GND | GND | - | Ground |

**SD Card (SPI):**
| Signal | GPIO | Description |
|--------|------|-------------|
| MISO | 13 | Master In Slave Out |
| MOSI | 11 | Master Out Slave In (shared with TFT) |
| SCLK | 12 | SPI Clock (shared with TFT) |
| CS | 1 | Chip Select |
| VCC | 3.3V | Power |
| GND | GND | Ground |

---

### ESP32 Classic DevKitC

**Key Differences from ESP32-S3:**

| Feature | ESP32-S3 | ESP32 Classic |
|---------|----------|---------------|
| Built-in LED | GPIO 48 | GPIO 2 |
| I2C SDA/SCL | GPIO 15/16 | GPIO 21/22 |
| SPI | Custom pins | GPIO 23 (MOSI), 19 (MISO), 18 (SCK) |
| Available GPIO | More | Limited (some strapping pins) |

⚠️ **ESP32 Classic has fewer usable GPIO pins. Some pins share functions with built-in LEDs or flash.**

See `include/board_config.h` lines 149+ for complete ESP32 Classic pinout.

---

## Peripheral Wiring Guides

### Wiring an OLED Display (SSD1306)

**Parts Needed:**
- OLED 128×64 SSD1306 I2C module (usually 0.96" or 1.3")
- 4 jumper wires
- 2× 4.7kΩ resistors (for pull-ups, if not on OLED module)

**Steps:**

1. **Enable in code:**
   ```cpp
   // include/board_config.h
   #define HAS_OLED
   ```

2. **Wire connections (ESP32-S3):**
   ```
   OLED VCC → ESP32 3.3V
   OLED GND → ESP32 GND
   OLED SDA → ESP32 GPIO 15
   OLED SCL → ESP32 GPIO 16
   ```

3. **Add pull-ups (if needed):**
   - Some OLED modules have built-in pull-ups
   - If display doesn't work, add 4.7kΩ resistor: SDA → 3.3V
   - And 4.7kΩ resistor: SCL → 3.3V

4. **Upload and test:**
   ```bash
   pio run --target upload
   ```

5. **Check Serial Monitor:**
   ```
   --- Initializing Displays ---
   ✓ OLED initialized
   ```

**Troubleshooting:**
- **No display:** Check I2C address (default: 0x3C). Try `i2cdetect` scan.
- **Garbled display:** Verify OLED_WIDTH and OLED_HEIGHT in `config.h`
- **No response:** Check pull-ups, wiring, and 3.3V power

---

### Wiring a TFT Display (ST7789)

**Parts Needed:**
- ST7789 240×240 TFT display (or ILI9341 240×320)
- 8 jumper wires

**Steps:**

1. **Enable in code:**
   ```cpp
   // include/board_config.h
   #define HAS_ST7789
   ```

2. **Wire connections (ESP32-S3):**
   ```
   TFT VCC → ESP32 3.3V (or 5V, check module)
   TFT GND → ESP32 GND
   TFT MOSI (DIN) → ESP32 GPIO 11
   TFT SCK (CLK) → ESP32 GPIO 12
   TFT CS → ESP32 GPIO 10
   TFT DC (RS/A0) → ESP32 GPIO 9
   TFT RST → ESP32 GPIO 14
   TFT BL (LED) → ESP32 GPIO 7
   ```

3. **Configure display size:**
   ```cpp
   // include/config.h
   #define TFT_WIDTH  240
   #define TFT_HEIGHT 240   // Or 320 for ILI9341
   #define TFT_ROTATION 2   // 0-3 for different orientations
   ```

4. **Upload and test**

5. **Check display:**
   - Backlight should turn on (GPIO 7 HIGH)
   - Project name and version should appear

**Troubleshooting:**
- **Blank white screen:** Wrong VCC voltage or initialization failed
- **Garbled colors:** Check MOSI/SCK wiring
- **Nothing displays:** Verify CS, DC, RST connections
- **Dim backlight:** BL pin might need PWM control

---

### Wiring NeoPixel LED Strip

**Parts Needed:**
- WS2812B LED strip (or ring/matrix)
- External 5V power supply (1A per 20 LEDs recommended)
- 1× 470Ω resistor (optional but recommended for data line)
- 1× 1000µF capacitor (optional but recommended for power smoothing)

**Steps:**

1. **Enable in code:**
   ```cpp
   // include/board_config.h
   #define HAS_NEOPIXEL
   #define NEOPIXEL_NUM 30  // Number of LEDs
   ```

2. **Wire connections (ESP32-S3):**
   ```
   ESP32 GPIO 48 → (470Ω resistor) → LED Strip DIN
   ESP32 GND → Power Supply GND → LED Strip GND
   Power Supply 5V → LED Strip VCC
   (1000µF capacitor between 5V and GND near LEDs)
   ```

**⚠️ Critical:** Always connect ESP32 GND to power supply GND!

3. **Power calculation:**
   - Each LED draws ~60mA at full white brightness
   - 30 LEDs × 60mA = 1.8A → use 2A+ power supply
   - ESP32 USB power (500mA) is NOT enough for >8 LEDs

4. **Upload and test:**
   - Built-in heartbeat uses NeoPixel (cycles colors)

**Troubleshooting:**
- **LEDs don't light:** Check 5V power, verify GND connection
- **First LED works, others don't:** Broken data line, check resistor/connections
- **Random colors/glitching:** Power supply insufficient or GND not connected
- **Dim LEDs:** Lower brightness in code or increase power supply amperage

---

### Wiring NeoPixel Matrix 8×8

**New in v0.9.0!**

**Parts Needed:**
- WS2812B 8×8 matrix (64 LEDs)
- External 5V 3-4A power supply
- Jumper wires

**Steps:**

1. **Enable in code:**
   ```cpp
   // include/board_config.h
   #define HAS_NEOPIXEL_MATRIX
   ```

2. **Wire connections (ESP32-S3):**
   ```
   ESP32 GPIO 3 → Matrix DIN
   ESP32 GND → Power GND → Matrix GND
   Power 5V → Matrix VCC
   ```

3. **Power requirements:**
   - 64 LEDs × 60mA = 3.84A at full brightness!
   - **Minimum 3A power supply required**
   - Consider limiting brightness in code to reduce current

4. **Code example:**
   ```cpp
   #ifdef HAS_NEOPIXEL_MATRIX
   #include <Adafruit_NeoMatrix.h>
   Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
       8, 8, NEOPIXEL_MATRIX,
       NEO_MATRIX_TOP + NEO_MATRIX_LEFT +
       NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
       NEO_GRB + NEO_KHZ800
   );
   #endif
   ```

---

## Enabling/Disabling Features

### Method 1: Use #define Flags (Recommended)

Edit `include/board_config.h`:

```cpp
// Enable feature (uncommented)
#define HAS_OLED

// Disable feature (commented out)
//#define HAS_DHT
```

### Method 2: Conditional Compilation

The firmware automatically skips code for disabled features:

```cpp
#ifdef HAS_OLED
    // This code only compiles if HAS_OLED is defined
    display_oled.begin();
#endif
```

**Benefits:**
- Reduces flash memory usage
- Prevents unused library linking
- Avoids GPIO conflicts

### Checking Compiled Features

After upload, check Serial Monitor:

```
=== Base_ESP32_Multi v0.9.0 ===
Enabled features:
  - OLED Display
  - ST7789 TFT
  - RGB LED
  - NeoPixel Strip
  - WiFi
  - Web Server
  - OTA Updates
```

---

## Safety and Best Practices

### Voltage Levels

**⚠️ CRITICAL:** ESP32 GPIO pins are **3.3V ONLY**. Never apply 5V directly!

**Safe:**
- 3.3V sensors → ESP32 GPIO ✓
- ESP32 GPIO → 5V device inputs (usually OK, check datasheet) ✓
- Battery (7.4V-12V) → voltage regulator → ESP32 3.3V ✓

**UNSAFE:**
- 5V sensor output → ESP32 GPIO ❌ (use voltage divider!)
- 12V relay → ESP32 GPIO ❌ (use transistor + optocoupler!)
- Reversed polarity ❌ (double-check VCC/GND!)

### Voltage Divider Formula

To convert 5V to 3.3V:

```
       5V
        │
       [R1 = 10kΩ]
        │
        ├─────→ ESP32 GPIO (3.3V)
        │
       [R2 = 20kΩ]
        │
       GND

Vout = Vin × (R2 / (R1 + R2))
     = 5V × (20kΩ / 30kΩ)
     = 3.3V ✓
```

### Current Limits

**ESP32 GPIO current limits:**
- Per pin: 12mA (absolute max 40mA)
- Total for all pins: ~200mA

**Solutions for high-current devices:**
- **LEDs:** Use 220-470Ω current-limiting resistor
- **Buzzer:** Use transistor (NPN 2N2222 or similar)
- **Relay:** Use transistor + flyback diode
- **Motor:** Use motor driver (L298N, TB6612, etc.)

### Pull-up/Pull-down Resistors

**I2C (SDA/SCL):**
- **Required:** 4.7kΩ pull-up to 3.3V
- Often built into modules, but add if missing

**Buttons:**
- **Internal pull-ups available** via `INPUT_PULLUP` mode
- External 10kΩ pull-up/pull-down for reliability

**Floating pins:**
- Never leave input pins floating (undefined state)
- Use pull-up or pull-down resistors

### Power Supply Guidelines

**USB Power (500mA):**
- ESP32 alone: ✓
- ESP32 + OLED: ✓
- ESP32 + TFT: ✓
- ESP32 + 8 NeoPixels: ✓
- ESP32 + 30 NeoPixels: ❌ (needs external power)

**External 5V Power (2A+):**
- Required for NeoPixel strips (>8 LEDs)
- Required for motors, relays
- Connect ESP32 GND to power supply GND!

**Battery Power:**
- 7.4V LiPo → voltage regulator (AMS1117-3.3) → ESP32
- Add 100µF capacitor near ESP32 for stability

---

## Troubleshooting

### "GPIO already in use" Error

**Problem:** Two features trying to use the same GPIO.

**Solution:**
1. Check `board_config.h` for conflicting pins
2. Disable one feature or change GPIO assignment

### Display Shows Nothing

**OLED:**
1. Check I2C address (0x3C or 0x3D) - use I2C scanner
2. Verify pull-up resistors (4.7kΩ to 3.3V)
3. Check wiring (SDA/SCL not swapped)

**TFT:**
1. Verify backlight is on (BL pin HIGH)
2. Check SPI wiring (MOSI, SCK, CS, DC, RST)
3. Confirm correct TFT_WIDTH/HEIGHT in config.h

### LEDs Don't Work

**RGB LED:**
1. Check resistors (220-470Ω in series)
2. Verify common cathode vs common anode
3. Test with simple digitalWrite() HIGH/LOW

**NeoPixel:**
1. Verify 5V power supply connected
2. Check GND is common between ESP32 and power supply
3. Confirm data pin (GPIO 48 for strip, GPIO 3 for matrix)
4. Reduce brightness if power supply insufficient

### Sensor Reads Incorrect Values

**DHT:**
1. Add 10kΩ pull-up resistor to DATA line
2. Wait 2 seconds between reads
3. Check library compatibility (DHT11 vs DHT22)

**HC-SR04:**
1. Verify voltage divider on ECHO pin
2. Check 5V power supply
3. Ensure no obstacles within 2cm of sensor

### Compilation Errors

**"undefined reference to..."**
- Missing library in `platformio.ini`
- Feature enabled but library not installed

**"error: 'XXX' was not declared"**
- Feature flag (#define HAS_XXX) missing in board_config.h
- Typo in GPIO pin name

---

## Getting Help

**Before asking for help:**
1. Check Serial Monitor output at 115200 baud
2. Verify wiring matches documentation
3. Measure voltages with multimeter (3.3V, 5V, GND)
4. Test with minimal hardware first

**GitHub Issues:** https://github.com/morfredus/Base_ESP32_Multi/issues

Include in your issue:
- Board type (ESP32-S3 or Classic)
- Enabled features (#define list)
- Serial Monitor output
- Wiring description or photo

---

## Summary

- ✅ Edit `board_config.h` to enable/disable hardware features
- ✅ Follow GPIO pinout tables for your board (ESP32-S3 vs Classic)
- ✅ Always use 3.3V logic or voltage dividers for 5V sensors
- ✅ Add current-limiting resistors for LEDs
- ✅ Use external power for NeoPixels, motors, relays
- ✅ Connect all GNDs together (ESP32, power supply, peripherals)
- ✅ Add pull-up resistors for I2C (4.7kΩ)

Happy building! 🔧
