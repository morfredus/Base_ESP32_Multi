/**
 * @file neopixel.h
 * @brief Module de gestion centralisee des NeoPixels (LED interne + Matrice 8x8)
 * @version 0.8.3
 * @date 2026-01-04
 *
 * Ce module gere simultanement :
 * - La LED NeoPixel interne (GPIO 48 sur ESP32-S3 uniquement)
 * - La matrice WS2812B 8x8 (64 LEDs) sur MATRIX8X8_PIN (les deux environnements)
 *
 * IMPORTANT:
 * - La LED interne GPIO 48 est EXCLUSIVEMENT pour ESP32-S3
 * - La matrice utilise MATRIX8X8_PIN de board_config.h (ne pas modifier)
 * - Les deux modules coexistent independamment sur ESP32-S3
 * - L'absence de la matrice est geree sans crash
 *
 * Constantes utilisees depuis board_config.h (NE PAS RENOMMER):
 * - NEOPIXEL (GPIO 48 sur S3)
 * - MATRIX8X8_PIN (GPIO 3 sur S3, GPIO 32 sur Classic)
 * - MATRIX8X8_NUMPIXELS (64)
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
    extern Adafruit_NeoPixel* pInternalPixel;  // Pointeur pour init differee
#endif

#ifdef HAS_MATRIX8X8
    extern Adafruit_NeoPixel* pMatrix8x8;      // Pointeur pour init differee
    extern bool matrix8x8Available;
#endif

// ===================================================================
// SECTION 2 : COULEURS PREDEFINIES
// ===================================================================
// Format: 0xRRGGBB (la librairie Adafruit gere la conversion GRB)

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
 * @brief Initialise tous les modules NeoPixel actives
 *
 * Initialise dans l'ordre:
 * 1. LED NeoPixel interne (si HAS_NEOPIXEL && TARGET_ESP32_S3)
 * 2. Matrice 8x8 (si HAS_MATRIX8X8)
 *
 * Un delai est insere entre les initialisations pour eviter
 * les conflits RMT sur ESP32.
 *
 * @note Appeler UNE SEULE FOIS dans setup()
 */
void setupNeoPixels();

// ===================================================================
// SECTION 4 : LED INTERNE (ESP32-S3 UNIQUEMENT)
// ===================================================================

#if defined(HAS_NEOPIXEL) && defined(TARGET_ESP32_S3)

/**
 * @brief Definit la couleur de la LED interne
 * @param color Couleur au format 0xRRGGBB
 */
void setInternalPixelColor(uint32_t color);

/**
 * @brief Definit la couleur de la LED interne (RGB separes)
 * @param r Rouge (0-255)
 * @param g Vert (0-255)
 * @param b Bleu (0-255)
 */
void setInternalPixelRGB(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Definit la luminosite de la LED interne
 * @param brightness Luminosite (0-255)
 */
void setInternalPixelBrightness(uint8_t brightness);

/**
 * @brief Eteint la LED interne
 */
void clearInternalPixel();

/**
 * @brief Effet heartbeat sur la LED interne
 * @param baseColor Couleur de base (0xRRGGBB)
 * @param isHigh true = intensite forte, false = intensite faible
 */
void internalPixelHeartbeat(uint32_t baseColor, bool isHigh);

#endif // HAS_NEOPIXEL && TARGET_ESP32_S3

// ===================================================================
// SECTION 5 : MATRICE 8x8 (DEUX ENVIRONNEMENTS)
// ===================================================================

#ifdef HAS_MATRIX8X8

/**
 * @brief Verifie si la matrice 8x8 est disponible
 * @return true si la matrice est initialisee et prete
 */
bool isMatrix8x8Available();

/**
 * @brief Definit la couleur d'un pixel par index
 * @param pixel Index du pixel (0-63)
 * @param color Couleur (0xRRGGBB)
 */
void setMatrixPixelColor(uint16_t pixel, uint32_t color);

/**
 * @brief Definit la couleur d'un pixel par coordonnees X,Y
 * @param x Colonne (0-7)
 * @param y Ligne (0-7)
 * @param color Couleur (0xRRGGBB)
 */
void setMatrixPixelXY(uint8_t x, uint8_t y, uint32_t color);

/**
 * @brief Remplit toute la matrice avec une couleur
 * @param color Couleur (0xRRGGBB)
 */
void fillMatrix(uint32_t color);

/**
 * @brief Efface la matrice (tous les pixels eteints)
 */
void clearMatrix();

/**
 * @brief Definit la luminosite de la matrice
 * @param brightness Luminosite (0-255)
 */
void setMatrixBrightness(uint8_t brightness);

/**
 * @brief Applique les changements sur la matrice
 */
void showMatrix();

/**
 * @brief Convertit coordonnees X,Y en index (layout zigzag)
 * @param x Colonne (0-7)
 * @param y Ligne (0-7)
 * @return Index (0-63), ou 255 si hors limites
 */
uint8_t xyToIndex(uint8_t x, uint8_t y);

// --- Effets visuels ---

/**
 * @brief Effet arc-en-ciel
 * @param wait Delai entre etapes (ms)
 */
void matrixRainbow(uint16_t wait);

/**
 * @brief Remplissage progressif
 * @param color Couleur
 * @param wait Delai entre pixels (ms)
 */
void matrixWipe(uint32_t color, uint16_t wait);

/**
 * @brief Trace une ligne horizontale
 * @param y Ligne (0-7)
 * @param color Couleur
 */
void matrixDrawHLine(uint8_t y, uint32_t color);

/**
 * @brief Trace une ligne verticale
 * @param x Colonne (0-7)
 * @param color Couleur
 */
void matrixDrawVLine(uint8_t x, uint32_t color);

/**
 * @brief Trace un rectangle
 * @param x1,y1 Coin superieur gauche
 * @param x2,y2 Coin inferieur droit
 * @param color Couleur
 * @param filled true = rempli, false = contour
 */
void matrixDrawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                    uint32_t color, bool filled);

#endif // HAS_MATRIX8X8

// ===================================================================
// SECTION 6 : UTILITAIRES
// ===================================================================

/**
 * @brief Cree une couleur RGB
 * @param r,g,b Composantes (0-255)
 * @return Couleur (0xRRGGBB)
 */
uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Attenue une couleur
 * @param color Couleur originale
 * @param factor Facteur (0.0=noir, 1.0=original)
 * @return Couleur attenuee
 */
uint32_t dimColor(uint32_t color, float factor);

#endif // NEOPIXEL_H
