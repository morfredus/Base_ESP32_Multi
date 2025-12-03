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

#endif