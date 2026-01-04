/**
 * @file neopixel.cpp
 * @brief Implementation du module de gestion des NeoPixels
 * @version 0.8.4
 * @date 2026-01-04
 *
 * CORRECTIFS v0.8.4:
 * - Retour a l'instanciation statique (allocation dynamique causait crash)
 * - Matrice 8x8 desactivee par defaut (cause de bootloop)
 * - Delai yield() pour eviter watchdog timeout
 * - Initialisation sequentielle avec verifications
 *
 * REGLES STRICTES:
 * - NE PAS modifier board_config.h
 * - NE PAS renommer MATRIX8X8_PIN, MATRIX8X8_NUMPIXELS, NEOPIXEL
 * - LED interne GPIO 48 = ESP32-S3 UNIQUEMENT
 * - Matrice 8x8 = DESACTIVEE par defaut (decommenter HAS_MATRIX8X8)
 */

#include "neopixel.h"

// ===================================================================
// SECTION 1 : INSTANCIATION STATIQUE DES OBJETS
// ===================================================================
// Instanciation statique pour eviter les crashs d'allocation dynamique

#if defined(HAS_NEOPIXEL) && defined(TARGET_ESP32_S3)
    // LED NeoPixel interne soudee sur GPIO 48 (ESP32-S3 uniquement)
    static Adafruit_NeoPixel _internalPixel(NEOPIXEL_NUM, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
    Adafruit_NeoPixel* pInternalPixel = &_internalPixel;
    static bool internalPixelReady = false;
#endif

#ifdef HAS_MATRIX8X8
    // Matrice 8x8 WS2812B - 64 LEDs
    static Adafruit_NeoPixel _matrix8x8(MATRIX8X8_NUMPIXELS, MATRIX8X8_PIN, NEO_GRB + NEO_KHZ800);
    Adafruit_NeoPixel* pMatrix8x8 = &_matrix8x8;
    bool matrix8x8Available = false;
#endif

// ===================================================================
// SECTION 2 : INITIALISATION
// ===================================================================

void setupNeoPixels() {
    LOG_PRINTLN("--- Initialisation NeoPixels v0.8.4 ---");

    // ---------------------------------------------------------------
    // 2.1 : LED NeoPixel interne (ESP32-S3 uniquement, GPIO 48)
    // ---------------------------------------------------------------
    #if defined(HAS_NEOPIXEL) && defined(TARGET_ESP32_S3)
        LOG_PRINTLN("[..] Init LED interne GPIO 48...");
        yield();  // Evite watchdog timeout

        _internalPixel.begin();
        yield();

        _internalPixel.setBrightness(30);
        _internalPixel.clear();
        _internalPixel.show();

        internalPixelReady = true;
        LOG_PRINTLN("[OK] LED interne GPIO 48 prete");

        delay(10);  // Court delai stabilisation
    #endif

    // ---------------------------------------------------------------
    // 2.2 : Matrice 8x8 (desactivee par defaut - cause bootloop)
    // ---------------------------------------------------------------
    #ifdef HAS_MATRIX8X8
        LOG_PRINTF("[..] Init matrice 8x8 GPIO %d...\n", MATRIX8X8_PIN);
        yield();  // Evite watchdog timeout

        _matrix8x8.begin();
        yield();

        _matrix8x8.setBrightness(MATRIX_BRIGHTNESS);
        _matrix8x8.clear();
        _matrix8x8.show();

        matrix8x8Available = true;
        LOG_PRINTF("[OK] Matrice 8x8 prete (GPIO %d)\n", MATRIX8X8_PIN);

        // PAS de flash de confirmation!
    #endif

    LOG_PRINTLN("--- NeoPixels OK ---");
}

// ===================================================================
// SECTION 3 : LED INTERNE (ESP32-S3 UNIQUEMENT)
// ===================================================================

#if defined(HAS_NEOPIXEL) && defined(TARGET_ESP32_S3)

void setInternalPixelColor(uint32_t color) {
    if (!internalPixelReady) return;
    _internalPixel.setPixelColor(0, color);
    _internalPixel.show();
}

void setInternalPixelRGB(uint8_t r, uint8_t g, uint8_t b) {
    if (!internalPixelReady) return;
    _internalPixel.setPixelColor(0, _internalPixel.Color(r, g, b));
    _internalPixel.show();
}

void setInternalPixelBrightness(uint8_t brightness) {
    if (!internalPixelReady) return;
    _internalPixel.setBrightness(brightness);
    _internalPixel.show();
}

void clearInternalPixel() {
    if (!internalPixelReady) return;
    _internalPixel.clear();
    _internalPixel.show();
}

void internalPixelHeartbeat(uint32_t baseColor, bool isHigh) {
    if (!internalPixelReady) return;
    if (isHigh) {
        _internalPixel.setPixelColor(0, baseColor);
    } else {
        _internalPixel.setPixelColor(0, dimColor(baseColor, 0.2f));
    }
    _internalPixel.show();
}

#endif // HAS_NEOPIXEL && TARGET_ESP32_S3

// ===================================================================
// SECTION 4 : MATRICE 8x8 (DEUX ENVIRONNEMENTS)
// ===================================================================

#ifdef HAS_MATRIX8X8

bool isMatrix8x8Available() {
    return matrix8x8Available;
}

void setMatrixPixelColor(uint16_t pixel, uint32_t color) {
    if (!matrix8x8Available || pixel >= MATRIX8X8_NUMPIXELS) return;
    _matrix8x8.setPixelColor(pixel, color);
}

uint8_t xyToIndex(uint8_t x, uint8_t y) {
    if (x >= 8 || y >= 8) return 255;

    // Layout zigzag (serpentin)
    if (y % 2 == 0) {
        return y * 8 + x;           // Lignes paires: gauche -> droite
    } else {
        return y * 8 + (7 - x);     // Lignes impaires: droite -> gauche
    }
}

void setMatrixPixelXY(uint8_t x, uint8_t y, uint32_t color) {
    if (!matrix8x8Available) return;
    uint8_t index = xyToIndex(x, y);
    if (index != 255) {
        _matrix8x8.setPixelColor(index, color);
    }
}

void fillMatrix(uint32_t color) {
    if (!matrix8x8Available) return;
    for (uint16_t i = 0; i < MATRIX8X8_NUMPIXELS; i++) {
        _matrix8x8.setPixelColor(i, color);
    }
}

void clearMatrix() {
    if (!matrix8x8Available) return;
    _matrix8x8.clear();
}

void setMatrixBrightness(uint8_t brightness) {
    if (!matrix8x8Available) return;
    _matrix8x8.setBrightness(brightness);
}

void showMatrix() {
    if (!matrix8x8Available) return;
    _matrix8x8.show();
}

// ---------------------------------------------------------------
// 4.1 : Effets visuels
// ---------------------------------------------------------------

void matrixRainbow(uint16_t wait) {
    if (!matrix8x8Available) return;

    static uint16_t hue = 0;
    for (uint16_t i = 0; i < MATRIX8X8_NUMPIXELS; i++) {
        uint16_t pixelHue = hue + (i * 65536L / MATRIX8X8_NUMPIXELS);
        _matrix8x8.setPixelColor(i, _matrix8x8.gamma32(_matrix8x8.ColorHSV(pixelHue)));
    }
    _matrix8x8.show();
    hue += 256;
    delay(wait);
}

void matrixWipe(uint32_t color, uint16_t wait) {
    if (!matrix8x8Available) return;

    for (uint16_t i = 0; i < MATRIX8X8_NUMPIXELS; i++) {
        _matrix8x8.setPixelColor(i, color);
        _matrix8x8.show();
        delay(wait);
    }
}

void matrixDrawHLine(uint8_t y, uint32_t color) {
    if (!matrix8x8Available || y >= 8) return;
    for (uint8_t x = 0; x < 8; x++) {
        setMatrixPixelXY(x, y, color);
    }
}

void matrixDrawVLine(uint8_t x, uint32_t color) {
    if (!matrix8x8Available || x >= 8) return;
    for (uint8_t y = 0; y < 8; y++) {
        setMatrixPixelXY(x, y, color);
    }
}

void matrixDrawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                    uint32_t color, bool filled) {
    if (!matrix8x8Available) return;

    if (x1 > x2) { uint8_t t = x1; x1 = x2; x2 = t; }
    if (y1 > y2) { uint8_t t = y1; y1 = y2; y2 = t; }

    if (filled) {
        for (uint8_t y = y1; y <= y2 && y < 8; y++) {
            for (uint8_t x = x1; x <= x2 && x < 8; x++) {
                setMatrixPixelXY(x, y, color);
            }
        }
    } else {
        for (uint8_t x = x1; x <= x2 && x < 8; x++) {
            setMatrixPixelXY(x, y1, color);
            setMatrixPixelXY(x, y2, color);
        }
        for (uint8_t y = y1; y <= y2 && y < 8; y++) {
            setMatrixPixelXY(x1, y, color);
            setMatrixPixelXY(x2, y, color);
        }
    }
}

#endif // HAS_MATRIX8X8

// ===================================================================
// SECTION 5 : UTILITAIRES
// ===================================================================

uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

uint32_t dimColor(uint32_t color, float factor) {
    if (factor < 0.0f) factor = 0.0f;
    if (factor > 1.0f) factor = 1.0f;

    uint8_t r = (uint8_t)(((color >> 16) & 0xFF) * factor);
    uint8_t g = (uint8_t)(((color >> 8) & 0xFF) * factor);
    uint8_t b = (uint8_t)((color & 0xFF) * factor);

    return makeColor(r, g, b);
}
