# Getting Started with Base_ESP32_Multi

**Version:** 0.9.0
**Last Updated:** 2026-01-03

Welcome to Base_ESP32_Multi! This guide will help you get your ESP32 project up and running in minutes.

---

## 📋 Table of Contents

1. [Prerequisites](#prerequisites)
2. [Quick Start](#quick-start)
3. [First-Time Setup](#first-time-setup)
4. [Building and Uploading](#building-and-uploading)
5. [Verifying Installation](#verifying-installation)
6. [Next Steps](#next-steps)
7. [Troubleshooting](#troubleshooting)

---

## Prerequisites

Before you begin, make sure you have:

### Hardware
- ESP32-S3 DevKitC-1 (recommended) or ESP32 Classic DevKitC
- USB-C cable (for ESP32-S3) or Micro-USB cable (for ESP32 Classic)
- Computer with Windows, macOS, or Linux

### Software
- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO IDE extension](https://platformio.org/install/ide?install=vscode)
- Git (optional, for cloning the repository)

### Knowledge
- Basic understanding of Arduino/ESP32 programming
- Familiarity with command line (optional but helpful)

---

## Quick Start

### 1. Clone the Repository

```bash
git clone https://github.com/morfredus/Base_ESP32_Multi.git
cd Base_ESP32_Multi
```

Or download the ZIP file from GitHub and extract it.

### 2. Open in VS Code

```bash
code .
```

Or open VS Code and use **File → Open Folder** to select the project directory.

### 3. Install Dependencies

PlatformIO will automatically download all required libraries when you first build the project. No manual installation needed!

---

## First-Time Setup

### Step 1: Configure WiFi Credentials

**IMPORTANT:** You must create a `secrets.h` file with your WiFi credentials.

1. Navigate to the `include/` folder
2. Copy `secrets_exemple.h` to create `secrets.h`:

```bash
cp include/secrets_exemple.h include/secrets.h
```

3. Edit `include/secrets.h` with your WiFi information:

```cpp
// Network 1 (Primary)
const char* WIFI_SSID1 = "YourHomeNetwork";      // ← Replace with your WiFi name
const char* WIFI_PASS1 = "YourPassword123";      // ← Replace with your password

// Network 2 (Secondary - Optional)
const char* WIFI_SSID2 = "YourOfficeNetwork";    // ← Or leave empty ""
const char* WIFI_PASS2 = "OfficePassword456";    // ← Or leave empty ""

// Networks 3 & 4 (Optional)
const char* WIFI_SSID3 = "";                     // Leave empty if not needed
const char* WIFI_PASS3 = "";
const char* WIFI_SSID4 = "";
const char* WIFI_PASS4 = "";
```

**📌 Note:** The `secrets.h` file is gitignored and will never be committed to version control (for security).

For detailed WiFi configuration, see [WIFI_SETUP.md](WIFI_SETUP.md).

### Step 2: Select Your Board Configuration

The project supports multiple ESP32 boards. Edit `platformio.ini` and set the default environment:

```ini
[platformio]
default_envs = esp32s3_n16r8    ; ← Choose your board here
```

**Available environments:**
- `esp32s3_n16r8` - ESP32-S3 with 16MB Flash + 8MB PSRAM (recommended)
- `esp32s3_n8r8` - ESP32-S3 with 8MB Flash + 8MB PSRAM
- `esp32devkitc` - ESP32 Classic (4MB Flash, no PSRAM)

### Step 3: Configure Hardware Features

Edit `include/board_config.h` to enable/disable hardware features:

```cpp
// Enable the features your board has
#define HAS_LED_BUILTIN    // Built-in LED
#define HAS_LED_RGB        // RGB LED (if available)
#define HAS_OLED           // OLED SSD1306 display (if available)
#define HAS_ST7789         // ST7789 TFT display (if available)
#define HAS_NEOPIXEL       // NeoPixel/WS2812B strip (if available)
// etc.
```

For detailed hardware configuration, see [HARDWARE_SETUP.md](HARDWARE_SETUP.md).

---

## Building and Uploading

### Using PlatformIO GUI (Recommended for Beginners)

1. **Open PlatformIO Panel:**
   - Click the PlatformIO icon (alien head) in the VS Code sidebar
   - Or use: **View → Command Palette → PlatformIO: Home**

2. **Build the Project:**
   - Click **Build** in the PlatformIO toolbar (checkmark icon)
   - Or use the Project Tasks panel: **General → Build**

3. **Upload to ESP32:**
   - Connect your ESP32 via USB
   - Click **Upload** in the PlatformIO toolbar (arrow icon)
   - Or use: **General → Upload**

4. **Monitor Serial Output:**
   - Click **Monitor** in the PlatformIO toolbar (plug icon)
   - Or use: **General → Monitor**
   - Baud rate: 115200

### Using Command Line

```bash
# Build the project
pio run

# Upload to ESP32
pio run --target upload

# Open serial monitor
pio device monitor

# Build + Upload + Monitor (all in one)
pio run --target upload && pio device monitor
```

---

## Verifying Installation

### 1. Check Serial Output

After uploading, open the Serial Monitor (115200 baud). You should see:

```
=== Base_ESP32_Multi v0.9.0 ===
Chip ID: 0xABCDEF
Flash: 16MB @ 80MHz

--- Initializing Displays ---
✓ OLED initialized
✓ ST7789 initialized

--- Starting WiFiMulti ---
  - Network added: YourHomeNetwork
Connecting to WiFi...
✓ Connected to: YourHomeNetwork
IP Address: 192.168.1.100

Web server started on http://192.168.1.100
  - OTA page: http://192.168.1.100/ota

=== Setup Complete ===
```

### 2. Check Built-in LED

The built-in LED should blink (heartbeat pattern) every second, indicating the ESP32 is running.

### 3. Test Web Interface

1. Open a web browser
2. Go to the IP address shown in the serial monitor (e.g., `http://192.168.1.100`)
3. You should see the dashboard with system information

### 4. Test Buttons (if available)

- **BOOT Button (long press 2s):** Triggers reboot with progress bar
- **Button 1:** Cycles through RGB LED colors (if available)
- **Button 2:** Emits a beep from buzzer (if available)

---

## Next Steps

### Explore the Web Interface

- **Dashboard:** View system info (CPU, RAM, Flash, WiFi, etc.)
- **OTA Update:** Upload new firmware wirelessly
- **Reboot:** Restart the ESP32 remotely

### Customize Your Project

1. **Modify main.cpp:** Add your custom logic to `loop()`
2. **Create new modules:** Add files to `src/` and `include/`
3. **Add libraries:** Edit `platformio.ini` under `lib_deps`

### Learn Advanced Features

- [WiFi Setup Guide](WIFI_SETUP.md) - Multi-network configuration
- [OTA Update Guide](OTA_UPDATE.md) - Wireless firmware updates
- [Hardware Setup Guide](HARDWARE_SETUP.md) - GPIO configuration and peripherals

---

## Troubleshooting

### Problem: "secrets.h: No such file or directory"

**Solution:** You forgot to create `secrets.h`. Copy the example:
```bash
cp include/secrets_exemple.h include/secrets.h
```
Then edit it with your WiFi credentials.

---

### Problem: "WiFi connection failed"

**Solutions:**
1. Check your WiFi credentials in `secrets.h`
2. Make sure your WiFi network is 2.4GHz (ESP32 doesn't support 5GHz)
3. Verify the SSID and password are correct (case-sensitive)
4. Try moving the ESP32 closer to your router

---

### Problem: "Upload failed" or "Port not found"

**Solutions:**
1. Check USB cable connection
2. Install CP210x or CH340 USB drivers for your board
3. Try a different USB cable (some cables are charge-only)
4. Select the correct COM port in PlatformIO
5. Press and hold the BOOT button during upload (if auto-reset fails)

---

### Problem: Serial monitor shows garbled text

**Solution:** Make sure the baud rate is set to **115200** in the serial monitor.

---

### Problem: Board doesn't respond after upload

**Solutions:**
1. Press the **RESET** button on the ESP32
2. Disconnect and reconnect the USB cable
3. Re-upload the firmware

---

### Problem: Compilation errors

**Common causes:**
1. Missing `secrets.h` file
2. Wrong board selected in `platformio.ini`
3. Corrupted library cache - try: `pio run --target clean`

---

## Getting Help

- **GitHub Issues:** https://github.com/morfredus/Base_ESP32_Multi/issues
- **Documentation:** Check the `docs/` folder for detailed guides
- **PlatformIO Community:** https://community.platformio.org/

---

## What's Next?

- ✅ You've successfully set up Base_ESP32_Multi!
- 🔧 Customize hardware features in `board_config.h`
- 🌐 Configure multiple WiFi networks (see [WIFI_SETUP.md](WIFI_SETUP.md))
- 📡 Learn about OTA updates (see [OTA_UPDATE.md](OTA_UPDATE.md))
- 🚀 Start building your own features!

Happy coding! 🎉
