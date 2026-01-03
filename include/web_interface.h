#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

/**
 * @file web_interface.h
 * @brief Gestion de l'interface web et des handlers du serveur
 * @version 0.9.0
 *
 * Module dédié à la gestion des routes HTTP et handlers du serveur web.
 * Contient les callbacks pour les différents endpoints de l'API web.
 * Inclut maintenant le support OTA avec upload de firmware.
 */

#include <Update.h>
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
 * @brief Handler pour la page OTA (GET /ota)
 * Affiche la page d'upload de firmware
 */
void handleOTA() {
    String html = generateOTAPage();
    server.send(200, "text/html", html);
    LOG_PRINTLN("Page OTA servie");
}

/**
 * @brief Handler pour l'upload de firmware (POST /update)
 * Gère l'upload et la flash du firmware
 */
void handleFirmwareUpload() {
    HTTPUpload& upload = server.upload();

    if (upload.status == UPLOAD_FILE_START) {
        LOG_PRINTF("Update: %s\n", upload.filename.c_str());

        // Start with max available size
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
            Update.printError(Serial);
        }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
        // Write firmware chunk
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
            Update.printError(Serial);
        }
    } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {
            LOG_PRINTF("Update Success: %u bytes\nRebooting...\n", upload.totalSize);
        } else {
            Update.printError(Serial);
        }
    }
}

/**
 * @brief Handler pour la fin de l'upload (GET /update après POST)
 * Renvoie le statut de l'upload
 */
void handleFirmwareUploadComplete() {
    if (Update.hasError()) {
        server.send(200, "text/html", generateOTAResultPage(false, Update.errorString()));
    } else {
        server.send(200, "text/html", generateOTAResultPage(true, ""));

        // Option 1: Auto-reboot après 1 seconde (défaut)
        delay(1000);
        ESP.restart();

        // Option 2: Redémarrage manuel via bouton web (commentez les 2 lignes ci-dessus et décommentez ci-dessous)
        // LOG_PRINTLN("✅ OTA réussi. Utilisez le bouton Reboot pour redémarrer.");
        // La page affichera un bouton "Reboot Now"
    }
}

/**
 * @brief Handler pour le redémarrage (POST /reboot)
 * Redémarre l'ESP32 (sans confirmation popup)
 */
void handleReboot() {
    server.send(200, "application/json", "{\"status\":\"ok\",\"message\":\"Reboot in progress...\"}");
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
    server.on("/ota", handleOTA);
    server.on("/update", HTTP_POST, handleFirmwareUploadComplete, handleFirmwareUpload);
    server.on("/reboot", HTTP_POST, handleReboot);
    server.onNotFound(handleNotFound);
    server.begin();
    LOG_PRINTLN("Serveur web démarré sur http://" + WiFi.localIP().toString());
    LOG_PRINTLN("  - Page OTA : http://" + WiFi.localIP().toString() + "/ota");
}

#endif // WEB_INTERFACE_H
