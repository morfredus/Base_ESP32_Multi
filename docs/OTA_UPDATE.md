# OTA Update Guide

**Version:** 0.9.0
**Last Updated:** 2026-01-03

This guide explains how to update your ESP32 firmware Over-The-Air (OTA) without needing a USB cable. Base_ESP32_Multi supports two OTA methods: Web Browser upload and Network upload via Arduino IDE/PlatformIO.

---

## 📋 Table of Contents

1. [What is OTA?](#what-is-ota)
2. [OTA Methods](#ota-methods)
3. [Web Browser OTA (Recommended)](#web-browser-ota-recommended)
4. [Network OTA via PlatformIO](#network-ota-via-platformio)
5. [Security Considerations](#security-considerations)
6. [Troubleshooting](#troubleshooting)
7. [Best Practices](#best-practices)

---

## What is OTA?

**OTA (Over-The-Air)** updates allow you to upload new firmware to your ESP32 **wirelessly** over WiFi, without connecting a USB cable.

### Benefits

- ✅ **No physical access needed** - Update devices in hard-to-reach locations
- ✅ **Faster deployment** - Update multiple devices quickly
- ✅ **Reduced wear** - No USB port wear from repeated connections
- ✅ **Remote updates** - Update devices anywhere on your network
- ✅ **User-friendly** - Web interface for non-technical users

### Requirements

- ESP32 must be connected to WiFi
- Sufficient flash memory (OTA requires space for both old and new firmware)
- Same network as your computer (or port forwarding for remote updates)

---

## OTA Methods

Base_ESP32_Multi v0.9.0 supports **two OTA methods**:

| Method | Interface | Best For | Difficulty |
|--------|-----------|----------|------------|
| **Web Browser OTA** | HTTP upload via web page | End users, quick updates | ⭐ Easy |
| **Network OTA** | Arduino IDE / PlatformIO | Developers, debugging | ⭐⭐ Intermediate |

Both methods are included in the firmware by default!

---

## Web Browser OTA (Recommended)

### Overview

Upload a compiled `.bin` firmware file through a web browser. Perfect for non-developers!

### Step-by-Step Instructions

#### 1. Build Your Firmware

First, compile your project to generate a `.bin` file:

**Using PlatformIO:**

```bash
pio run
```

The firmware file will be located at:
```
.pio/build/<environment>/firmware.bin
```

For example:
- `.pio/build/esp32s3_n16r8/firmware.bin`
- `.pio/build/esp32devkitc/firmware.bin`

#### 2. Access the OTA Page

1. Make sure your ESP32 is powered on and connected to WiFi
2. Check the Serial Monitor for the IP address:
   ```
   Web server started on http://192.168.1.100
     - OTA page: http://192.168.1.100/ota
   ```

3. Open a web browser and navigate to:
   ```
   http://<ESP32-IP-Address>/ota
   ```
   Example: `http://192.168.1.100/ota`

#### 3. Upload the Firmware

1. **Click "Choose File"** and select your `.bin` firmware file
2. **Click "Upload Firmware"**
3. **Wait for upload** - A progress bar will show the upload status
4. **Success!** - The device will automatically reboot with the new firmware

**Upload Progress:**
```
⏳ Uploading firmware... [████████████] 100%
✅ Upload successful! Device rebooting...
```

#### 4. Verify the Update

After reboot (takes ~10 seconds):

1. The web page will automatically redirect to the dashboard
2. Check the version number in the header
3. Or check Serial Monitor for the new version:
   ```
   === Base_ESP32_Multi v0.9.1 ===  ← New version
   ```

---

### Web OTA Features

**✅ Drag-and-Drop Support** - Drop `.bin` files directly on the upload zone
**✅ File Validation** - Warns if you upload a non-.bin file
**✅ Real-Time Progress** - See upload percentage and data transferred
**✅ Inline Validation** - No annoying popup confirmations
**✅ Auto-Reboot** - Device restarts automatically after successful upload
**✅ Error Handling** - Clear error messages if upload fails

---

## Network OTA via PlatformIO

### Overview

Upload firmware directly from PlatformIO using the ESP32's network address. Great for development workflows!

### Prerequisites

1. ESP32 connected to WiFi (same network as your computer)
2. ArduinoOTA enabled in firmware (already included in v0.9.0)
3. Know your ESP32's IP address

### Configuration

#### 1. Find Your ESP32's IP Address

Check the Serial Monitor after boot:
```
Web server started on http://192.168.1.100
```

Or check your router's DHCP client list.

#### 2. Configure platformio.ini

Add the following to your environment in `platformio.ini`:

```ini
[env:esp32s3_n16r8]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino

; ... existing settings ...

; OTA Configuration
upload_protocol = espota
upload_port = 192.168.1.100      ; ← Replace with your ESP32's IP
upload_flags =
    --port=3232
    --auth=                       ; Leave empty if no password
```

**For multiple environments**, add to each one you want to OTA upload to.

#### 3. Upload via OTA

```bash
pio run --target upload
```

Or use the PlatformIO GUI: **Upload** button (same as USB upload)

### OTA Upload Process

You'll see output like:

```
Uploading .pio/build/esp32s3_n16r8/firmware.bin
Uploading: [============      ] 60%
Uploading: [==================] 100%
Success
```

The ESP32 will automatically reboot with the new firmware!

---

### Network OTA Features

The firmware includes visual feedback during network OTA:

#### TFT Display (if available)
- Shows "OTA UPDATE" title
- Real-time progress bar (0-100%)
- Upload speed and estimated time
- Success/error messages

#### NeoPixel LED (if available)
- **Blue** - OTA update in progress
- **Green** - OTA success
- **Red** - OTA error

#### Serial Monitor
Detailed logs:
```
ArduinoOTA started on port 3232
Hostname: Base_ESP32_Multi
OTA Update Started
Progress: 0%
Progress: 25%
Progress: 50%
Progress: 75%
Progress: 100%
OTA Update Complete! Rebooting...
```

---

## Security Considerations

### Default Security

By default, **no password** is required for OTA updates. This is convenient but **not secure** for production devices!

### Adding OTA Password Protection

#### For Web OTA

Edit `include/web_interface.h` and add authentication:

```cpp
void handleOTA() {
    // Add HTTP Basic Authentication
    if (!server.authenticate("admin", "your-password-here")) {
        return server.requestAuthentication();
    }

    String html = generateOTAPage();
    server.send(200, "text/html", html);
}
```

#### For Network OTA (ArduinoOTA)

Edit `src/main.cpp` in the `setupOTA()` function:

```cpp
void setupOTA() {
    ArduinoOTA.setPort(3232);
    ArduinoOTA.setHostname(PROJECT_NAME);

    // Add password protection
    ArduinoOTA.setPassword("your-ota-password");  // ← Add this line

    // ... rest of setup ...
}
```

Then update `platformio.ini`:

```ini
upload_flags =
    --port=3232
    --auth=your-ota-password     ; ← Add password
```

### Network Security

**⚠️ Important Security Practices:**

1. **Use passwords** - Always set OTA passwords for devices outside your home network
2. **Trusted networks only** - Only enable OTA on trusted WiFi networks
3. **Firewall rules** - Block port 3232 from external access
4. **HTTPS** - For production, consider HTTPS for web OTA (requires SSL certificates)
5. **Verify firmware** - Double-check .bin files before uploading

**For production devices**, consider:
- Implementing firmware signature verification
- Using VPN for remote OTA access
- Disabling OTA after deployment (via build flag)

---

## Troubleshooting

### Problem: "OTA page not accessible"

**Symptoms:**
- Browser shows "Connection refused" or "Page not found"
- Can't access `http://<IP>/ota`

**Solutions:**
1. Verify ESP32 is connected to WiFi (check Serial Monitor)
2. Ping the ESP32: `ping 192.168.1.100` (replace with your IP)
3. Check if web server started (Serial Monitor should show "Web server started")
4. Make sure you're on the same network as the ESP32
5. Try the root page first: `http://<IP>/` (dashboard)
6. Disable firewall temporarily to test
7. Try incognito/private browsing mode

---

### Problem: "Upload fails at 0%" or "Connection lost"

**Symptoms:**
```
⏳ Uploading firmware...
❌ Network error. Check connection and try again.
```

**Solutions:**

1. **Insufficient memory** - ESP32 doesn't have enough free flash space
   - Check if firmware.bin size < available flash
   - Free up space by reducing code/assets

2. **WiFi signal weak** - Move ESP32 closer to router during update

3. **Power supply issues** - Use a quality 2A USB power supply
   - OTA upload is power-intensive
   - Weak power can cause crashes

4. **Network congestion** - Pause other network-heavy activities during update

5. **Firewall blocking** - Temporarily disable firewall

6. **Wrong IP address** - Double-check ESP32's current IP

---

### Problem: "Upload completes but device won't boot"

**Symptoms:**
- Upload shows 100% success
- Device reboots but doesn't start (boot loop)
- Serial Monitor shows garbled text or repeating errors

**Solutions:**

1. **Wrong firmware file** - Make sure you uploaded the correct .bin for your board
   - ESP32-S3 firmware won't work on ESP32 Classic
   - Check partitions match

2. **Corrupted upload** - Re-upload the firmware
   - Try Web OTA instead of Network OTA (or vice versa)

3. **Partition overflow** - Firmware is too large for partition
   - Check `.pio/build/<env>/firmware.bin` size
   - Compare with partition table in `platformio.ini`

4. **Flash manually** - If OTA is broken, use USB cable to recover:
   ```bash
   pio run --target upload
   ```

---

### Problem: "Network OTA not found by PlatformIO"

**Symptoms:**
```
Error: Timeout waiting for response
Could not connect to esp32-s3-devkitc-1 on port 3232
```

**Solutions:**

1. **Check IP address** - Verify ESP32 IP in `platformio.ini` matches current IP
2. **Port 3232 blocked** - Check firewall rules
3. **mDNS not working** - Use IP address instead of hostname
4. **ESP32 not advertising** - Check Serial Monitor for "ArduinoOTA started"
5. **Wrong network** - Make sure computer and ESP32 are on same subnet

---

### Problem: "Upload succeeds but shows old version"

**Symptoms:**
- Upload completes successfully
- Device reboots
- Version number doesn't change

**Solutions:**

1. **Browser cache** - Hard refresh the web page (Ctrl+F5 or Cmd+Shift+R)
2. **Uploaded wrong file** - Verify you compiled before uploading
3. **Version not updated** - Check if you actually changed PROJECT_VERSION in platformio.ini
4. **Old build** - Clean and rebuild:
   ```bash
   pio run --target clean
   pio run
   ```

---

## Best Practices

### Development Workflow

1. **Test locally first** - Use USB upload for initial testing
2. **Enable OTA after WiFi works** - Don't rely on OTA for first upload
3. **Keep USB accessible** - Always have a way to recover via USB
4. **Version everything** - Update version number with each build
5. **Test OTA early** - Verify OTA works before deploying devices remotely

### Production Deployment

1. **Always set passwords** - Protect both Web OTA and Network OTA
2. **Test firmware thoroughly** - Never deploy untested firmware via OTA
3. **Have a rollback plan** - Keep previous firmware version ready
4. **Monitor uploads** - Watch Serial Monitor during OTA updates
5. **Staged rollout** - Update one device first, verify, then update others
6. **Document versions** - Keep a CHANGELOG of what each version changes

### Safety Measures

```cpp
// In main.cpp - Add OTA safety checks
void setupOTA() {
    ArduinoOTA.setHostname(PROJECT_NAME);
    ArduinoOTA.setPassword("my-secure-password");

    ArduinoOTA.onStart([]() {
        // Prevent writes during OTA
        // Close files, stop tasks, etc.
    });

    ArduinoOTA.onError([](ota_error_t error) {
        // Log error for debugging
        LOG_PRINTF("OTA Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) LOG_PRINTLN("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) LOG_PRINTLN("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) LOG_PRINTLN("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) LOG_PRINTLN("Receive Failed");
        else if (error == OTA_END_ERROR) LOG_PRINTLN("End Failed");

        // Don't reboot on error - allow retry
    });

    ArduinoOTA.begin();
}
```

---

## OTA Upload Checklist

Before every OTA update:

- [ ] Firmware compiled successfully (no errors)
- [ ] Version number updated in platformio.ini
- [ ] Tested on one device via USB first
- [ ] ESP32 has stable WiFi connection
- [ ] Good power supply connected (2A+ recommended)
- [ ] Backup of previous firmware saved
- [ ] Know how to recover via USB if OTA fails
- [ ] Serial Monitor ready to monitor upload

---

## Getting Help

If you encounter issues not covered here:

1. **Check Serial Monitor** - Most errors are logged there
2. **Verify network connectivity** - Can you ping the ESP32?
3. **Test with USB first** - Make sure the firmware works before OTA
4. **Check GitHub Issues** - Someone may have had the same problem

**GitHub Issues:** https://github.com/morfredus/Base_ESP32_Multi/issues

---

## Summary

- ✅ **Two OTA methods:** Web Browser (easy) and Network (PlatformIO)
- ✅ **Web OTA:** Upload .bin files via web page at `http://<IP>/ota`
- ✅ **Network OTA:** Configure `upload_port` in platformio.ini
- ✅ **Security:** Add passwords for production devices
- ✅ **Troubleshooting:** Check WiFi, power, and firmware size
- ✅ **Best practices:** Test locally, version everything, monitor uploads

Happy wireless updating! 📡
