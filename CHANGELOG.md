# Changelog

All notable changes to Base_ESP32_Multi will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [0.9.1] - 2026-01-03

### 🐛 Fixed

- **NeoPixel compilation error on ESP32 Classic**: Added automatic fallback mechanism in `main.cpp` that uses `NEOPIXEL_MATRIX` pin when `NEOPIXEL` is not defined in `board_config.h`. This fixes the "'NEOPIXEL' was not declared in this scope" error when compiling for `esp32devkitc` environment with `HAS_NEOPIXEL` enabled.

### 📝 Documentation

- **Enhanced code comments for beginners**:
  - Added comprehensive Doxygen header to `main.cpp` explaining the project structure
  - Added detailed NeoPixel fallback explanation with warning about GPIO 2 conflict on ESP32 Classic
  - Improved `config.h` with step-by-step NeoPixel activation instructions
  - Added pin mapping summary tables to both README.md and README_FR.md

### 🔄 Changed

- **Version consistency**: Updated PROJECT_VERSION to 0.9.1 across all files:
  - `platformio.ini`
  - `main.cpp`
  - `display.h` / `display.cpp`
  - `web_interface.h`
  - `web_pages.h`
  - `secrets_exemple.h` / `secrets_exemple_FR.h`
  - `README.md` / `README_FR.md`

### ⚙️ Technical Details

**Root Cause Analysis:**
- `board_config.h` defines `NEOPIXEL` (GPIO 48) only for ESP32-S3
- For ESP32 Classic, only `NEOPIXEL_MATRIX` (GPIO 2) was defined
- When `HAS_NEOPIXEL` was enabled in `config.h` for ESP32 Classic, compilation failed

**Solution:**
- Added preprocessor fallback in `main.cpp`:
  ```cpp
  #ifdef HAS_NEOPIXEL
      #ifndef NEOPIXEL
          #ifdef NEOPIXEL_MATRIX
              #define NEOPIXEL NEOPIXEL_MATRIX
          #else
              #error "No NeoPixel pin defined"
          #endif
      #endif
  #endif
  ```

**Files Modified:**
- `src/main.cpp` (NeoPixel fallback + Doxygen header)
- `include/config.h` (enhanced NeoPixel comments)
- `platformio.ini` (version update)
- `include/display.h` (version update)
- `src/display.cpp` (version update)
- `include/web_interface.h` (version update)
- `include/web_pages.h` (version update)
- `include/secrets_exemple.h` (version update)
- `include/secrets_exemple_FR.h` (version update)
- `README.md` (version + pin mapping tables)
- `README_FR.md` (version + pin mapping tables)
- `CHANGELOG.md` (this file)
- `CHANGELOG_FR.md` (French version)

---

## [0.9.0] - 2026-01-03

### 🚀 Added

#### WiFi System
- **Multi-network WiFi configuration with individual variables** (#1)
  - New format: `WIFI_SSID1`/`WIFI_PASS1`, `WIFI_SSID2`/`WIFI_PASS2`, etc.
  - Replaces old array-based `WIFI_NETWORKS[][2]` format
  - Up to 4 networks supported by default (easily extendable)
  - Created `secrets_exemple.h` (EN) and `secrets_exemple_FR.h` (FR) templates
  - Automatic connection to strongest available network
  - Automatic failover between networks

#### OTA Updates
- **ArduinoOTA network-based updates** (#1)
  - OTA updates via Arduino IDE/PlatformIO
  - Port 3232 (standard ArduinoOTA port)
  - Visual feedback on TFT display during OTA
  - NeoPixel color indicators (Blue = updating, Green = success, Red = error)
  - Progress bar with percentage and estimated time
  - Detailed error handling and logging

- **Web-based OTA interface** (#2)
  - New `/ota` page for browser-based firmware uploads
  - Drag-and-drop `.bin` file upload support
  - Real-time progress tracking with XMLHttpRequest
  - File validation (`.bin` extension check)
  - Inline status messages (no popups)
  - Auto-redirect after successful upload
  - Complete error handling with user-friendly messages

#### Hardware Support
- **NeoPixel Matrix 8×8 GPIO assignment** (#1)
  - ESP32-S3: GPIO 3 for NeoPixel Matrix (WS2812B-64)
  - ESP32 Classic: GPIO 2 for NeoPixel Matrix (⚠️ conflicts with LED_BUILTIN)
  - Documentation for 64-LED matrix power requirements (3A @ 5V)
  - Example code compatible with Adafruit_NeoMatrix library

#### Documentation
- **Complete beginner-friendly guides (EN + FR):**
  - `docs/GETTING_STARTED.md` / `docs/GETTING_STARTED_FR.md`
  - `docs/WIFI_SETUP.md` / `docs/WIFI_SETUP_FR.md`
  - `docs/OTA_UPDATE.md` / `docs/OTA_UPDATE_FR.md`
  - `docs/HARDWARE_SETUP.md` / `docs/HARDWARE_SETUP_FR.md`
- Comprehensive troubleshooting sections
- Wiring diagrams and safety guidelines
- GPIO pinout tables for all supported boards

### 🔄 Changed

#### Display System
- **Renamed ST7789 constants to generic TFT naming** (#1)
  - `ST7789_WIDTH` → `TFT_WIDTH`
  - `ST7789_HEIGHT` → `TFT_HEIGHT`
  - `ST7789_ROTATION` → `TFT_ROTATION`
  - Updated all references in `config.h`, `display.cpp`, `main.cpp`
  - Rationale: Supports multiple TFT display types (ST7789, ILI9341, ST7735, etc.)

#### Web Interface
- **Replaced confirm() popups with inline validations** (#2)
  - Reboot button now shows inline status messages
  - OTA upload uses progress bars and status divs
  - Better user experience (no blocking dialogs)
  - Status messages: info (blue), success (green), error (red), warning (orange)

#### Version Management
- **Updated PROJECT_VERSION to 0.9.0** across all files:
  - `platformio.ini`: Build flags and comments
  - `display.h`: File header
  - `display.cpp`: File header and date (2026-01-03)
  - `web_pages.h`: File header

### 🐛 Fixed

- **OTA page functions** (#2)
  - Added missing `generateOTAPage()` function to `web_pages.h`
  - Added missing `generateOTAResultPage()` function to `web_pages.h`
  - Fixed compilation errors related to undeclared OTA functions

- **WiFi configuration** (#2)
  - Created `secrets.h` template from `secrets_exemple.h`
  - Fixed "WIFI_SSID not declared" compilation errors
  - Added documentation for creating `secrets.h` locally

### 📝 Documentation

- **New comprehensive guides** (bilingual EN/FR):
  - Getting Started: Installation, first-time setup, verification
  - WiFi Setup: Multi-network config, security, troubleshooting
  - OTA Updates: Both web and network OTA methods
  - Hardware Setup: GPIO pinouts, wiring guides, safety practices

- **Updated README** (EN/FR):
  - Version 0.9.0 features
  - Quick start guide
  - Links to new documentation

### 🔒 Security

- **WiFi credentials isolation:**
  - `secrets.h` added to `.gitignore`
  - Never committed to version control
  - Template files provided for easy setup

- **OTA security documentation:**
  - Password protection examples
  - Network security best practices
  - Warnings about open network usage

### ⚙️ Technical Details

**Commits:**
- aa70c5b - feat: Major refactor v0.9.0 - WiFi system, OTA, TFT naming, NeoPixel Matrix
- 87cecc2 - fix: Complete v0.9.0 implementation - OTA pages + version update + build fixes

**Files Modified:**
- `include/secrets_exemple.h` (created)
- `include/secrets_exemple_FR.h` (created)
- `include/board_config.h` (NeoPixel Matrix GPIO added)
- `include/config.h` (ST7789 → TFT renaming)
- `include/web_interface.h` (OTA handlers added)
- `include/web_pages.h` (OTA page generators added, inline validations)
- `include/display.h` (version updated)
- `src/main.cpp` (WiFi refactor, ArduinoOTA integration)
- `src/display.cpp` (TFT constants updated, version/date updated)
- `platformio.ini` (version updated to 0.9.0)
- `docs/` (8 new documentation files)

**Lines Changed:**
- +616 insertions
- -69 deletions
- 11 files modified

---

## [0.8.2-rc.1] - Previous Release

See git history for details of previous versions.

---

## Legend

- 🚀 **Added**: New features
- 🔄 **Changed**: Changes in existing functionality
- 🐛 **Fixed**: Bug fixes
- 🗑️ **Deprecated**: Soon-to-be removed features
- ❌ **Removed**: Removed features
- 🔒 **Security**: Security improvements
- 📝 **Documentation**: Documentation changes

---

## Versioning

This project follows [Semantic Versioning](https://semver.org/):
- **MAJOR** version (X.0.0): Incompatible API changes
- **MINOR** version (0.X.0): New functionality in a backwards-compatible manner
- **PATCH** version (0.0.X): Backwards-compatible bug fixes

**v0.9.0 Classification:**
- MINOR version bump (0.8 → 0.9)
- Reason: New features (multi-network WiFi, OTA, NeoPixel Matrix) added
- Backwards compatibility: Mostly compatible, but requires `secrets.h` migration

---

## Migration Guide: 0.8.x → 0.9.0

### Breaking Changes

1. **WiFi Configuration Format:**
   - **Old format (removed):**
     ```cpp
     const char* WIFI_NETWORKS[][2] = {
         {"SSID1", "PASS1"},
         {"SSID2", "PASS2"}
     };
     ```
   - **New format (required):**
     ```cpp
     const char* WIFI_SSID1 = "SSID1";
     const char* WIFI_PASS1 = "PASS1";
     const char* WIFI_SSID2 = "SSID2";
     const char* WIFI_PASS2 = "PASS2";
     ```

2. **secrets.h File:**
   - Must create `include/secrets.h` from template:
     ```bash
     cp include/secrets_exemple.h include/secrets.h
     ```
   - Edit with your WiFi credentials

3. **Display Constants (optional migration):**
   - Old `ST7789_*` constants still work but deprecated
   - Recommended: Update custom code to use `TFT_*` constants

### New Features to Enable

1. **OTA Updates:**
   - Already enabled by default (ArduinoOTA + Web OTA)
   - Access web interface: `http://<ESP32-IP>/ota`
   - Optional: Add password protection (see OTA_UPDATE.md)

2. **NeoPixel Matrix:**
   - Uncomment in `board_config.h`:
     ```cpp
     #define HAS_NEOPIXEL_MATRIX
     ```
   - Wire to GPIO 3 (ESP32-S3) or GPIO 2 (ESP32 Classic)

---

For detailed changes, see the [commit history](https://github.com/morfredus/Base_ESP32_Multi/commits/main).
