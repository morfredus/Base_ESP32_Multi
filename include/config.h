/**
 * @file config.h
 * @brief Configuration centrale du projet ESP32 Multi-environnement
 * @version 0.8.5
 * @date 2026-01-04
 *
 * Ce fichier centralise toutes les configurations du projet:
 * - Parametres globaux (debug, serial, WiFi timeout)
 * - Activation des modules (OLED, ST7789, NeoPixel, Matrice 8x8)
 * - Alias de pins vers board_config.h
 *
 * REGLES STRICTES:
 * - NE PAS modifier board_config.h
 * - NE PAS renommer les constantes de board_config.h
 * - Les alias PIN_xxx mappent vers les constantes de board_config.h
 *
 * ARCHITECTURE NEOPIXEL:
 * - LED interne GPIO 48 : ESP32-S3 UNIQUEMENT (HAS_NEOPIXEL)
 * - Matrice 8x8 : DEUX environnements (HAS_MATRIX8X8)
 * - Les deux coexistent independamment sur ESP32-S3
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "board_config.h"

// ===================================================================
// SECTION 1 : CONFIGURATION GLOBALE
// ===================================================================

#define SERIAL_BAUD_RATE 115200
#define WIFI_TIMEOUT_MS  10000  // 10 secondes pour tenter la connexion

// ===================================================================
// SECTION 2 : DEBUG
// ===================================================================
// Commenter pour desactiver les logs serie

#define ENABLE_DEBUG_LOG

#ifdef ENABLE_DEBUG_LOG
    #define LOG_PRINT(...) Serial.print(__VA_ARGS__)
    #define LOG_PRINTLN(...) Serial.println(__VA_ARGS__)
    #define LOG_PRINTF(...) Serial.printf(__VA_ARGS__)
#else
    #define LOG_PRINT(...)
    #define LOG_PRINTLN(...)
    #define LOG_PRINTF(...)
#endif

// ===================================================================
// SECTION 3 : MODULES AFFICHAGE
// ===================================================================

// --- OLED SSD1306 ---
#define HAS_OLED
#define OLED_WIDTH       128
#define OLED_HEIGHT      64
#define OLED_ADDR        0x3C   // Adresse I2C (0x3D sur certains)
#define OLED_RESET       -1     // Pas de pin reset

// --- TFT ST7789 ---
#define HAS_ST7789
#define ST7789_WIDTH     240
#define ST7789_HEIGHT    320    // 240 carre, 135 rectangulaire
#define ST7789_ROTATION  2      // Rotation (0-3)

// ===================================================================
// SECTION 4 : MODULES LED
// ===================================================================

// --- LED RGB Standard ---
#define HAS_LED_RGB

// --- NeoPixel interne (ESP32-S3 GPIO 48 UNIQUEMENT) ---
#define HAS_NEOPIXEL
#define NEOPIXEL_NUM     1      // 1 LED interne soudee

// --- Matrice NeoPixel 8x8 WS2812B (DEUX environnements) ---
// ATTENTION: Cause bootloop si activee avec LED interne!
// Decommenter UNIQUEMENT si matrice physiquement connectee
// Utilise MATRIX8X8_PIN et MATRIX8X8_NUMPIXELS de board_config.h
// #define HAS_MATRIX8X8
#define MATRIX_BRIGHTNESS 30    // Luminosite par defaut (0-255)

// ===================================================================
// SECTION 5 : ALIAS DE PINS
// ===================================================================
// Mapping vers les constantes de board_config.h
// NE PAS RENOMMER les constantes sources!

// --- Boutons ---
#define PIN_BUTTON_BOOT   BUTTON_BOOT
#define PIN_BUTTON_1      BUTTON_1
#define PIN_BUTTON_2      BUTTON_2

// --- LEDs RGB ---
#define PIN_LED_RED       LED_RED
#define PIN_LED_GREEN     LED_GREEN
#define PIN_LED_BLUE      LED_BLUE

// --- Buzzer ---
#define PIN_BUZZER        BUZZER

// --- I2C ---
#define PIN_I2C_SDA       I2C_SDA
#define PIN_I2C_SCL       I2C_SCL

// --- TFT SPI ---
#define PIN_TFT_MOSI      TFT_MOSI
#define PIN_TFT_MISO      TFT_MISO
#define PIN_TFT_SCLK      TFT_SCLK
#define PIN_TFT_CS        TFT_CS
#define PIN_TFT_DC        TFT_DC
#define PIN_TFT_RST       TFT_RST
#define PIN_TFT_BL        TFT_BL

// --- NeoPixel interne (ESP32-S3 uniquement) ---
#if defined(TARGET_ESP32_S3)
    #define PIN_NEOPIXEL  NEOPIXEL   // GPIO 48
#endif

// --- LED Builtin (ESP32 Classic uniquement) ---
#if defined(TARGET_ESP32_CLASSIC)
    #define PIN_LED_BUILTIN LED_BUILTIN  // GPIO 2
#endif

// ===================================================================
// SECTION 6 : NOTES IMPORTANTES
// ===================================================================
/*
 * CONSTANTES DE board_config.h (NE PAS MODIFIER):
 *
 * ESP32-S3:
 *   NEOPIXEL = 48 (LED interne soudee)
 *   MATRIX8X8_PIN = 3
 *   MATRIX8X8_NUMPIXELS = 64
 *
 * ESP32 Classic:
 *   LED_BUILTIN = 2
 *   MATRIX8X8_PIN = 32
 *   MATRIX8X8_NUMPIXELS = 64
 *
 * Les couleurs ST77xx sont dans Adafruit_ST77xx.h:
 *   ST77XX_BLACK, ST77XX_WHITE, ST77XX_RED, etc.
 */

#endif // CONFIG_H
