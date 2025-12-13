# Pin Connection Guide - ESP32 Base

> 📌 **Beginner's Guide**: This document explains how to physically connect components to your ESP32-S3 or ESP32 Classic board.

**[Version Française](PIN_MAPPING_FR.md)**

## 🎯 Table of Contents
- [ESP32-S3 DevKitC-1](#esp32-s3-devkitc-1)
- [ESP32 Classic DevKitC](#esp32-classic-devkitc)
- [Detailed Connection Diagrams](#detailed-connection-diagrams)
- [Beginner Tips](#beginner-tips)

---

## ESP32-S3 DevKitC-1

### 📋 Pin Summary Table

| Component | Signal | GPIO Pin | Description | Notes |
|-----------|--------|----------|-------------|-------|
| **BOOT Button** | Button | GPIO 0 | Built-in button on board | Already present, no need to connect |
| **RGB LED (NeoPixel)** | Data | GPIO 48 | Built-in addressable RGB LED | Already present on board |
| **OLED SSD1306** | SDA | GPIO 15 | I2C Data | Connect to OLED SDA pin |
| **OLED SSD1306** | SCL | GPIO 16 | I2C Clock | Connect to OLED SCL pin |
| **OLED SSD1306** | VCC | 3.3V | Power | 3V3 pin on ESP32 |
| **OLED SSD1306** | GND | GND | Ground | GND pin on ESP32 |
| **TFT ST7789** | MOSI (SDA) | GPIO 11 | SPI Data | SDA pin on ST7789 |
| **TFT ST7789** | SCLK (SCL) | GPIO 12 | SPI Clock | SCL pin on ST7789 |
| **TFT ST7789** | CS | GPIO 10 | Chip Select | Device selection |
| **TFT ST7789** | DC | GPIO 9 | Data/Command | Data/command indicator |
| **TFT ST7789** | RST | GPIO 13 | Reset | Screen reset |
| **TFT ST7789** | BL | GPIO 7 | Backlight | LED backlight |
| **TFT ST7789** | VCC | 3.3V | Power | 3V3 pin on ESP32 |
| **TFT ST7789** | GND | GND | Ground | GND pin on ESP32 |

### 🔌 OLED SSD1306 Connection Diagram (I2C)

```
ESP32-S3           OLED SSD1306
┌─────────┐        ┌──────────┐
│         │        │          │
│ GPIO 15 ├───────►│ SDA      │
│ GPIO 16 ├───────►│ SCL      │
│    3V3  ├───────►│ VCC      │
│    GND  ├───────►│ GND      │
│         │        │          │
└─────────┘        └──────────┘
```

**Default I2C Address**: `0x3C` (sometimes `0x3D`, check with I2C scanner)

### 🖥️ TFT ST7789 Connection Diagram (SPI)

```
ESP32-S3           ST7789 TFT
┌─────────┐        ┌──────────┐
│         │        │          │
│ GPIO 11 ├───────►│ MOSI/SDA │ Data
│ GPIO 12 ├───────►│ SCLK/SCL │ Clock
│ GPIO 10 ├───────►│ CS       │ Chip Select
│ GPIO  9 ├───────►│ DC       │ Data/Command
│ GPIO 13 ├───────►│ RST      │ Reset
│ GPIO  7 ├───────►│ BL       │ Backlight
│    3V3  ├───────►│ VCC      │ Power
│    GND  ├───────►│ GND      │ Ground
│         │        │          │
└─────────┘        └──────────┘
```

---

## ESP32 Classic DevKitC

### 📋 Pin Summary Table

| Component | Signal | GPIO Pin | Description | Notes |
|-----------|--------|----------|-------------|-------|
| **BOOT Button** | Button | GPIO 0 | Built-in button on board | Already present |
| **Builtin LED** | LED | GPIO 2 | Blue built-in LED | Already present on board |
| **OLED SSD1306** | SDA | GPIO 21 | I2C Data | Connect to OLED SDA pin |
| **OLED SSD1306** | SCL | GPIO 22 | I2C Clock | Connect to OLED SCL pin |
| **OLED SSD1306** | VCC | 3.3V | Power | 3V3 pin on ESP32 |
| **OLED SSD1306** | GND | GND | Ground | GND pin on ESP32 |
| **TFT ST7789** | SCLK (SCL) | GPIO 18 | SPI Clock | SCL pin on screen |
| **TFT ST7789** | MOSI (SDA) | GPIO 23 | SPI Data | SDA pin on screen |
| **TFT ST7789** | CS | GPIO 27 | Chip Select | Device selection |
| **TFT ST7789** | DC | GPIO 14 | Data/Command | Data/command indicator |
| **TFT ST7789** | RST | GPIO 25 | Reset | Screen reset |
| **TFT ST7789** | BL | GPIO 32 | Backlight | LED backlight |
| **TFT ST7789** | VCC | 3.3V | Power | 3V3 pin on ESP32 |
| **TFT ST7789** | GND | GND | Ground | GND pin on ESP32 |

### 🔌 OLED SSD1306 Connection Diagram (I2C)

```
ESP32 Classic      OLED SSD1306
┌─────────┐        ┌──────────┐
│         │        │          │
│ GPIO 21 ├───────►│ SDA      │
│ GPIO 22 ├───────►│ SCL      │
│    3V3  ├───────►│ VCC      │
│    GND  ├───────►│ GND      │
│         │        │          │
└─────────┘        └──────────┘
```

**Default I2C Address**: `0x3C` (sometimes `0x3D`, verify with I2C scanner)

---

## 🛠️ Detailed Connection Diagrams

### OLED SSD1306 - I2C Configuration

The OLED screen uses the **I2C** protocol which requires only **2 data wires** (SDA and SCL) plus power:

**I2C Advantages**:
- ✅ Less wiring (only 4 wires total)
- ✅ Easy bus sharing (multiple I2C devices on same pins)
- ✅ Simple configuration

**⚠️ Important Points**:
1. **Verify I2C address**: Usually `0x3C`, but some modules use `0x3D`. Use I2C scanner if in doubt.
2. **Power**: Strictly use 3.3V (not 5V on ESP32).
3. **Pull-up resistors**: Often already present on OLED module, no need to add.

### TFT ST7789 - SPI Configuration

TFT displays use the **SPI** protocol which requires more wires but offers higher speed:

**SPI Advantages**:
- ✅ Very fast (smooth refresh)
- ✅ Supports high-resolution color displays
- ✅ No address conflicts

**⚠️ Important Points**:
1. **Power**: Some displays accept 5V, others only 3.3V. **Check your module documentation!**
2. **Pin order**: Names may vary (MOSI/SDA, SCLK/SCL). Refer to table above.
3. **Backlight (BL)**: If screen stays black, verify backlight is connected and powered (HIGH on BL pin).
4. **Chip Select (CS)**: Must be LOW to activate display. Code handles this automatically.

---

## 💡 Beginner Tips

### 🔍 Identifying Your ESP32 Pins

1. **Search for "pinout" diagram** of your exact board (Google: "ESP32-S3 DevKitC pinout")
2. **Pins are numbered**: Count from edge or find silkscreen labels
3. **Use a multimeter**: In continuity mode, test which physical pin matches desired GPIO

### 🔌 How to Wire Properly

1. **Cut power**: Always unplug USB cable before connecting/disconnecting components
2. **Quality wires**: Avoid long or poor-quality wires (bad contacts)
3. **Breadboard recommended**: For prototyping without soldering
4. **Document your connections**: Make a diagram or take a photo before disconnecting everything

### ⚡ Electrical Safety

- ⚠️ **Never connect 5V to a GPIO pin** (unless explicitly compatible)
- ⚠️ **Always connect GND to GND** (common ground between ESP32 and peripherals)
- ⚠️ **Verify module power**: 3.3V or 5V? (see component datasheet)

### 🐛 Quick Troubleshooting

| Problem | Likely Cause | Solution |
|---------|--------------|----------|
| OLED doesn't turn on | Wrong I2C address | Test `0x3C` then `0x3D` in `config.h` |
| TFT stays black | Backlight not powered | Verify GPIO 7 (S3) or GPIO 32 (Classic) connection |
| TFT displays garbage | Wrong SPI wiring | Recheck MOSI, SCLK, CS, DC, RST |
| Nothing works | Insufficient power | Use quality USB cable or external power |
| Crashes at startup | Short circuit | Unplug everything and reconnect one component at a time |

### 📚 Useful Resources

- **I2C Scanner**: Use Arduino "I2C Scanner" sketch to detect your OLED address
- **Datasheets**: Always consult official documentation for your modules
- **Forums**: ESP32.com, Arduino Forum, Reddit r/esp32

---

## 🎓 Configuration Summary in Code

### File `include/config.h`

To **enable** a display, uncomment the corresponding line:

```cpp
// OLED
#define HAS_OLED          // Uncomment to enable OLED SSD1306

// TFT ST7789 (ESP32-S3 only in this config)
#define HAS_ST7789        // Uncomment to enable ST7789
```

### File `include/board_config.h`

GPIO numbers are **already configured** in this file according to your board.
**You normally don't need to modify them** unless using custom wiring.

---

*For more details, see [README.md](../README.md) and [ARCHITECTURE.md](ARCHITECTURE.md)*
