/**
 * @file neopixel.cpp
 * @brief Implementation du module de gestion des NeoPixels
 * @version 1.0.0
 * @date 2026-01-03
 */

#include "neopixel.h"

// ===================================================================
// INSTANCIATION DES OBJETS
// ===================================================================

#ifdef HAS_NEOPIXEL
    #if defined(TARGET_ESP32_S3)
        // LED NeoPixel interne soudee sur GPIO 48 (ESP32-S3)
        Adafruit_NeoPixel internalPixel(NEOPIXEL_NUM, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
    #endif
#endif

#ifdef HAS_MATRIX8X8
    // Matrice 8x8 WS2812B (64 LEDs)
    Adafruit_NeoPixel matrix8x8(MATRIX8X8_NUMPIXELS, MATRIX8X8_PIN, NEO_GRB + NEO_KHZ800);
    bool matrix8x8Available = false;
#endif

// ===================================================================
// INITIALISATION
// ===================================================================

void setupNeoPixels() {
    LOG_PRINTLN("--- Initialisation des NeoPixels ---");

    // --- LED NeoPixel interne (ESP32-S3 uniquement) ---
    #ifdef HAS_NEOPIXEL
        #if defined(TARGET_ESP32_S3)
            internalPixel.begin();
            internalPixel.setBrightness(30);
            internalPixel.clear();
            internalPixel.show();
            LOG_PRINTLN("[OK] LED NeoPixel interne (GPIO 48) initialisee");
        #else
            LOG_PRINTLN("[--] LED NeoPixel interne non disponible sur cette carte");
        #endif
    #endif

    // --- Matrice 8x8 ---
    #ifdef HAS_MATRIX8X8
        LOG_PRINTF("[..] Initialisation matrice 8x8 (GPIO %d)...\n", MATRIX8X8_PIN);

        matrix8x8.begin();
        matrix8x8.setBrightness(MATRIX_BRIGHTNESS);
        matrix8x8.clear();
        matrix8x8.show();

        // Test de detection de la matrice
        // Note: La detection physique n'est pas possible avec WS2812B
        // On suppose que la matrice est presente si l'initialisation reussit
        matrix8x8Available = true;

        LOG_PRINTF("[OK] Matrice 8x8 (%d pixels) initialisee sur GPIO %d\n",
                   MATRIX8X8_NUMPIXELS, MATRIX8X8_PIN);

        // Flash rapide pour confirmer l'initialisation
        fillMatrix(COLOR_GREEN);
        showMatrix();
        delay(100);
        clearMatrix();
        showMatrix();
    #endif
}

// ===================================================================
// LED INTERNE (ESP32-S3)
// ===================================================================

#ifdef HAS_NEOPIXEL
#if defined(TARGET_ESP32_S3)

void setInternalPixelColor(uint32_t color) {
    internalPixel.setPixelColor(0, color);
    internalPixel.show();
}

void setInternalPixelRGB(uint8_t r, uint8_t g, uint8_t b) {
    internalPixel.setPixelColor(0, internalPixel.Color(r, g, b));
    internalPixel.show();
}

void setInternalPixelBrightness(uint8_t brightness) {
    internalPixel.setBrightness(brightness);
    internalPixel.show();
}

void clearInternalPixel() {
    internalPixel.clear();
    internalPixel.show();
}

void internalPixelHeartbeat(uint32_t baseColor, bool isHigh) {
    if (isHigh) {
        internalPixel.setPixelColor(0, baseColor);
    } else {
        internalPixel.setPixelColor(0, dimColor(baseColor, 0.2f));
    }
    internalPixel.show();
}

#endif // TARGET_ESP32_S3
#endif // HAS_NEOPIXEL

// ===================================================================
// MATRICE 8x8
// ===================================================================

#ifdef HAS_MATRIX8X8

bool isMatrix8x8Available() {
    return matrix8x8Available;
}

void setMatrixPixelColor(uint16_t pixel, uint32_t color) {
    if (!matrix8x8Available || pixel >= MATRIX8X8_NUMPIXELS) return;
    matrix8x8.setPixelColor(pixel, color);
}

uint8_t xyToIndex(uint8_t x, uint8_t y) {
    if (x >= 8 || y >= 8) return 255;  // Hors limites

    // Layout standard: ligne par ligne, de gauche a droite
    // Modifiez cette formule si votre matrice a un cablage different
    // Layout en zigzag (serpentin) : alternance du sens a chaque ligne
    if (y % 2 == 0) {
        // Lignes paires : gauche a droite
        return y * 8 + x;
    } else {
        // Lignes impaires : droite a gauche
        return y * 8 + (7 - x);
    }
}

void setMatrixPixelXY(uint8_t x, uint8_t y, uint32_t color) {
    if (!matrix8x8Available) return;
    uint8_t index = xyToIndex(x, y);
    if (index != 255) {
        matrix8x8.setPixelColor(index, color);
    }
}

void fillMatrix(uint32_t color) {
    if (!matrix8x8Available) return;
    for (uint16_t i = 0; i < MATRIX8X8_NUMPIXELS; i++) {
        matrix8x8.setPixelColor(i, color);
    }
}

void clearMatrix() {
    if (!matrix8x8Available) return;
    matrix8x8.clear();
}

void setMatrixBrightness(uint8_t brightness) {
    if (!matrix8x8Available) return;
    matrix8x8.setBrightness(brightness);
}

void showMatrix() {
    if (!matrix8x8Available) return;
    matrix8x8.show();
}

// --- Effets visuels ---

void matrixRainbow(uint16_t wait) {
    if (!matrix8x8Available) return;

    static uint16_t hue = 0;
    for (uint16_t i = 0; i < MATRIX8X8_NUMPIXELS; i++) {
        uint16_t pixelHue = hue + (i * 65536L / MATRIX8X8_NUMPIXELS);
        matrix8x8.setPixelColor(i, matrix8x8.gamma32(matrix8x8.ColorHSV(pixelHue)));
    }
    matrix8x8.show();
    hue += 256;
    delay(wait);
}

void matrixWipe(uint32_t color, uint16_t wait) {
    if (!matrix8x8Available) return;

    for (uint16_t i = 0; i < MATRIX8X8_NUMPIXELS; i++) {
        matrix8x8.setPixelColor(i, color);
        matrix8x8.show();
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

void matrixDrawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint32_t color, bool filled) {
    if (!matrix8x8Available) return;

    // S'assurer que x1 <= x2 et y1 <= y2
    if (x1 > x2) { uint8_t tmp = x1; x1 = x2; x2 = tmp; }
    if (y1 > y2) { uint8_t tmp = y1; y1 = y2; y2 = tmp; }

    if (filled) {
        for (uint8_t y = y1; y <= y2 && y < 8; y++) {
            for (uint8_t x = x1; x <= x2 && x < 8; x++) {
                setMatrixPixelXY(x, y, color);
            }
        }
    } else {
        // Haut et bas
        for (uint8_t x = x1; x <= x2 && x < 8; x++) {
            setMatrixPixelXY(x, y1, color);
            setMatrixPixelXY(x, y2, color);
        }
        // Gauche et droite
        for (uint8_t y = y1; y <= y2 && y < 8; y++) {
            setMatrixPixelXY(x1, y, color);
            setMatrixPixelXY(x2, y, color);
        }
    }
}

#endif // HAS_MATRIX8X8

// ===================================================================
// UTILITAIRES
// ===================================================================

uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

uint32_t dimColor(uint32_t color, float factor) {
    if (factor < 0.0f) factor = 0.0f;
    if (factor > 1.0f) factor = 1.0f;

    uint8_t r = ((color >> 16) & 0xFF) * factor;
    uint8_t g = ((color >> 8) & 0xFF) * factor;
    uint8_t b = (color & 0xFF) * factor;

    return makeColor(r, g, b);
}
