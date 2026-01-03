/**
 * @file secrets_exemple.h
 * @brief Example configuration file for WiFi credentials
 * @version 0.9.1
 *
 * IMPORTANT: This is an EXAMPLE file. You must:
 * 1. Copy this file to 'secrets.h' in the same directory
 * 2. Edit 'secrets.h' with your actual WiFi credentials
 * 3. NEVER commit 'secrets.h' to version control (it's in .gitignore)
 *
 * Usage:
 * - The system will try to connect to networks in order (SSID1, SSID2, SSID3, etc.)
 * - Add as many networks as needed by creating WIFI_SSID4/WIFI_PASS4, etc.
 * - Leave unused networks with empty strings ("") to disable them
 */

#ifndef SECRETS_H
#define SECRETS_H

// ============================================================
// WiFi Network Configuration (Multi-Network Support)
// ============================================================
// The ESP32 will attempt to connect to these networks in order.
// Add your WiFi credentials below. You can define up to 10 networks.

// Network 1 (Primary)
const char* WIFI_SSID1 = "YourHomeNetwork";   // Replace with your WiFi SSID
const char* WIFI_PASS1 = "YourPassword123";   // Replace with your WiFi password

// Network 2 (Secondary - Office, backup, etc.)
const char* WIFI_SSID2 = "OfficeNetwork";     // Replace or leave empty "" to disable
const char* WIFI_PASS2 = "OfficePass456";     // Replace or leave empty "" to disable

// Network 3 (Mobile hotspot, guest network, etc.)
const char* WIFI_SSID3 = "";                  // Optional - leave empty to disable
const char* WIFI_PASS3 = "";                  // Optional - leave empty to disable

// Network 4 (Optional)
const char* WIFI_SSID4 = "";                  // Optional - leave empty to disable
const char* WIFI_PASS4 = "";                  // Optional - leave empty to disable

// ============================================================
// IMPORTANT NOTES:
// ============================================================
// - Passwords are case-sensitive
// - SSID names must match exactly (including spaces)
// - For WPA2/WPA3 networks, use the password directly
// - For open networks (no password), use an empty string: ""
// - Maximum password length: 63 characters
// - Maximum SSID length: 32 characters
//
// Example for open network:
//   const char* WIFI_SSID3 = "FreePublicWiFi";
//   const char* WIFI_PASS3 = "";
//
// To add more networks, simply create WIFI_SSID5/WIFI_PASS5, etc.
// and update the WiFi setup code in main.cpp accordingly.
// ============================================================

#endif // SECRETS_H
