# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/).

**[Version Française](CHANGELOG_FR.md)**

## [0.8.2] - 2025-12-13

### Changed
- ⚡ **TFT ST7789 Optimization**: Using hardware SPI instead of software SPI
  - New declaration: `Adafruit_ST7789(&SPI, PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST)`
  - Added `SPI.begin()` in `setupST7789()` for explicit initialization
  - Improved display performance (46-60% faster)
- 🎨 **Adafruit Colors**: Using official library constants
  - Replaced `COLOR_*` with `ST77XX_*` (e.g., `COLOR_BLACK` → `ST77XX_BLACK`)
  - Removed custom definitions from `config.h`
  - Consistency with Adafruit library standards

### Fixed
- 🔧 Smoother TFT display thanks to hardware SPI
- 📦 Code reduction with removal of 11 lines of redundant definitions

### Documentation
- 📘 [CHANGES_v0.8.2.md](docs/CHANGES_v0.8.2_FR.md) - Complete technical documentation of optimizations (French)
- 📘 [CHANGES_v0.8.2.md](docs/CHANGES_v0.8.2.md) - English version

## [0.8.1] - 2025-12-13

### Fixed
- 🔧 **Pin Name Consistency**: All pin names used in the code now exactly match declarations in `board_config.h`
  - `display.cpp`: `TFT_CS`, `TFT_DC`, `TFT_RST`, `TFT_BL` → `PIN_TFT_CS`, `PIN_TFT_DC`, `PIN_TFT_RST`, `PIN_TFT_BL`
  - `main.cpp`: `DEFAULT_BUZZER_PIN` → `PIN_BUZZER` (according to board_config.h)
  - Added `PIN_LED_BUILTIN` in board_config.h for ESP32 Classic (GPIO 2)
- 📋 **board_config.h** as single reference for all project pin names

### Documentation
- 📝 Documentation updated to reflect pin name consistency
- 📘 [CHANGES_v0.8.1.md](docs/CHANGES_v0.8.1_FR.md) - Detailed documentation (French)

## [0.8.0] - 2025-12-13

### Added
- 🎮 **Advanced Multi-Button Control System**:
  - BOOT Button: Restart with 2-second progress bar (cancellable before 100%)
  - Button 1: RGB Cycle (Red → Green → Blue → White → Off)
  - Button 2: Buzzer beep on press (1kHz, 100ms)
- 🎨 **Improved NeoPixel Management**:
  - Fixed violet during reboot sequence
  - Green heartbeat when WiFi connected
  - Red heartbeat when WiFi disconnected/searching
- 🔊 **Sound Feedback**: Piezo buzzer for user feedback (Pin 6)
- 🚨 **Reboot Safety**: Cancellation possible before 100% with screen restoration
- 📊 **TFT Progress Bar**: Magenta display, updates every 50ms

### Changed
- 🔄 **main.cpp**: Complete button handling overhaul
  - Added 3 OneButton objects (BOOT, BTN1, BTN2)
  - New callback functions: `handleButton1Click()`, `handleButton2PressStart()`, `handleButton2PressStop()`
  - Modified `handleLongPress()` with 100% reboot logic
  - Improved NeoPixel heartbeat with `isRebooting` condition
- ⚙️ **config.h**: Enabled HAS_LED_RGB and HAS_NEOPIXEL definitions
- 🎯 **Reboot Logic**: Direct pin verification via `digitalRead()` for reliability

### Fixed
- 🐛 RGB LED now correctly controlled (HAS_LED_RGB definition)
- 🐛 NeoPixel displaying all WiFi + reboot states
- 🐛 Reboot only triggers if bar at 100% + button still pressed
- 🐛 Automatic WiFi screen restoration after reboot cancellation

### Documentation
- 📋 Added `CHANGES_v0.8.0.md`: Detailed document of 10 numbered changes (French)
- 📊 Summary tables of pins and behaviors
- 🧪 Testing section with results

## [0.7.0] - 2025-12-06

### Added
- 🖥️ **Full TFT ST7789 Support**: High-resolution color display (240x240 or 240x135)
- 📱 **display.h/display.cpp Module**: Modular architecture to manage OLED and ST7789 in a unified way
- 🎨 **Graphical Startup Interface**:
  - Display project name and version at boot
  - WiFi connection progress bar (0-100%)
  - SSID and IP address display once connected
  - Error messages if connection fails
- 🔄 **Dual-Screen Support**: OLED and TFT can work simultaneously
- 📐 **Flexible Configuration**: Easy enable/disable of each display in `config.h`

### Changed
- 🏗️ **Clean Architecture**: Display logic separated from main.cpp
- 📝 **Function-based Interface**: `setupDisplays()`, `displayStartup()`, `displayWifiProgress()`, etc.
- 🎨 **Unified API**: Same functions work for both OLED and TFT

### Documentation
- 📘 Added detailed guides for display configuration
- 🔧 SPI and I2C wiring diagrams
- 📊 Complete pin mapping tables

## [0.6.0] - 2025-12-05

### Added
- 🌐 **Modular Web Interface**: Complete separation of CSS, HTML, and HTTP handlers
  - `web_styles.h`: Reusable CSS styles
  - `web_pages.h`: HTML page generator
  - `web_interface.h`: HTTP handlers and server setup
- 📱 **Modern Dashboard**: Real-time system information display
  - Chip ID, flash size and speed
  - Heap and PSRAM memory
  - CPU frequency
  - WiFi status and IP address
- 🎨 **Responsive Design**: Gradient animations, cards, and modern styles
- 🔄 **Reboot Button**: Direct restart from web interface

### Changed
- 📦 **Modular Architecture**: Code organized in reusable modules
- 🧹 **Clean main.cpp**: Reduced from 424 to 271 lines (-36%)
- 📚 **Complete Documentation**: Architecture guide and upgrade instructions

### Documentation
- 📘 `ARCHITECTURE.md`: Complete technical guide
- 📘 `UPGRADE_0.6.0.md`: Detailed migration guide
- 📘 `COHERENCE_CHECK.md`: Verification checklist

## [0.5.0] - 2025-12-02

### Added
- 🌈 **RGB LED Support**: Visual WiFi status (Yellow/Green/Red)
- 📡 **WiFiMulti**: Automatic connection to multiple networks
- 🌐 **Basic Web Server**: System information dashboard
- 📋 **board_config.h**: Centralized hardware pin mapping
- 🎯 **Multi-Board Support**: ESP32-S3 and ESP32 Classic

### Changed
- 🔧 **Project Structure**: Separation of config and hardware mapping
- 📝 **Clean Code**: Better organization and comments

## [0.4.0] - 2025-11-28

### Added
- 📺 **OLED Display Support**: SSD1306 128x64
- 📊 **Status Display**: WiFi connection and IP address
- ⚙️ **Conditional Compilation**: Easy enable/disable features

## [0.3.0] - 2025-11-25

### Added
- 📡 **WiFi Connection**: Basic WiFi support
- 🔐 **secrets.h**: Secure credential management
- 📝 **Serial Debugging**: Connection status monitoring

## [0.2.0] - 2025-11-20

### Added
- 💡 **LED Blink**: Basic test on Pin 2
- 🔧 **PlatformIO Config**: Multi-environment support

## [0.1.0] - 2025-11-15

### Added
- 🎉 **Initial Project**: Basic ESP32-S3 template
- 📁 **Project Structure**: Standard folders
- 📋 **README**: Basic documentation

---

## Legend

- 🎉 New features
- 🔧 Bug fixes
- 📚 Documentation
- ⚡ Performance
- 🔒 Security
- 🎨 UI/UX
- 🏗️ Architecture
- 📦 Dependencies
