#ifndef WEB_PAGES_H
#define WEB_PAGES_H

/**
 * @file web_pages.h
 * @brief Générateur des pages HTML de l'interface web
 * @version 0.6.0
 * 
 * Module dédié à la génération du contenu HTML de l'interface web.
 * Contient les fonctions pour construire les différentes cartes et sections.
 */

#include "web_styles.h"

/**
 * @brief Génère la page HTML complète du tableau de bord
 * @param chipId Identifiant unique du chip ESP32
 * @param flashSize Taille de la mémoire flash en bytes
 * @param flashSpeed Vitesse de la flash en MHz
 * @param heapSize Taille du heap total en bytes
 * @param freeHeap Heap libre en bytes
 * @param psramSize Taille PSRAM en bytes (0 si non présent)
 * @param freePsram PSRAM libre en bytes
 * @param cpuFreq Fréquence CPU en MHz
 * @return String contenant le HTML complet
 */
String generateDashboardPage(
    uint32_t chipId,
    uint32_t flashSize,
    uint32_t flashSpeed,
    uint32_t heapSize,
    uint32_t freeHeap,
    uint32_t psramSize,
    uint32_t freePsram,
    uint32_t cpuFreq
) {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta charset='utf-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>" + String(PROJECT_NAME) + "</title>";
    html += "<style>" + String(WEB_STYLES) + "</style>";
    html += "</head><body>";
    html += "<div class='container'>";
    
    // --- HEADER ---
    html += "<div class='header'>";
    html += "<h1>" + String(PROJECT_NAME) + "</h1>";
    html += "<p>Version " + String(PROJECT_VERSION) + "</p>";
    html += "</div>";
    
    html += "<div class='cards-grid'>";
    
    // --- CARTE 1: Matériel ---
    html += "<div class='card'>";
    html += "<div class='card-title'>⚡ Matériel</div>";
    html += "<div class='card-item'><span class='card-label'>Board:</span><span class='card-value'>" + String(BOARD_NAME) + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>Chip ID:</span><span class='card-value mono'>0x" + String(chipId, HEX) + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>SDK:</span><span class='card-value'>" + String(ESP.getSdkVersion()) + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>CPU:</span><span class='card-value'>" + String(cpuFreq) + " MHz (2 cœurs)</span></div>";
    html += "</div>";
    
    // --- CARTE 2: Mémoire Flash ---
    html += "<div class='card'>";
    html += "<div class='card-title'>💾 Flash</div>";
    html += "<div class='card-item'><span class='card-label'>Taille:</span><span class='card-value'>" + String(flashSize / (1024 * 1024)) + " MB</span></div>";
    html += "<div class='card-item'><span class='card-label'>Vitesse:</span><span class='card-value'>" + String(flashSpeed) + " MHz</span></div>";
    html += "<div class='card-item'><span class='card-label'>Mode:</span><span class='card-value'>QIO</span></div>";
    html += "</div>";
    
    // --- CARTE 3: RAM (Heap) ---
    html += "<div class='card'>";
    html += "<div class='card-title'>📊 RAM (Heap)</div>";
    uint32_t heapUsed = heapSize - freeHeap;
    float heapPercent = (float)heapUsed / heapSize * 100;
    html += "<div class='card-item'><span class='card-label'>Total:</span><span class='card-value'>" + String(heapSize / 1024) + " KB</span></div>";
    html += "<div class='card-item'><span class='card-label'>Libre:</span><span class='card-value'>" + String(freeHeap / 1024) + " KB</span></div>";
    html += "<div class='progress-container'>";
    html += "<div class='progress-label'><span>Utilisation</span><span>" + String(heapPercent, 1) + "%</span></div>";
    html += "<div class='progress-bar'><div class='progress-fill' style='width:" + String(heapPercent) + "%;'></div></div>";
    html += "</div></div>";
    
    // --- CARTE 4: PSRAM (si disponible) ---
    if(psramSize > 0) {
        uint32_t psramUsed = psramSize - freePsram;
        float psramPercent = (float)psramUsed / psramSize * 100;
        html += "<div class='card'>";
        html += "<div class='card-title'>🔋 PSRAM</div>";
        html += "<div class='card-item'><span class='card-label'>Total:</span><span class='card-value'>" + String(psramSize / (1024 * 1024)) + " MB</span></div>";
        html += "<div class='card-item'><span class='card-label'>Libre:</span><span class='card-value'>" + String(freePsram / 1024) + " KB</span></div>";
        html += "<div class='progress-container'>";
        html += "<div class='progress-label'><span>Utilisation</span><span>" + String(psramPercent, 1) + "%</span></div>";
        html += "<div class='progress-bar'><div class='progress-fill' style='width:" + String(psramPercent) + "%;'></div></div>";
        html += "</div></div>";
    }
    
    // --- CARTE 5: WiFi ---
    html += "<div class='card'>";
    html += "<div class='card-title'>📶 Réseau WiFi</div>";
    html += "<div class='card-item'><span class='card-label'>SSID:</span><span class='card-value'>" + WiFi.SSID() + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>IP:</span><span class='card-value mono'>" + WiFi.localIP().toString() + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>MAC:</span><span class='card-value mono'>" + WiFi.macAddress() + "</span></div>";
    int rssi = WiFi.RSSI();
    int signalPercent = (rssi + 100) * 2;
    signalPercent = (signalPercent > 100) ? 100 : (signalPercent < 0) ? 0 : signalPercent;
    html += "<div class='card-item'><span class='card-label'>Signal:</span><span class='card-value'>" + String(rssi) + " dBm</span></div>";
    html += "<div class='progress-container'>";
    html += "<div class='progress-label'><span>Force signal</span><span>" + String(signalPercent) + "%</span></div>";
    html += "<div class='progress-bar'><div class='progress-fill' style='width:" + String(signalPercent) + "%;'></div></div>";
    html += "</div></div>";
    
    // --- CARTE 6: Système ---
    html += "<div class='card'>";
    html += "<div class='card-title'>⏱️ Système</div>";
    unsigned long uptime = millis() / 1000;
    unsigned long hours = uptime / 3600;
    unsigned long minutes = (uptime % 3600) / 60;
    unsigned long seconds = uptime % 60;
    html += "<div class='card-item'><span class='card-label'>Uptime:</span><span class='card-value'>" + String(hours) + "h " + String(minutes) + "m " + String(seconds) + "s</span></div>";
    html += "<div class='card-item'><span class='card-label'>Température:</span><span class='card-value'>" + String(temperatureRead(), 1) + " °C</span></div>";
    html += "</div>";
    
    html += "</div>";  // Fin cards-grid
    
    // --- CARTE 7: Réseau Détaillé ---
    html += "<div class='card'>";
    html += "<div class='card-title'>🔗 Détails Réseau</div>";
    html += "<div class='card-item'><span class='card-label'>Masque:</span><span class='card-value mono'>" + WiFi.subnetMask().toString() + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>Passerelle:</span><span class='card-value mono'>" + WiFi.gatewayIP().toString() + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>DNS:</span><span class='card-value mono'>" + WiFi.dnsIP().toString() + "</span></div>";
    html += "</div>";
    
    // --- BOUTONS D'ACTION ---
    html += "<div class='actions'>";
    html += "<button onclick='location.reload()'>🔄 Actualiser</button>";
    html += "<button class='danger' onclick='if(confirm(\"Êtes-vous sûr de vouloir redémarrer ?\")) fetch(\"/reboot\")'>🔴 Redémarrer</button>";
    html += "</div>";
    
    html += "</div></body></html>";
    
    return html;
}

#endif // WEB_PAGES_H
