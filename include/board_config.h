/**
 * @file board_config.h
 * @brief Mapping des pins GPIO pour les différentes cartes ESP32
 * @version 0.7.0
 * @date 2025-12-06
 * 
 * Ce fichier centralise toutes les définitions de pins selon la carte utilisée.
 * 
 * @section usage Utilisation
 * 1. Définissez la cible dans platformio.ini avec l'option `-D TARGET_ESP32_S3` ou `-D TARGET_ESP32_CLASSIC`
 * 2. Les pins correspondantes seront automatiquement sélectionnées
 * 3. Consultez docs/PIN_MAPPING.md pour les schémas de connexion détaillés
 * 
 * @section pinMapping Référence des pins
 * - Les broches I2C (SDA/SCL) sont pour l'écran OLED SSD1306
 * - Les broches SPI (MOSI/SCLK/CS/DC/RST) sont pour l'écran TFT ST7789 ou ILI9341
 * - NEOPIXEL est optionnel (LED RGB adressable)
 * 
 * @see include/config.h pour activer/désactiver les écrans (HAS_OLED, HAS_ST7789)
 */

#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include <Arduino.h>

// ============================================================================
// BOUTON BOOT - Disponible sur toutes les cartes
// ============================================================================
/**
 * @def PIN_BUTTON_BOOT
 * @brief Bouton BOOT intégré sur toutes les cartes ESP32
 * 
 * Ce bouton est présent physiquement sur la carte. Appui long pour redémarrer.
 * Valeur GPIO : 0 (universel sur toutes les ESP32)
 * 
 * @note Actif bas : L'appui ramène le pin à GND (0V)
 * @see main.cpp pour la gestion (OneButton library)
 */
#define PIN_BUTTON_BOOT 0 

// ============================================================================
// CONFIGURATION ESP32-S3 DevKitC-1
// ============================================================================
#if defined(TARGET_ESP32_S3)

    /**
     * @def BOARD_NAME
     * @brief Nom lisible de la carte pour affichage
     */
    #define BOARD_NAME "ESP32-S3 DevKitC-1"
    
    // ========================================================================
    // LED RGB (NeoPixel) - Optionnel
    // ========================================================================
    /**
     * @defgroup NEOPIXEL NeoPixel LED RGB
     * @brief Configuration de la LED RGB adressable intégrée
     * @{
     */
    #define HAS_NEOPIXEL           ///< Définit que la carte a un NeoPixel
    #define PIN_NEOPIXEL 48        ///< GPIO 48 : Broche de contrôle de la LED
    #define NEOPIXEL_NUM 1         ///< Nombre de pixels (1 sur la S3 DevKitC)
    /** @} */
    
    // ========================================================================
    // Bus I2C - Pour l'écran OLED SSD1306
    // ========================================================================
    /**
     * @defgroup I2C_OLED Bus I2C pour OLED
     * @brief Configuration des pins I2C pour l'écran OLED SSD1306
     * 
     * Protocole I2C : Utilise seulement 2 broches de données (SDA/SCL)
     * - Plus simple à câbler (moins de fils)
     * - Partageable avec plusieurs appareils I2C
     * - Adresse par défaut : 0x3C (voir config.h)
     * @{
     */
    #define PIN_I2C_SDA 21         ///< GPIO 21 : Données I2C (Serial DAta)
    #define PIN_I2C_SCL 20         ///< GPIO 20 : Horloge I2C (Serial CLock)
    /** @} */

    // ========================================================================
    // Bus SPI - Pour l'écran TFT ST7789 (écran couleur)
    // ========================================================================
    /**
     * @defgroup SPI_TFT Bus SPI pour TFT ST7789
     * @brief Configuration des pins SPI pour l'écran TFT ST7789 (240x240 couleur)
     * 
     * Protocole SPI : Communication rapide avec plusieurs broches de contrôle
     * - Très rapide (écran couleur fluide)
     * - Plus de branchements que I2C
     * 
     * @note Consultez docs/PIN_MAPPING.md pour le schéma de connexion complet
     * @{
     */
    #define TFT_MOSI  11      ///< GPIO 11 : Données (Master Out Slave In) / SDA du TFT
    #define TFT_SCLK  12      ///< GPIO 12 : Horloge SPI (Serial CLocK) / SCL du TFT
    #define TFT_CS    10      ///< GPIO 10 : Sélection puce (Chip Select)
    #define TFT_DC     9      ///< GPIO  9 : Données/Commande (Data/Command)
    #define TFT_RST   13      ///< GPIO 13 : Réinitialisation (ReSeT)
    #define TFT_BL     7      ///< GPIO  7 : Rétroéclairage (BackLight) - LED pour illuminer l'écran
    /** @} */

// ============================================================================
// CONFIGURATION ESP32 CLASSIQUE (DevKitC)
// ============================================================================
#elif defined(TARGET_ESP32_CLASSIC)

    /**
     * @def BOARD_NAME
     * @brief Nom lisible de la carte pour affichage
     */
    #define BOARD_NAME "ESP32 Classic DevKitC"
    
    // ========================================================================
    // LED Builtin - Optionnel (écran de diagnostic)
    // ========================================================================
    /**
     * @defgroup LED_BUILTIN LED Builtin
     * @brief Configuration de la LED bleue intégrée
     * @{
     */
    #define PIN_LED_BUILTIN 2    ///< GPIO 2 : LED bleue intégrée (heartbeat visuel)
    /** @} */
    
    // ========================================================================
    // Bus I2C - Pour l'écran OLED SSD1306
    // ========================================================================
    /**
     * @defgroup I2C_OLED Bus I2C pour OLED
     * @brief Configuration des pins I2C pour l'écran OLED SSD1306
     * 
     * Pins I2C différentes sur ESP32 Classic par rapport à S3
     * @{
     */
    #define PIN_I2C_SDA 21       ///< GPIO 21 : Données I2C
    #define PIN_I2C_SCL 22       ///< GPIO 22 : Horloge I2C
    /** @} */

    // ========================================================================
    // Bus SPI - Pour l'écran TFT ILI9341 (écran couleur)
    // ========================================================================
    /**
     * @defgroup SPI_TFT Bus SPI pour TFT ILI9341
     * @brief Configuration des pins SPI pour écran TFT ILI9341
     * 
     * Pins SPI différentes sur ESP32 Classic par rapport à S3.
     * Note : On utilise ILI9341 sur Classic, ST7789 sur S3
     * @{
     */
    #define TFT_MOSI  23      ///< GPIO 23 : Données SPI / SDA du TFT
    #define TFT_SCLK  18      ///< GPIO 18 : Horloge SPI / SCL du TFT
    #define TFT_CS    19      ///< GPIO 19 : Sélection puce
    #define TFT_DC    27      ///< GPIO 27 : Données/Commande
    #define TFT_RST   26      ///< GPIO 26 : Réinitialisation
    #define TFT_BL    13      ///< GPIO 13 : Rétroéclairage
    /** @} */

#else
    #error "Aucune cible definie ! Utilisez -D TARGET_ESP32_S3 ou -D TARGET_ESP32_CLASSIC dans platformio.ini"
#endif

// ============================================================================
// NOTES DE CONNEXION POUR DÉBUTANTS
// ============================================================================
/**
 * @page beginnerGuide Guide de connexion pour débutants
 * 
 * @section connector Connaissance de base
 * - **GPIO** : General Purpose Input/Output - pins que vous pouvez programmer
 * - **I2C** : Inter-Integrated Circuit - protocole pour communiquer lentement avec des composants
 * - **SPI** : Serial Peripheral Interface - protocole plus rapide que I2C
 * - **GND** : Ground (masse) - référence électrique, TOUJOURS à connecter
 * - **3V3** : Alimentation 3.3V - tension pour alimenter les modules (⚠️ pas 5V sur ESP32!)
 * 
 * @section connectionSteps Étapes de connexion physique
 * 1. **Coupez l'alimentation** : Débranchez le câble USB avant de brancher/débrancher des composants
 * 2. **Identifiez les broches** : Comptez depuis le bord ou repérez les numéros sérigraphiés
 * 3. **Connectez en respectant l'ordre** : D'abord GND, puis les données, puis VCC
 * 4. **Vérifiez deux fois** : Triple-check avant de brancher l'alimentation !
 * 5. **Mettez sous tension** : Branchez le câble USB
 * 
 * @section troubleshooting Dépannage
 * - Écran reste noir ? Vérifiez le rétroéclairage (pin BL) et l'adresse I2C
 * - OLED ne s'allume pas ? Testez les adresses 0x3C et 0x3D
 * - TFT affiche n'importe quoi ? Revérifiez tous les câblages SPI
 * 
 * @see docs/PIN_MAPPING.md pour les schémas détaillés et images de câblage
 */

#endif // BOARD_CONFIG_H
