# Base_ESP32_S3

Universal starter template for ESP32 and ESP32-S3 projects on PlatformIO.
Includes automatic WiFi management, multi-environment structure, and support for status LEDs (NeoPixel), OLED displays, and color TFT screens.

**Current version: 0.8.2**

**[Version Française](README_FR.md)**

## 🚀 Features
* **Multi-Board Support**: Ready for ESP32-S3 (N16R8, N8R8) and ESP32 Classic (DevKitC).
* **WiFiMulti**: Attempts to connect to a list of known networks.
* **Modular Web Server** *(v0.6.0)*: Modern web interface with modular architecture - separate CSS styles, flexible HTML generator, organized handlers. Accessible at http://[ESP32_IP].
* **Security**: WiFi credentials are excluded from Git (`secrets.h`).
* **Visual Feedback**: Automatic RGB LED management (Pin 48 on S3) to indicate WiFi status (Yellow=Connecting, Green=OK, Red=Error).
* **Dual-Screen Display** *(v0.7.0)*: Simultaneous support for OLED SSD1306 (128x64) and TFT ST7789 (240x240 color) with unified interface.
  - Display project name and version at startup
  - WiFi connection progress bar
  - SSID and IP address once connected
* **Advanced Button System** *(v0.8.0)*:
  - **BOOT Button**: Reboot with 2-second progress bar (cancellable before 100%)
  - **Button 1**: RGB Cycle (Red → Green → Blue → White → Off)
  - **Button 2**: Buzzer beep on press (sound feedback)
* **Smart NeoPixel** *(v0.8.0)*: 
  - Fixed violet during restart
  - Green heartbeat when WiFi connected
  - Red heartbeat when WiFi searching/disconnected
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

### Current Version: v0.8.2 (2025-12-13)
**What's New in v0.8.2 (Optimization):**
- ⚡ **Hardware SPI for TFT**: Using hardware SPI for better performance
- 🎨 **Adafruit Colors**: Replaced `COLOR_*` with official `ST77XX_*` constants

**What's New in v0.8.1:**
- 🔧 **Pin Name Consistency**: All pin names normalized with `PIN_` prefix
- 📋 **board_config.h as single reference** for all project pin names

**What's New in v0.8.0:**
- 🎮 **Advanced Button System**: BOOT (reboot), BTN1 (RGB), BTN2 (Buzzer)
- 🎨 **Smart NeoPixel**: Indicates WiFi and reboot status
- 🔊 **Sound Feedback**: Piezo buzzer

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

**Project Version**: 0.8.2  
**Last Update**: December 13, 2025  
**Compatible Boards**: ESP32-S3 DevKitC-1 (N16R8, N8R8), ESP32 Classic DevKitC
