# 📚 Documentation Index - v0.8.2

**Last update**: December 13, 2025  
**Project version**: 0.8.2

**[Version Française](INDEX_FR.md)** | 🌍 [Bilingual Structure](BILINGUAL_STRUCTURE.md)

---

## 🚀 Quick Start

| Document | Description | For whom? |
|----------|-------------|-----------|
| [README.md](../README.md) | Project overview | 👤 Everyone |
| [platformio.ini](../platformio.ini) | PlatformIO configuration | 🔧 Developers |
| [docs/PIN_MAPPING.md](PIN_MAPPING.md) | Detailed wiring guide | 🎓 Beginners |

---

## 📋 Documentation by Version

### Version 0.8.2 (Current)
- 📘 [CHANGES_v0.8.2.md](CHANGES_v0.8.2.md) - TFT optimization and Adafruit colors (English)
- 📘 [CHANGES_v0.8.2_FR.md](CHANGES_v0.8.2_FR.md) - Version française
- 📘 [UPGRADE_0.8.2.md](UPGRADE_0.8.2.md) - Upgrade guide (English)
- 📘 [UPGRADE_0.8.2_FR.md](UPGRADE_0.8.2_FR.md) - Guide en français
- 📘 [COHERENCE_v0.8.2.md](COHERENCE_v0.8.2.md) - Consistency verification
- 📘 [SUMMARY_0.8.2.md](SUMMARY_0.8.2.md) - Complete summary

### Version 0.8.1
- 📘 [CHANGES_v0.8.1_FR.md](CHANGES_v0.8.1_FR.md) - Technical details (French)
- 📘 [UPGRADE_0.8.1_FR.md](UPGRADE_0.8.1_FR.md) - Upgrade guide (French)
- 📘 [RELEASE_0.8.1_FR.md](RELEASE_0.8.1_FR.md) - Complete release package (French)

### Version 0.8.0
- 📘 [CHANGES_v0.8.0_FR.md](CHANGES_v0.8.0_FR.md) - Multi-button system (French)
- 📘 [UPGRADE_0.8.0_FR.md](UPGRADE_0.8.0_FR.md) - Migration to v0.8.0 (French)
- 📘 [SYNTHESIS_0.8.0_FR.md](SYNTHESIS_0.8.0_FR.md) - Technical synthesis (French)

### Version 0.7.0
- 📘 [IMPLEMENTATION_SUMMARY.md](IMPLEMENTATION_SUMMARY.md) - Implementation summary

### Version 0.6.0
- 📘 [UPGRADE_0.6.0_FR.md](UPGRADE_0.6.0_FR.md) - Modular web server (French)

---

## 🏗️ Architecture and References

| Document | Content | Level |
|----------|---------|-------|
| [ARCHITECTURE.md](ARCHITECTURE.md) | Project structure | 🔵 Intermediate |
| [COHERENCE_CHECK.md](COHERENCE_CHECK.md) | Consistency verification | 🟢 All |
| [PIN_MAPPING.md](PIN_MAPPING.md) | Detailed GPIO mapping | 🟢 Beginner |
| [SYNTHESIS.md](SYNTHESIS.md) | Technical synthesis | 🔴 Advanced |

---

## 📝 History and Changes

| Document | Description |
|----------|-------------|
| [CHANGELOG.md](../CHANGELOG.md) | Complete version history (English) |
| [CHANGELOG_FR.md](../CHANGELOG_FR.md) | Historique complet (Français) |
| [RELEASE_0.8.2.md](RELEASE_0.8.2.md) | Release notes v0.8.2 |
| [CHANGES_v0.8.2.md](CHANGES_v0.8.2.md) | TFT optimization and colors (v0.8.2) |
| [CHANGES_v0.8.1_FR.md](CHANGES_v0.8.1_FR.md) | Pin name consistency (v0.8.1) |
| [CHANGES_v0.8.0_FR.md](CHANGES_v0.8.0_FR.md) | Multi-button system (v0.8.0) |

---

## 🔧 Configuration and Code

### Configuration Files
| File | Description | Modify? |
|------|-------------|---------|
| [platformio.ini](../platformio.ini) | PlatformIO config | ⚠️ Rarely |
| [include/config.h](../include/config.h) | Project config | ✅ Often |
| [include/board_config.h](../include/board_config.h) | Pin mapping | ⚠️ With caution |
| [include/secrets.h](../include/secrets.h) | WiFi credentials | ✅ To create |

### Source Files
| File | Role | Complexity |
|------|------|------------|
| [src/main.cpp](../src/main.cpp) | Main logic | 🟡 Medium |
| [src/display.cpp](../src/display.cpp) | Display management | 🟢 Simple |
| [include/display.h](../include/display.h) | Display interface | 🟢 Simple |
| [include/web_interface.h](../include/web_interface.h) | Web server | 🟡 Medium |
| [include/web_pages.h](../include/web_pages.h) | HTML pages | 🟢 Simple |
| [include/web_styles.h](../include/web_styles.h) | CSS styles | 🟢 Simple |

---

## 🎯 By Use Case

### I'm new to ESP32
1. 📗 [README.md](../README.md) - General introduction
2. 📗 [PIN_MAPPING.md](PIN_MAPPING.md) - Component wiring
3. 📗 [include/board_config.h](../include/board_config.h) - Available pins

### I want to customize the project
1. 📘 [include/config.h](../include/config.h) - Enable/disable features
2. 📘 [ARCHITECTURE.md](ARCHITECTURE.md) - Understand the structure
3. 📘 [src/main.cpp](../src/main.cpp) - Modify the logic

### I want to add a display
1. 📗 [PIN_MAPPING.md](PIN_MAPPING.md) - Physical connections
2. 📗 [include/config.h](../include/config.h) - Enable `HAS_OLED` or `HAS_ST7789`
3. 📗 [ARCHITECTURE.md](ARCHITECTURE.md) - Display module usage

### I want to understand the web interface
1. 📘 [ARCHITECTURE.md](ARCHITECTURE.md) - Modular architecture
2. 📘 [include/web_interface.h](../include/web_interface.h) - HTTP handlers
3. 📘 [include/web_pages.h](../include/web_pages.h) - Page generation
4. 📘 [include/web_styles.h](../include/web_styles.h) - CSS styles

### I'm migrating from an older version
1. 📙 [CHANGELOG.md](../CHANGELOG.md) - What's changed
2. 📙 [UPGRADE_0.8.2.md](UPGRADE_0.8.2.md) - Migration guide v0.8.2 (if available in English)
3. 📙 [UPGRADE_0.8.2_FR.md](UPGRADE_0.8.2_FR.md) - Guide de migration v0.8.2 (Français)
4. 📙 [COHERENCE_CHECK.md](COHERENCE_CHECK.md) - Verify consistency

---

## 🔍 By Component

### WiFi
- Configuration: [include/config.h](../include/config.h) - Network list
- Credentials: [include/secrets.h](../include/secrets.h) - SSID/Password
- Logic: [src/main.cpp](../src/main.cpp) - WiFiMulti connection

### OLED Display
- Configuration: [include/config.h](../include/config.h) - `HAS_OLED`
- Pins: [include/board_config.h](../include/board_config.h) - `PIN_I2C_SDA`, `PIN_I2C_SCL`
- Wiring: [PIN_MAPPING.md](PIN_MAPPING.md) - I2C connection
- Usage: [src/display.cpp](../src/display.cpp) - Display functions

### TFT ST7789 Display
- Configuration: [include/config.h](../include/config.h) - `HAS_ST7789`
- Pins: [include/board_config.h](../include/board_config.h) - SPI pins
- Wiring: [PIN_MAPPING.md](PIN_MAPPING.md) - SPI connection
- Usage: [src/display.cpp](../src/display.cpp) - Display functions

### RGB LED
- Configuration: [include/config.h](../include/config.h) - `HAS_LED_RGB`
- Pins: [include/board_config.h](../include/board_config.h) - `PIN_LED_RED/GREEN/BLUE`
- Usage: [src/main.cpp](../src/main.cpp) - WiFi status

### NeoPixel
- Configuration: [include/config.h](../include/config.h) - `NEOPIXEL_NUM`
- Pin: [include/board_config.h](../include/board_config.h) - `PIN_NEOPIXEL`
- Usage: [src/main.cpp](../src/main.cpp) - Heartbeat

### Sensors
- Available pins: [include/board_config.h](../include/board_config.h)
- Wiring: [PIN_MAPPING.md](PIN_MAPPING.md)
- Safety: [include/board_config.h](../include/board_config.h) - Safety reminders

---

## 📊 Document Structure

```
Project/
├── 📄 README.md ............................ Overview
├── 📄 CHANGELOG.md ......................... Complete history
├── 📄 platformio.ini ....................... PlatformIO config
│
├── include/
│   ├── 📄 config.h ......................... Project config
│   ├── 📄 board_config.h ................... Pin mapping
│   ├── 📄 display.h ........................ Display interface
│   ├── 📄 web_interface.h .................. Web server
│   ├── 📄 web_pages.h ...................... HTML pages
│   ├── 📄 web_styles.h ..................... CSS styles
│   └── 📄 secrets.h ........................ WiFi (to create)
│
├── src/
│   ├── 📄 main.cpp ......................... Main logic
│   └── 📄 display.cpp ...................... Display management
│
└── docs/
    ├── 📄 INDEX.md ......................... This file
    ├── 📄 IMPLEMENTATION_SUMMARY.md ........ Implementation summary
    ├── 📄 ARCHITECTURE.md .................. Project structure
    ├── 📄 COHERENCE_CHECK.md ............... Verification
    ├── 📄 PIN_MAPPING.md ................... Wiring guide
    ├── 📄 CHANGES_v0.8.2.md ................ Details v0.8.2
    ├── 📄 CHANGES_v0.8.1_FR.md ............. Details v0.8.1 (FR)
    ├── 📄 CHANGES_v0.8.0_FR.md ............. Details v0.8.0 (FR)
    ├── 📄 RELEASE_0.8.2.md ................. Package v0.8.2
    ├── 📄 RELEASE_0.8.1_FR.md .............. Package v0.8.1 (FR)
    ├── 📄 UPGRADE_0.8.2.md ................. Guide v0.8.2
    ├── 📄 UPGRADE_0.8.1_FR.md .............. Guide v0.8.1 (FR)
    └── 📄 SUMMARY_0.8.2.md ................. Summary v0.8.2
```

---

## 💡 Reading Tips

### For Quick Reference
Start with [README.md](../README.md) then [PIN_MAPPING.md](PIN_MAPPING.md).

### For Deep Understanding
1. [ARCHITECTURE.md](ARCHITECTURE.md)
2. [IMPLEMENTATION_SUMMARY.md](IMPLEMENTATION_SUMMARY.md)
3. [SYNTHESIS.md](SYNTHESIS.md)

### For Updates
Check [CHANGELOG.md](../CHANGELOG.md) for the latest changes, then read the specific UPGRADE guide.

### For Troubleshooting
1. [COHERENCE_CHECK.md](COHERENCE_CHECK.md) - Verify your configuration
2. [PIN_MAPPING.md](PIN_MAPPING.md) - Check wiring
3. [README.md](../README.md) - Review setup

---

## 🔗 External Links

- **PlatformIO**: [platformio.org](https://platformio.org/)
- **ESP32 Arduino Core**: [github.com/espressif/arduino-esp32](https://github.com/espressif/arduino-esp32)
- **Adafruit Libraries**: [github.com/adafruit](https://github.com/adafruit)

---

## 📞 Getting Help

1. **Documentation**: Check this index
2. **Examples**: See [src/main.cpp](../src/main.cpp)
3. **Wiring**: Consult [PIN_MAPPING.md](PIN_MAPPING.md)
4. **Updates**: Read [CHANGELOG.md](../CHANGELOG.md)

---

**Last Version (0.8.2)**
- 📘 [RELEASE_0.8.2.md](RELEASE_0.8.2.md)
- 📘 [CHANGES_v0.8.2.md](CHANGES_v0.8.2.md)
- 📘 [UPGRADE_0.8.2.md](UPGRADE_0.8.2.md)
- 📘 [COHERENCE_v0.8.2.md](COHERENCE_v0.8.2.md)
- 📘 [SUMMARY_0.8.2.md](SUMMARY_0.8.2.md)

---

*Index created on December 13, 2025*  
*Project version: 0.8.2*
