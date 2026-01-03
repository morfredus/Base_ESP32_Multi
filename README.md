# Base_ESP32_Multi

Universal starter template for ESP32 and ESP32-S3 projects on PlatformIO.
Includes automatic WiFi management, multi-environment structure, and support for status LEDs (NeoPixel), OLED displays, and color TFT screens.

**Current version: 0.9.1**

**[Version Française](README_FR.md)**

## 🚀 Features

* **Multi-Board Support**: Ready for ESP32-S3 (N16R8, N8R8) and ESP32 Classic (DevKitC).
* **Three PlatformIO Environments**:
  - `esp32s3_n16r8`: ESP32-S3 with 16MB Flash / 8MB PSRAM (primary)
  - `esp32s3_n8r8`: ESP32-S3 with 8MB Flash / 8MB PSRAM
  - `esp32devkitc`: ESP32 Classic with 4MB Flash
* **WiFiMulti**: Attempts to connect to a list of known networks (up to 4 networks).
* **OTA Updates** *(v0.9.0)*:
  - ArduinoOTA network-based updates (port 3232)
  - Web-based OTA interface at `/ota`
* **Modular Web Server** *(v0.6.0)*: Modern web interface with modular architecture - separate CSS styles, flexible HTML generator, organized handlers. Accessible at http://[ESP32_IP].
* **Security**: WiFi credentials are excluded from Git (`secrets.h`).
* **Visual Feedback**: Automatic RGB LED management (Pin 48 on S3, Pin 2 on Classic via fallback) to indicate WiFi status (Yellow=Connecting, Green=OK, Red=Error).
* **Dual-Screen Display** *(v0.7.0)*: Simultaneous support for OLED SSD1306 (128x64) and TFT ST7789 (240x240 color) with unified interface.
  - Display project name and version at startup
  - WiFi connection progress bar
  - SSID and IP address once connected
* **Advanced Button System** *(v0.8.0)*:
  - **BOOT Button**: Reboot with 2-second progress bar (cancellable before 100%)
  - **Button 1**: RGB Cycle (Red → Green → Blue → White → Off)
  - **Button 2**: Buzzer beep on press (sound feedback)
* **Smart NeoPixel** *(v0.8.0+)*:
  - Fixed violet during restart
  - Green heartbeat when WiFi connected
  - Red heartbeat when WiFi searching/disconnected
  - Automatic fallback to NEOPIXEL_MATRIX pin on ESP32 Classic
* **NeoPixel Matrix 8×8 Support** *(v0.9.0)*:
  - ESP32-S3: GPIO 3
  - ESP32 Classic: GPIO 2 (⚠️ shared with LED_BUILTIN)
* **Clean Structure**: Separation of project config (`config.h`), hardware mapping (`board_config.h`), and display management (`display.h`).

## 🛠️ Installation & Getting Started

### 1. Clone the Project
Get this folder and rename it according to your new project.

### 2. Create `secrets.h` File
The `include/secrets.h` file is ignored by Git. You must create it manually:

```cpp
// include/secrets.h
#ifndef SECRETS_H
#define SECRETS_H

const char* WIFI_NETWORKS[][2] = {
    {"Home_SSID", "Password123"},
    {"Office_SSID", "Password456"},
    {"Phone_Hotspot", "12345678"}
};

#endif
```

### 3. Configure Displays (Optional)
OLED and ST7789 displays are enabled by default in `include/config.h`.

#### 📍 Display Connections

For detailed connection diagrams and complete GPIO tables, see:
**→ [docs/PIN_MAPPING.md](docs/PIN_MAPPING.md)** 

Complete guide including:
- GPIO summary tables for ESP32-S3 and ESP32 Classic
- Wiring diagrams (ASCII art)
- Troubleshooting tips
- I2C vs SPI configuration

#### ⚙️ Software Configuration

In `include/config.h`, you can enable/disable displays:

```cpp
// OLED SSD1306 (I2C)
#define HAS_OLED          // Uncomment to enable

// TFT ST7789 (SPI color)
#define HAS_ST7789        // Uncomment to enable
```

**Note**: You can use both displays simultaneously!

#### Automatic Display
- **At startup**: Project name and version
- **WiFi connection**: Progress bar (0-100%)
- **Connected**: Network SSID and assigned IP address
- **Error**: Connection failure message

To disable a display, simply comment out its `#define` line in `config.h`.

## 🏗️ Modular Architecture (v0.6.0+)

The project uses a modular architecture for the web interface, separating responsibilities:

### Web Modules

| Module | File | Responsibility |
|--------|------|----------------|
| **Styles** | `include/web_styles.h` | Contains all reusable CSS (flexbox, gradients, animations) |
| **Pages** | `include/web_pages.h` | HTML generator with `generateDashboardPage()` function |
| **Interface** | `include/web_interface.h` | HTTP handlers and web server configuration |

### Display Modules (v0.7.0+)

| Module | File | Responsibility |
|--------|------|----------------|
| **Display** | `include/display.h` | Unified interface for OLED and ST7789 |
| **Display Impl** | `src/display.cpp` | Implementation of display functions |

### Advantages of This Architecture

- ✅ **Modularity**: Each module has a single responsibility
- ✅ **Reusability**: Styles and generators can be used in other projects
- ✅ **Maintainability**: Easy modifications without affecting the rest of the code
- ✅ **Testability**: Each module can be tested independently
- ✅ **Documentation**: Doxygen code for all modules

### Usage Example

```cpp
// main.cpp
#include "display.h"  // Unified display interface

// Setup
setupDisplays();      // Initialize OLED and/or ST7789
displayStartup(PROJECT_NAME, PROJECT_VERSION);  // Startup screen

// Loop
displayWifiProgress(progress);  // During WiFi connection
displayWifiConnected(ssid, ip);  // Once connected
```

**Learn more**: See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)

## 📋 Changelog

For complete version history and modifications, see [CHANGELOG.md](CHANGELOG.md) or [CHANGELOG_FR.md](CHANGELOG_FR.md) (French).

**Technical documentation**: See [docs/](docs/) for detailed guides and technical references.

### Current Version: v0.9.1 (2026-01-03)

**What's New in v0.9.1 (Patch):**
- 🐛 **Fixed NeoPixel compilation error on ESP32 Classic**: Added automatic fallback to `NEOPIXEL_MATRIX` when `NEOPIXEL` is not defined
- 📝 **Enhanced code comments**: Added beginner-friendly documentation in `main.cpp` and `config.h`
- 📋 **Version consistency**: All files now correctly reference v0.9.1

**What's New in v0.9.0:**
- 🌐 **Multi-network WiFi**: Support for up to 4 networks with automatic failover
- 📡 **OTA Updates**: ArduinoOTA (port 3232) + Web-based OTA at `/ota`
- 🔲 **NeoPixel Matrix 8×8**: GPIO support added (GPIO 3 on S3, GPIO 2 on Classic)
- 📚 **Complete documentation**: Bilingual guides (EN/FR) for beginners

**What's New in v0.8.x:**
- ⚡ **Hardware SPI for TFT** (v0.8.2)
- 🎮 **Advanced Button System**: BOOT (reboot), BTN1 (RGB), BTN2 (Buzzer) (v0.8.0)
- 🎨 **Smart NeoPixel**: Indicates WiFi and reboot status (v0.8.0)

## 📚 Documentation

All documentation is available in the `docs/` folder in both French and English:
- [docs/INDEX.md](docs/INDEX.md) - Documentation index
- [docs/PIN_MAPPING.md](docs/PIN_MAPPING.md) - Complete pin mapping guide
- [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) - Software architecture

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues.

## 📜 License

This project is licensed under the MIT License - see the LICENSE file for details.

---

**Project Version**: 0.9.1
**Last Update**: January 3, 2026
**Compatible Boards**: ESP32-S3 DevKitC-1 (N16R8, N8R8), ESP32 Classic DevKitC

## 📌 Pin Mapping Summary

### ESP32-S3 DevKitC-1

| Function | GPIO | Notes |
|----------|------|-------|
| NeoPixel (single) | 48 | Addressable RGB LED |
| NeoPixel Matrix 8×8 | 3 | WS2812B-64 |
| TFT (SPI) | CS=10, DC=9, RST=14, BL=7 | ST7789 240x240 |
| I2C (OLED) | SDA=15, SCL=16 | SSD1306 128x64 |
| Buttons | BOOT=0, BTN1=38, BTN2=39 | Active LOW |
| Buzzer | 6 | PWM capable |
| LED RGB | R=21, G=41, B=42 | Standard LEDs |

### ESP32 Classic DevKitC

| Function | GPIO | Notes |
|----------|------|-------|
| NeoPixel Matrix 8×8 | 2 | ⚠️ Shared with LED_BUILTIN |
| TFT (SPI) | CS=27, DC=14, RST=25, BL=32 | ST7789 |
| I2C (OLED) | SDA=21, SCL=22 | SSD1306 |
| Buttons | BOOT=0, BTN1=5, BTN2=12 | Active LOW |
| Buzzer | 19 | PWM capable |
| LED RGB | R=13, G=26, B=33 | Standard LEDs |

For complete pin mapping, see [docs/PIN_MAPPING.md](docs/PIN_MAPPING.md).
