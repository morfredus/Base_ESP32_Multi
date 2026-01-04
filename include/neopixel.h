/**
 * @file neopixel.h
 * @brief Module de gestion des NeoPixels (LED interne + Matrice 8x8)
 * @version 0.8.4
 * @date 2026-01-04
 *
 * ARCHITECTURE:
 * - LED interne GPIO 48 : ESP32-S3 UNIQUEMENT (HAS_NEOPIXEL)
 * - Matrice 8x8 : DESACTIVEE par defaut (HAS_MATRIX8X8)
 *
 * IMPORTANT - REGLES STRICTES:
 * - NE PAS modifier board_config.h
 * - NE PAS renommer NEOPIXEL, MATRIX8X8_PIN, MATRIX8X8_NUMPIXELS
 * - La LED interne GPIO 48 reste independante de la matrice
 * - Matrice desactivee par defaut (cause bootloop avec LED interne)
 *
 * CONSTANTES (board_config.h - NE PAS MODIFIER):
 *   ESP32-S3:    NEOPIXEL=48, MATRIX8X8_PIN=3
 *   ESP32 Classic: MATRIX8X8_PIN=32
 *   Les deux: MATRIX8X8_NUMPIXELS=64
 */

#ifndef NEOPIXEL_H
#define NEOPIXEL_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"

// ===================================================================
// SECTION 1 : OBJETS GLOBAUX
// ===================================================================

#if defined(HAS_NEOPIXEL) && defined(TARGET_ESP32_S3)
    extern Adafruit_NeoPixel* pInternalPixel;
#endif

#ifdef HAS_MATRIX8X8
    extern Adafruit_NeoPixel* pMatrix8x8;
    extern bool matrix8x8Available;
#endif

// ===================================================================
// SECTION 2 : COULEURS PREDEFINIES (0xRRGGBB)
// ===================================================================

#define COLOR_OFF      0x000000
#define COLOR_RED      0xFF0000
#define COLOR_GREEN    0x00FF00
#define COLOR_BLUE     0x0000FF
#define COLOR_YELLOW   0xFFFF00
#define COLOR_CYAN     0x00FFFF
#define COLOR_MAGENTA  0xFF00FF
#define COLOR_WHITE    0xFFFFFF
#define COLOR_ORANGE   0xFFA500
#define COLOR_PURPLE   0x800080

// ===================================================================
// SECTION 3 : INITIALISATION
// ===================================================================

/**
 * @brief Initialise les NeoPixels actives
 * @note Appeler UNE SEULE FOIS dans setup()
 */
void setupNeoPixels();

// ===================================================================
// SECTION 4 : LED INTERNE (ESP32-S3 GPIO 48)
// ===================================================================

#if defined(HAS_NEOPIXEL) && defined(TARGET_ESP32_S3)

void setInternalPixelColor(uint32_t color);
void setInternalPixelRGB(uint8_t r, uint8_t g, uint8_t b);
void setInternalPixelBrightness(uint8_t brightness);
void clearInternalPixel();
void internalPixelHeartbeat(uint32_t baseColor, bool isHigh);

#endif

// ===================================================================
// SECTION 5 : MATRICE 8x8 (DESACTIVEE PAR DEFAUT)
// ===================================================================

#ifdef HAS_MATRIX8X8

bool isMatrix8x8Available();
void setMatrixPixelColor(uint16_t pixel, uint32_t color);
void setMatrixPixelXY(uint8_t x, uint8_t y, uint32_t color);
void fillMatrix(uint32_t color);
void clearMatrix();
void setMatrixBrightness(uint8_t brightness);
void showMatrix();
uint8_t xyToIndex(uint8_t x, uint8_t y);

// Effets visuels
void matrixRainbow(uint16_t wait);
void matrixWipe(uint32_t color, uint16_t wait);
void matrixDrawHLine(uint8_t y, uint32_t color);
void matrixDrawVLine(uint8_t x, uint32_t color);
void matrixDrawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                    uint32_t color, bool filled);

#endif

// ===================================================================
// SECTION 6 : UTILITAIRES
// ===================================================================

uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b);
uint32_t dimColor(uint32_t color, float factor);

#endif // NEOPIXEL_H
