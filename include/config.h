#ifndef CONFIG_H
#define CONFIG_H

#include "board_config.h"

// --- Configuration Globale ---
#define SERIAL_BAUD_RATE 115200
#define WIFI_TIMEOUT_MS  10000 // 10 secondes pour tenter la connexion

// Debug : Décommenter pour voir les logs détaillés
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

// --- Configuration OLED (SSD1306) ---
// Décommenter pour activer l'affichage OLED
#define HAS_OLED
#define OLED_WIDTH       128
#define OLED_HEIGHT      64
#define OLED_ADDR        0x3C  // Adresse I2C standard (0x3D sur certains modèles)
#define OLED_RESET       -1    // Pas de pin de reset (-1)

// --- Configuration LED RGB ---
// Décommenter pour activer le contrôle de la LED RGB standard
#define HAS_LED_RGB

// --- Configuration NeoPixel (LED RGB adressable) ---
// Décommenter pour activer la neopixel intégrée
#define HAS_NEOPIXEL
#define NEOPIXEL_NUM     1     // Nombre de neopixels (1 pour le modèle intégré)

// --- Configuration Matrice NeoPixel 8x8 (WS2812B) ---
// Décommenter pour activer la matrice 8x8 (64 LEDs)
#define HAS_MATRIX8X8
#define MATRIX_BRIGHTNESS 30  // Luminosité par défaut de la matrice (0-255)

// --- Configuration ST7789 (TFT couleur) ---
// Décommenter pour activer l'affichage ST7789
#define HAS_ST7789
#define ST7789_WIDTH     240   // Largeur de l'écran en pixels
#define ST7789_HEIGHT    240   // Hauteur de l'écran (240 pour carré, 135 pour rectangulaire)
#define ST7789_ROTATION  2     // Rotation de l'écran (0, 1, 2, 3)

// Note : Les couleurs sont définies dans Adafruit_ST77xx.h (ST77XX_BLACK, ST77XX_WHITE, etc.)

// ===================================================================
// ALIAS DE PINS (Mapping vers board_config.h)
// ===================================================================
// Ces alias permettent une syntaxe cohérente PIN_xxx dans le code

// Boutons
#define PIN_BUTTON_BOOT   BUTTON_BOOT
#define PIN_BUTTON_1      BUTTON_1
#define PIN_BUTTON_2      BUTTON_2

// LEDs
#define PIN_LED_RED       LED_RED
#define PIN_LED_GREEN     LED_GREEN
#define PIN_LED_BLUE      LED_BLUE

// Buzzer
#define PIN_BUZZER        BUZZER

// I2C
#define PIN_I2C_SDA       I2C_SDA
#define PIN_I2C_SCL       I2C_SCL

// TFT
#define PIN_TFT_MOSI      TFT_MOSI
#define PIN_TFT_MISO      TFT_MISO
#define PIN_TFT_SCLK      TFT_SCLK
#define PIN_TFT_CS        TFT_CS
#define PIN_TFT_DC        TFT_DC
#define PIN_TFT_RST       TFT_RST
#define PIN_TFT_BL        TFT_BL

// NeoPixel (LED interne - ESP32-S3 uniquement)
#if defined(TARGET_ESP32_S3)
    #define PIN_NEOPIXEL  NEOPIXEL
#endif

// LED Builtin (ESP32 Classic uniquement)
#if defined(TARGET_ESP32_CLASSIC)
    #define PIN_LED_BUILTIN LED_BUILTIN
#endif

// Matrice NeoPixel 8x8 (utilise les constantes de board_config.h)
// PIN: MATRIX8X8_PIN, NUMPIXELS: MATRIX8X8_NUMPIXELS

#endif