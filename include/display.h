/**
 * @file display.h
 * @brief Module de gestion centralisée des écrans (OLED et ST7789)
 * @version 0.8.6
 * @date 2026-01-04
 *
 * CORRECTIF v0.8.6:
 * - Pas d'instanciation statique (evite bootloop)
 * - Pointeurs null, objets crees dans setupDisplays()
 * - Evite le "Static Initialization Order Fiasco"
 *
 * Ce module fournit une interface unifiée pour gérer l'affichage
 * sur les écrans OLED (SSD1306) et TFT couleur (ST7789).
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include "config.h"

// ===================================================================
// SECTION 1 : INCLUDES CONDITIONNELS
// ===================================================================

#ifdef HAS_OLED
    #include <Adafruit_GFX.h>
    #include <Adafruit_SSD1306.h>
#endif

#ifdef HAS_ST7789
    #include <Adafruit_GFX.h>
    #include <Adafruit_ST7789.h>
#endif

// ===================================================================
// SECTION 2 : POINTEURS GLOBAUX (PAS D'INSTANCIATION STATIQUE!)
// ===================================================================
// IMPORTANT: Les objets sont crees dans setupDisplays() avec new
// pour eviter le "Static Initialization Order Fiasco"

#ifdef HAS_OLED
    extern Adafruit_SSD1306* pDisplayOled;
#endif

#ifdef HAS_ST7789
    extern Adafruit_ST7789* pDisplayTft;
#endif

// ===================================================================
// SECTION 3 : INTERFACE PUBLIQUE
// ===================================================================

/**
 * @brief Initialise tous les écrans activés (OLED et/ou ST7789)
 * @note Appeler cette fonction une seule fois dans setup()
 */
void setupDisplays();

/**
 * @brief Affiche l'écran de démarrage avec nom du projet et version
 */
void displayStartup(const char* projectName, const char* projectVersion);

/**
 * @brief Affiche la progression de la connexion WiFi
 * @param progress Progression en pourcentage (0-100)
 */
void displayWifiProgress(int progress);

/**
 * @brief Affiche les informations de connexion WiFi réussie
 */
void displayWifiConnected(const char* ssid, IPAddress ip);

/**
 * @brief Affiche un message d'erreur de connexion WiFi
 */
void displayWifiFailed();

// ===================================================================
// SECTION 4 : FONCTIONS INTERNES OLED
// ===================================================================

#ifdef HAS_OLED
bool setupOled();
void updateOledStatus(const char* status, int progress = -1);
void updateOledConnected(const char* ssid, IPAddress ip);
#endif

// ===================================================================
// SECTION 5 : FONCTIONS INTERNES ST7789
// ===================================================================

#ifdef HAS_ST7789
bool setupST7789();
void displayST7789Startup(const char* projectName, const char* projectVersion);
void displayST7789Progress(int progress);
void displayST7789Connected(const char* ssid, IPAddress ip);
void displayST7789Failed();
#endif

#endif // DISPLAY_H
