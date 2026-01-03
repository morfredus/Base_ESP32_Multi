#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

/**
 * @file web_interface.h
 * @brief Gestion de l'interface web et des handlers du serveur
 * @version 0.6.0
 * 
 * Module dédié à la gestion des routes HTTP et handlers du serveur web.
 * Contient les callbacks pour les différents endpoints de l'API web.
 */

#include "web_pages.h"

// Déclaration du serveur web (défini dans main.cpp)
extern WebServer server;

/**
 * @brief Handler pour la page d'accueil (GET /)
 * Génère et envoie le tableau de bord avec toutes les informations système
 */
void handleRoot() {
    // Récupération des informations techniques
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
    
    // Génération de la page HTML
    String html = generateDashboardPage(
        chipId, flashSize, flashSpeed,
        heapSize, freeHeap,
        psramSize, freePsram,
        cpuFreq
    );
    
    server.send(200, "text/html", html);
    LOG_PRINTLN("Page web servie");
}

/**
 * @brief Handler pour le redémarrage (GET /reboot)
 * Redémarre l'ESP32 après confirmation
 */
void handleReboot() {
    server.send(200, "text/plain", "Redémarrage en cours...");
    LOG_PRINTLN("Redémarrage demandé via web");
    delay(1000);
    ESP.restart();
}

/**
 * @brief Handler pour les pages non trouvées (404)
 */
void handleNotFound() {
    server.send(404, "text/plain", "Page non trouvée");
}

/**
 * @brief Initialise les routes du serveur web
 * À appeler dans setup() après la connexion WiFi
 */
void setupWebServer() {
    server.on("/", handleRoot);
    server.on("/reboot", handleReboot);
    server.onNotFound(handleNotFound);
    server.begin();
    LOG_PRINTLN("Serveur web démarré sur http://" + WiFi.localIP().toString());
}

#endif // WEB_INTERFACE_H
