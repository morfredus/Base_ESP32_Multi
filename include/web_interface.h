#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

/**
 * @file web_interface.h
 * @brief Gestion de l'interface web et des handlers du serveur
 * @version 0.8.9
 * @date 2026-01-04
 *
 * CORRECTIF v0.8.9:
 * - Utilisation du pointeur pServer au lieu de server statique
 * - Compatible avec l'initialisation differee dans main.cpp
 *
 * Module dedie a la gestion des routes HTTP et handlers du serveur web.
 * Contient les callbacks pour les differents endpoints de l'API web.
 */

#include "web_pages.h"

// ===================================================================
// SECTION 1 : DECLARATION DU SERVEUR WEB
// ===================================================================
// CORRECTIF v0.8.9: Utilise un pointeur pour eviter l'initialisation statique
extern WebServer* pServer;

// ===================================================================
// SECTION 2 : HANDLERS HTTP
// ===================================================================

/**
 * @brief Handler pour la page d'accueil (GET /)
 * Genere et envoie le tableau de bord avec toutes les informations systeme
 */
inline void handleRoot() {
    if (pServer == nullptr) return;

    // Recuperation des informations techniques
    uint32_t chipId = 0;
    for(int i=0; i<17; i=i+8) {
        chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }

    uint32_t flashSize = ESP.getFlashChipSize();
    uint32_t flashSpeed = ESP.getFlashChipSpeed() / 1000000;
    uint32_t heapSize = ESP.getHeapSize();
    uint32_t freeHeap = ESP.getFreeHeap();
    uint32_t psramSize = ESP.getPsramSize();
    uint32_t freePsram = ESP.getFreePsram();
    uint32_t cpuFreq = ESP.getCpuFreqMHz();

    // Generation de la page HTML
    String html = generateDashboardPage(
        chipId, flashSize, flashSpeed,
        heapSize, freeHeap,
        psramSize, freePsram,
        cpuFreq
    );

    pServer->send(200, "text/html", html);
    LOG_PRINTLN("Page web servie");
}

/**
 * @brief Handler pour le redemarrage (GET /reboot)
 */
inline void handleReboot() {
    if (pServer == nullptr) return;
    pServer->send(200, "text/plain", "Redemarrage en cours...");
    LOG_PRINTLN("Redemarrage demande via web");
    delay(1000);
    ESP.restart();
}

/**
 * @brief Handler pour les pages non trouvees (404)
 */
inline void handleNotFound() {
    if (pServer == nullptr) return;
    pServer->send(404, "text/plain", "Page non trouvee");
}

// ===================================================================
// SECTION 3 : INITIALISATION DU SERVEUR WEB
// ===================================================================

/**
 * @brief Initialise les routes du serveur web
 * @note Appeler dans setup() apres la connexion WiFi
 */
inline void setupWebServer() {
    if (pServer == nullptr) {
        LOG_PRINTLN("[!!] WebServer non initialise!");
        return;
    }

    pServer->on("/", handleRoot);
    pServer->on("/reboot", handleReboot);
    pServer->onNotFound(handleNotFound);
    pServer->begin();
    LOG_PRINTLN("Serveur web demarre sur http://" + WiFi.localIP().toString());
}

#endif // WEB_INTERFACE_H
