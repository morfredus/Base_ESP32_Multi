/**
 * @file neopixel.cpp
 * @brief Implementation du module de gestion des NeoPixels
 * @version 0.8.3
 * @date 2026-01-04
 *
 * CORRECTIFS v0.8.3:
 * - Utilisation de pointeurs pour eviter les conflits RMT au boot
 * - Suppression du flash de confirmation (cause de bootloop)
 * - Delai entre initialisations LED interne et matrice
 * - Verification de validite des pointeurs avant chaque operation
 *
 * REGLES STRICTES:
 * - NE PAS modifier board_config.h
 * - NE PAS renommer MATRIX8X8_PIN, MATRIX8X8_NUMPIXELS, NEOPIXEL
 * - LED interne GPIO 48 = ESP32-S3 UNIQUEMENT
 * - Matrice 8x8 = les DEUX environnements
 */

#include "neopixel.h"

// ===================================================================
// SECTION 1 : DECLARATION DES POINTEURS GLOBAUX
// ===================================================================
// Utilisation de pointeurs pour initialisation differee (evite conflits RMT)

#if defined(HAS_NEOPIXEL) && defined(TARGET_ESP32_S3)
    Adafruit_NeoPixel* pInternalPixel = nullptr;
#endif

#ifdef HAS_MATRIX8X8
    Adafruit_NeoPixel* pMatrix8x8 = nullptr;
    bool matrix8x8Available = false;
#endif

// ===================================================================
// SECTION 2 : INITIALISATION
// ===================================================================

void setupNeoPixels() {
    LOG_PRINTLN("--- Initialisation des NeoPixels ---");

    // ---------------------------------------------------------------
    // 2.1 : LED NeoPixel interne (ESP32-S3 uniquement, GPIO 48)
    // ---------------------------------------------------------------
    #if defined(HAS_NEOPIXEL) && defined(TARGET_ESP32_S3)
        LOG_PRINTLN("[..] Init LED interne (GPIO 48)...");

        // Allocation dynamique pour eviter conflit RMT au boot
        pInternalPixel = new Adafruit_NeoPixel(NEOPIXEL_NUM, PIN_NEOPIXEL,
                                                NEO_GRB + NEO_KHZ800);
        if (pInternalPixel != nullptr) {
            pInternalPixel->begin();
            pInternalPixel->setBrightness(30);
            pInternalPixel->clear();
            pInternalPixel->show();
            LOG_PRINTLN("[OK] LED interne GPIO 48 initialisee");
        } else {
            LOG_PRINTLN("[!!] Echec allocation LED interne");
        }

        // Delai critique pour eviter conflit RMT avec la matrice
        delay(50);
    #endif

    // ---------------------------------------------------------------
    // 2.2 : Matrice 8x8 (les deux environnements)
    // ---------------------------------------------------------------
    #ifdef HAS_MATRIX8X8
        LOG_PRINTF("[..] Init matrice 8x8 (GPIO %d, %d pixels)...\n",
                   MATRIX8X8_PIN, MATRIX8X8_NUMPIXELS);

        // Allocation dynamique pour eviter conflit RMT au boot
        pMatrix8x8 = new Adafruit_NeoPixel(MATRIX8X8_NUMPIXELS, MATRIX8X8_PIN,
                                           NEO_GRB + NEO_KHZ800);
        if (pMatrix8x8 != nullptr) {
            pMatrix8x8->begin();
            pMatrix8x8->setBrightness(MATRIX_BRIGHTNESS);
            pMatrix8x8->clear();
            pMatrix8x8->show();

            // Marquer comme disponible APRES initialisation reussie
            matrix8x8Available = true;
            LOG_PRINTF("[OK] Matrice 8x8 initialisee (GPIO %d)\n", MATRIX8X8_PIN);
        } else {
            matrix8x8Available = false;
            LOG_PRINTLN("[!!] Echec allocation matrice 8x8");
        }

        // PAS de flash de confirmation - cause de bootloop!
    #endif

    LOG_PRINTLN("--- NeoPixels prets ---");
}

// ===================================================================
// SECTION 3 : LED INTERNE (ESP32-S3 UNIQUEMENT)
// ===================================================================

#if defined(HAS_NEOPIXEL) && defined(TARGET_ESP32_S3)

void setInternalPixelColor(uint32_t color) {
    if (pInternalPixel == nullptr) return;
    pInternalPixel->setPixelColor(0, color);
    pInternalPixel->show();
}

void setInternalPixelRGB(uint8_t r, uint8_t g, uint8_t b) {
    if (pInternalPixel == nullptr) return;
    pInternalPixel->setPixelColor(0, pInternalPixel->Color(r, g, b));
    pInternalPixel->show();
}

void setInternalPixelBrightness(uint8_t brightness) {
    if (pInternalPixel == nullptr) return;
    pInternalPixel->setBrightness(brightness);
    pInternalPixel->show();
}

void clearInternalPixel() {
    if (pInternalPixel == nullptr) return;
    pInternalPixel->clear();
    pInternalPixel->show();
}

void internalPixelHeartbeat(uint32_t baseColor, bool isHigh) {
    if (pInternalPixel == nullptr) return;
    if (isHigh) {
        pInternalPixel->setPixelColor(0, baseColor);
    } else {
        pInternalPixel->setPixelColor(0, dimColor(baseColor, 0.2f));
    }
    pInternalPixel->show();
}

#endif // HAS_NEOPIXEL && TARGET_ESP32_S3

// ===================================================================
// SECTION 4 : MATRICE 8x8 (DEUX ENVIRONNEMENTS)
// ===================================================================

#ifdef HAS_MATRIX8X8

bool isMatrix8x8Available() {
    return matrix8x8Available && (pMatrix8x8 != nullptr);
}

void setMatrixPixelColor(uint16_t pixel, uint32_t color) {
    if (!isMatrix8x8Available() || pixel >= MATRIX8X8_NUMPIXELS) return;
    pMatrix8x8->setPixelColor(pixel, color);
}

uint8_t xyToIndex(uint8_t x, uint8_t y) {
    if (x >= 8 || y >= 8) return 255;

    // Layout zigzag (serpentin) : alternance du sens a chaque ligne
    // Lignes paires: gauche -> droite
    // Lignes impaires: droite -> gauche
    if (y % 2 == 0) {
        return y * 8 + x;
    } else {
        return y * 8 + (7 - x);
    }
}

void setMatrixPixelXY(uint8_t x, uint8_t y, uint32_t color) {
    if (!isMatrix8x8Available()) return;
    uint8_t index = xyToIndex(x, y);
    if (index != 255) {
        pMatrix8x8->setPixelColor(index, color);
    }
}

void fillMatrix(uint32_t color) {
    if (!isMatrix8x8Available()) return;
    for (uint16_t i = 0; i < MATRIX8X8_NUMPIXELS; i++) {
        pMatrix8x8->setPixelColor(i, color);
    }
}

void clearMatrix() {
    if (!isMatrix8x8Available()) return;
    pMatrix8x8->clear();
}

void setMatrixBrightness(uint8_t brightness) {
    if (!isMatrix8x8Available()) return;
    pMatrix8x8->setBrightness(brightness);
}

void showMatrix() {
    if (!isMatrix8x8Available()) return;
    pMatrix8x8->show();
}

// ---------------------------------------------------------------
// 4.1 : Effets visuels
// ---------------------------------------------------------------

void matrixRainbow(uint16_t wait) {
    if (!isMatrix8x8Available()) return;

    static uint16_t hue = 0;
    for (uint16_t i = 0; i < MATRIX8X8_NUMPIXELS; i++) {
        uint16_t pixelHue = hue + (i * 65536L / MATRIX8X8_NUMPIXELS);
        pMatrix8x8->setPixelColor(i, pMatrix8x8->gamma32(
                                      pMatrix8x8->ColorHSV(pixelHue)));
    }
    pMatrix8x8->show();
    hue += 256;
    delay(wait);
}

void matrixWipe(uint32_t color, uint16_t wait) {
    if (!isMatrix8x8Available()) return;

    for (uint16_t i = 0; i < MATRIX8X8_NUMPIXELS; i++) {
        pMatrix8x8->setPixelColor(i, color);
        pMatrix8x8->show();
        delay(wait);
    }
}

void matrixDrawHLine(uint8_t y, uint32_t color) {
    if (!isMatrix8x8Available() || y >= 8) return;

    for (uint8_t x = 0; x < 8; x++) {
        setMatrixPixelXY(x, y, color);
    }
}

void matrixDrawVLine(uint8_t x, uint32_t color) {
    if (!isMatrix8x8Available() || x >= 8) return;

    for (uint8_t y = 0; y < 8; y++) {
        setMatrixPixelXY(x, y, color);
    }
}

void matrixDrawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                    uint32_t color, bool filled) {
    if (!isMatrix8x8Available()) return;

    // Normaliser les coordonnees
    if (x1 > x2) { uint8_t t = x1; x1 = x2; x2 = t; }
    if (y1 > y2) { uint8_t t = y1; y1 = y2; y2 = t; }

    if (filled) {
        for (uint8_t y = y1; y <= y2 && y < 8; y++) {
            for (uint8_t x = x1; x <= x2 && x < 8; x++) {
                setMatrixPixelXY(x, y, color);
            }
        }
    } else {
        // Lignes horizontales (haut et bas)
        for (uint8_t x = x1; x <= x2 && x < 8; x++) {
            setMatrixPixelXY(x, y1, color);
            setMatrixPixelXY(x, y2, color);
        }
        // Lignes verticales (gauche et droite)
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
