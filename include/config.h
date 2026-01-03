#ifndef CONFIG_H
#define CONFIG_H

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

// ============================================================================
// Configuration NeoPixel (LED RGB adressable WS2812B / SK6812)
// ============================================================================
//
// POUR ACTIVER LE NEOPIXEL :
// 1. Décommentez la ligne "#define HAS_NEOPIXEL" ci-dessous
// 2. Connectez votre NeoPixel au bon GPIO selon votre carte :
//    - ESP32-S3 : GPIO 48 (défini comme NEOPIXEL dans board_config.h)
//    - ESP32 Classic : GPIO 2 (utilise NEOPIXEL_MATRIX par défaut)
//
// ⚠️ ATTENTION ESP32 CLASSIC :
//    GPIO 2 est partagé avec LED_BUILTIN. Si vous activez le NeoPixel,
//    la LED intégrée bleue clignotera de manière erratique.
//    Solution : Ne pas utiliser digitalWrite(LED_BUILTIN) dans votre code.
//
// 💡 ASTUCE : Par défaut, HAS_NEOPIXEL est désactivé pour éviter les
//    problèmes de crash RMT (Remote Control Module) sur certaines cartes
//    qui n'ont pas de NeoPixel physiquement connecté.
//
// Décommenter pour activer le NeoPixel :
//#define HAS_NEOPIXEL

// Nombre de LEDs NeoPixel (1 pour une LED unique, 64 pour matrice 8x8, etc.)
#define NEOPIXEL_NUM     1

// --- Configuration TFT (ST7789 / ILI9341 / autres écrans TFT couleur) ---
// Décommenter pour activer l'affichage TFT
#define HAS_ST7789
#define TFT_WIDTH     240   // Largeur de l'écran en pixels
#define TFT_HEIGHT    240   // Hauteur de l'écran (240 pour carré, 135 pour rectangulaire, 320 pour ILI9341)
#define TFT_ROTATION  2     // Rotation de l'écran (0, 1, 2, 3)

// Note : Les couleurs sont définies dans Adafruit_ST77xx.h (ST77XX_BLACK, ST77XX_WHITE, etc.)

#endif