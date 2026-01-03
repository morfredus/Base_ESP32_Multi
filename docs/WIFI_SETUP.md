# WiFi Setup Guide

**Version:** 0.9.0
**Last Updated:** 2026-01-03

This guide explains how to configure WiFi connectivity for Base_ESP32_Multi, including multi-network support and advanced troubleshooting.

---

## 📋 Table of Contents

1. [Overview](#overview)
2. [Quick Setup](#quick-setup)
3. [Multi-Network Configuration](#multi-network-configuration)
4. [Security Best Practices](#security-best-practices)
5. [Advanced Configuration](#advanced-configuration)
6. [Troubleshooting](#troubleshooting)

---

## Overview

Base_ESP32_Multi uses **WiFiMulti** to support multiple WiFi networks with automatic failover. This means:

- ✅ Define up to **10 WiFi networks** (easily extendable)
- ✅ **Automatic connection** to the strongest available network
- ✅ **Automatic reconnection** if the current network fails
- ✅ **Secure credential storage** (gitignored `secrets.h` file)
- ✅ **Easy switching** between home, office, mobile hotspot, etc.

**Supported WiFi Standards:**
- 802.11 b/g/n (2.4 GHz only)
- WPA/WPA2/WPA3 Personal encryption
- Open networks (no password)

**⚠️ Important:** ESP32 does **not** support 5GHz WiFi (802.11ac/ax). Use 2.4GHz networks only.

---

## Quick Setup

### Step 1: Create secrets.h

The `secrets.h` file contains your WiFi credentials and is **not tracked by git** for security.

```bash
# From the project root
cp include/secrets_exemple.h include/secrets.h
```

### Step 2: Edit WiFi Credentials

Open `include/secrets.h` in your editor and configure at least one network:

```cpp
// Network 1 (Primary) - REQUIRED
const char* WIFI_SSID1 = "MyHomeWiFi";      // Your WiFi network name
const char* WIFI_PASS1 = "MyPassword123";   // Your WiFi password

// Network 2 (Secondary) - OPTIONAL
const char* WIFI_SSID2 = "";  // Leave empty if not needed
const char* WIFI_PASS2 = "";

// Network 3 (Mobile/Guest) - OPTIONAL
const char* WIFI_SSID3 = "";
const char* WIFI_PASS3 = "";

// Network 4 (Extra) - OPTIONAL
const char* WIFI_SSID4 = "";
const char* WIFI_PASS4 = "";
```

### Step 3: Build and Upload

```bash
pio run --target upload
```

### Step 4: Verify Connection

Open the Serial Monitor (115200 baud) and check for:

```
--- Starting WiFiMulti ---
  - Network added: MyHomeWiFi
Connecting to WiFi...
✓ Connected to: MyHomeWiFi
IP Address: 192.168.1.100
```

---

## Multi-Network Configuration

### Use Case 1: Home + Office Networks

Perfect for devices that move between locations:

```cpp
const char* WIFI_SSID1 = "HomeNetwork";
const char* WIFI_PASS1 = "HomePassword";

const char* WIFI_SSID2 = "OfficeNetwork";
const char* WIFI_PASS2 = "OfficePassword";

const char* WIFI_SSID3 = "";
const char* WIFI_PASS3 = "";

const char* WIFI_SSID4 = "";
const char* WIFI_PASS4 = "";
```

**How it works:**
- ESP32 will try `HomeNetwork` first
- If unavailable, it tries `OfficeNetwork`
- Automatically switches when you move locations

---

### Use Case 2: Home + Mobile Hotspot Backup

Useful for testing or when your main network is down:

```cpp
const char* WIFI_SSID1 = "HomeNetwork";
const char* WIFI_PASS1 = "HomePassword";

const char* WIFI_SSID2 = "iPhone-Hotspot";
const char* WIFI_PASS2 = "hotspot123";

const char* WIFI_SSID3 = "";
const char* WIFI_PASS3 = "";

const char* WIFI_SSID4 = "";
const char* WIFI_PASS4 = "";
```

---

### Use Case 3: Multiple Networks with Open WiFi

Some networks don't require a password (open networks):

```cpp
const char* WIFI_SSID1 = "SecureHome";
const char* WIFI_PASS1 = "MyStrongPassword";

const char* WIFI_SSID2 = "GuestNetwork";    // Open network
const char* WIFI_PASS2 = "";                 // Empty password for open networks

const char* WIFI_SSID3 = "";
const char* WIFI_PASS3 = "";

const char* WIFI_SSID4 = "";
const char* WIFI_PASS4 = "";
```

**⚠️ Security Warning:** Open networks transmit data unencrypted. Avoid using them for sensitive operations.

---

### Adding More Than 4 Networks

To add networks 5, 6, 7, etc.:

**1. Edit `include/secrets.h`:**

```cpp
// Add these lines
const char* WIFI_SSID5 = "Network5";
const char* WIFI_PASS5 = "Password5";

const char* WIFI_SSID6 = "Network6";
const char* WIFI_PASS6 = "Password6";
```

**2. Edit `src/main.cpp` in the `setupWifi()` function:**

```cpp
void setupWifi() {
    LOG_PRINTLN("--- Starting WiFiMulti ---");

    if (strlen(WIFI_SSID1) > 0) {
        wifiMulti.addAP(WIFI_SSID1, WIFI_PASS1);
        LOG_PRINTF("  - Network added: %s\n", WIFI_SSID1);
    }
    // ... existing SSID2, SSID3, SSID4 ...

    // Add these new blocks:
    if (strlen(WIFI_SSID5) > 0) {
        wifiMulti.addAP(WIFI_SSID5, WIFI_PASS5);
        LOG_PRINTF("  - Network added: %s\n", WIFI_SSID5);
    }
    if (strlen(WIFI_SSID6) > 0) {
        wifiMulti.addAP(WIFI_SSID6, WIFI_PASS6);
        LOG_PRINTF("  - Network added: %s\n", WIFI_SSID6);
    }
    // etc.
}
```

---

## Security Best Practices

### ✅ DO:

1. **Keep secrets.h private:**
   - Never commit `secrets.h` to git (it's already in `.gitignore`)
   - Never share screenshots or logs containing passwords

2. **Use strong passwords:**
   - Minimum 12 characters
   - Mix of uppercase, lowercase, numbers, symbols
   - Avoid dictionary words

3. **Use WPA2/WPA3 encryption:**
   - Avoid WEP (broken security)
   - Prefer WPA3 if your router supports it

4. **Rotate credentials:**
   - Change WiFi passwords periodically
   - Update `secrets.h` after password changes

5. **Separate networks:**
   - Use a guest network for IoT devices if possible
   - Keeps IoT devices isolated from your main network

### ❌ DON'T:

1. **Never hardcode credentials in main.cpp:**
   - ❌ Bad: `wifiMulti.addAP("MyWiFi", "password123");`
   - ✅ Good: Use `secrets.h` variables

2. **Don't use open networks for sensitive data:**
   - OTA updates on open networks are risky
   - Anyone can sniff your traffic

3. **Don't commit secrets.h:**
   - Already prevented by `.gitignore`
   - Double-check before pushing to GitHub

---

## Advanced Configuration

### Connection Timeout

By default, the ESP32 tries to connect for ~20 attempts. To adjust:

**Edit `src/main.cpp`:**

```cpp
void setupWifi() {
    // ... network setup ...

    int attempts = 0;
    const int maxAttempts = 30;  // ← Increase timeout (default: 20)

    while (wifiMulti.run() != WL_CONNECTED && attempts < maxAttempts) {
        // ... connection logic ...
    }
}
```

### Static IP Configuration

To use a fixed IP instead of DHCP:

**Edit `src/main.cpp` after network connection:**

```cpp
void setupWifi() {
    // ... after successful connection ...

    if (wifiMulti.run() == WL_CONNECTED) {
        LOG_PRINTLN("✓ Connected!");

        // Optional: Set static IP
        IPAddress local_IP(192, 168, 1, 100);     // Desired IP
        IPAddress gateway(192, 168, 1, 1);        // Router IP
        IPAddress subnet(255, 255, 255, 0);       // Subnet mask
        IPAddress primaryDNS(8, 8, 8, 8);         // Google DNS
        IPAddress secondaryDNS(8, 8, 4, 4);       // Google DNS backup

        if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
            LOG_PRINTLN("Static IP configuration failed");
        }
    }
}
```

**⚠️ Warning:** Make sure the static IP is:
- Outside your router's DHCP range
- Not already used by another device
- In the same subnet as your router

### WiFi Power Management

To optimize for battery or stability:

```cpp
void setupWifi() {
    // ... after connection ...

    // Maximum performance (higher power consumption)
    WiFi.setSleep(false);

    // OR: Power-saving mode (may increase latency)
    // WiFi.setSleep(true);
}
```

---

## Troubleshooting

### Problem: "No networks added to WiFiMulti"

**Symptoms:**
```
--- Starting WiFiMulti ---
(No network added messages)
Connecting to WiFi...
WiFi connection failed
```

**Causes:**
1. All SSID variables are empty strings `""`
2. `secrets.h` not created (compilation would fail)

**Solution:**
- Make sure at least WIFI_SSID1 is not empty
- Check for typos in `secrets.h`

---

### Problem: "WiFi connection failed after 20 attempts"

**Symptoms:**
```
--- Starting WiFiMulti ---
  - Network added: MyWiFi
Connecting to WiFi........
WiFi connection failed after 20 attempts
```

**Solutions:**

1. **Check SSID and Password:**
   - Verify spelling (case-sensitive)
   - Check for extra spaces: `"MyWiFi"` ≠ `"MyWiFi "`
   - Try connecting with your phone to verify credentials

2. **Check 2.4GHz vs 5GHz:**
   - ESP32 only works with 2.4GHz
   - If your router has dual-band, make sure 2.4GHz is enabled
   - Some routers have separate SSIDs for each band

3. **Signal Strength:**
   - Move ESP32 closer to the router
   - Remove metal objects between ESP32 and router
   - Check antenna connection (if external antenna)

4. **Router Settings:**
   - Disable MAC address filtering (or whitelist ESP32)
   - Disable AP isolation on guest networks
   - Check maximum client limit (routers have connection limits)
   - Try setting a fixed WiFi channel (1, 6, or 11 for less interference)

5. **ESP32 Reset:**
   - Some ESP32 boards need a hard reset after flashing
   - Press the RESET button
   - Or power cycle the board

---

### Problem: "Connected but no internet"

**Symptoms:**
```
✓ Connected to: MyWiFi
IP Address: 192.168.1.100
(Web server accessible locally, but OTA fails)
```

**Solutions:**
1. Check your router's internet connection
2. Verify DNS settings (try `8.8.8.8` manually)
3. Check if your network has firewall rules blocking ESP32
4. Try pinging the gateway: add to `setup()`:
   ```cpp
   if (WiFi.ping(WiFi.gatewayIP())) {
       LOG_PRINTLN("Gateway reachable");
   }
   ```

---

### Problem: "Connection drops frequently"

**Symptoms:**
- WiFi connects, then disconnects after a few minutes
- Reconnects automatically but unstable

**Solutions:**

1. **Power Supply Issues:**
   - ESP32 WiFi draws significant current (~250mA peak)
   - Use a good quality USB power supply (2A minimum)
   - Add a 100µF capacitor between 3V3 and GND

2. **Interference:**
   - Move away from microwave ovens, Bluetooth devices
   - Try a different WiFi channel on your router
   - Reduce distance to router

3. **Disable Power Saving:**
   ```cpp
   WiFi.setSleep(false);
   ```

4. **Implement Auto-Reconnect:**
   The firmware already includes reconnection logic in `loop()`:
   ```cpp
   void loop() {
       if (WiFi.status() != WL_CONNECTED) {
           LOG_PRINTLN("WiFi disconnected, reconnecting...");
           wifiMulti.run();
       }
   }
   ```

---

### Problem: "secrets.h not found during compilation"

**Symptoms:**
```
fatal error: secrets.h: No such file or directory
```

**Solution:**
You forgot to create `secrets.h`:
```bash
cp include/secrets_exemple.h include/secrets.h
```
Then edit it with your WiFi credentials.

---

### Debugging WiFi Issues

Add detailed WiFi debugging:

**Edit `platformio.ini`:**

```ini
build_flags =
    -DCORE_DEBUG_LEVEL=5        ; Maximum debug level
    -DLOG_ENABLED=1
```

**Rebuild and check serial output for:**
- WiFi scan results
- Connection attempts
- DHCP negotiation
- Error codes

---

## Testing Your Configuration

### Test Script

Add this to `loop()` temporarily to test network switching:

```cpp
void loop() {
    // ... existing code ...

    // WiFi status every 10 seconds
    static unsigned long lastCheck = 0;
    if (millis() - lastCheck > 10000) {
        lastCheck = millis();

        LOG_PRINTF("WiFi Status: %s\n", WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected");
        if (WiFi.status() == WL_CONNECTED) {
            LOG_PRINTF("  SSID: %s\n", WiFi.SSID().c_str());
            LOG_PRINTF("  IP: %s\n", WiFi.localIP().toString().c_str());
            LOG_PRINTF("  Signal: %d dBm\n", WiFi.RSSI());
        }
    }
}
```

**Good signal strength:**
- -30 to -50 dBm: Excellent
- -50 to -60 dBm: Good
- -60 to -70 dBm: Fair
- Below -70 dBm: Weak (may cause issues)

---

## Getting Help

If you're still having issues:

1. **Check Serial Monitor** (115200 baud) for error messages
2. **Test with your phone** - Can you connect with the same credentials?
3. **Try a different network** - Is the problem network-specific?
4. **Report on GitHub** with:
   - Serial output (remove passwords!)
   - Router model and settings
   - ESP32 board variant

**GitHub Issues:** https://github.com/morfredus/Base_ESP32_Multi/issues

---

## Summary

- ✅ Create `secrets.h` from `secrets_exemple.h`
- ✅ Configure at least one network (WIFI_SSID1/PASS1)
- ✅ Add multiple networks for automatic failover
- ✅ Use strong WPA2/WPA3 passwords
- ✅ Keep `secrets.h` private (gitignored)
- ✅ Test with serial monitor to verify connection

Happy networking! 🌐
