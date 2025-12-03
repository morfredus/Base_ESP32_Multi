#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include <Arduino.h>

// Bouton BOOT standard sur tous les ESP32 (DevKit & S3)
#define PIN_BUTTON_BOOT 0 

// ------------------------------------------------------------------
// CONFIGURATION ESP32-S3
// ------------------------------------------------------------------
#if defined(TARGET_ESP32_S3)
    #define BOARD_NAME "ESP32-S3 DevKitC-1"
    
    #define HAS_NEOPIXEL
    #define PIN_NEOPIXEL    48
    #define NEOPIXEL_NUM    1
    
    #define PIN_I2C_SDA     42
    #define PIN_I2C_SCL     41
    
// ------------------------------------------------------------------
// CONFIGURATION ESP32 CLASSIQUE
// ------------------------------------------------------------------
#elif defined(TARGET_ESP32_CLASSIC)
    #define BOARD_NAME "ESP32 Classic DevKitC"
    
    #define PIN_LED_BUILTIN 2
    #define PIN_I2C_SDA     21
    #define PIN_I2C_SCL     22

#else
    #error "Aucune cible definie !"
#endif

#endif