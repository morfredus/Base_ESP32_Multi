#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>
#include <Adafruit_NeoPixel.h>
#include <OneButton.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "config.h"
#include "board_config.h"
#include "secrets.h"

// --- OBJETS ---
WiFiMulti wifiMulti;
WebServer server(80);
OneButton btn(PIN_BUTTON_BOOT, true); // true = Active Low (Bouton poussoir standard vers GND)

#ifdef HAS_NEOPIXEL
    Adafruit_NeoPixel pixels(NEOPIXEL_NUM, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
#endif

#ifdef HAS_OLED
    Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);
#endif

// --- VARIABLES GLOBALES ---
unsigned long previousMillis = 0;
const long interval = 1000;      // Vitesse du clignotement Heartbeat
bool ledState = false;

// --- FONCTIONS SERVEUR WEB ---
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
    
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta charset='utf-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>" + String(PROJECT_NAME) + "</title>";
    html += "<style>";
    html += "* {margin:0;padding:0;box-sizing:border-box;}";
    html += "body{font-family:'Segoe UI',Arial,sans-serif;background:linear-gradient(135deg,#667eea 0%,#764ba2 100%);min-height:100vh;padding:20px;}";
    html += ".container{max-width:900px;margin:0 auto;}";
    html += ".header{text-align:center;color:white;margin-bottom:30px;}";
    html += ".header h1{font-size:2.5em;margin-bottom:5px;text-shadow:2px 2px 4px rgba(0,0,0,0.3);}";
    html += ".header p{font-size:1.1em;opacity:0.95;}";
    html += ".cards-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(280px,1fr));gap:20px;margin-bottom:20px;}";
    html += ".card{background:white;border-radius:12px;padding:20px;box-shadow:0 8px 16px rgba(0,0,0,0.1);transition:all 0.3s ease;}";
    html += ".card:hover{transform:translateY(-5px);box-shadow:0 12px 24px rgba(0,0,0,0.15);}";
    html += ".card-title{font-size:1.3em;font-weight:bold;margin-bottom:15px;color:#333;border-bottom:2px solid #667eea;padding-bottom:10px;}";
    html += ".card-item{display:flex;justify-content:space-between;padding:8px 0;border-bottom:1px solid #eee;font-size:0.95em;}";
    html += ".card-item:last-child{border-bottom:none;}";
    html += ".card-label{font-weight:600;color:#555;}";
    html += ".card-value{color:#333;text-align:right;}";
    html += ".card-value.mono{font-family:monospace;font-size:0.9em;}";
    html += ".progress-container{margin-top:12px;}";
    html += ".progress-label{display:flex;justify-content:space-between;font-size:0.85em;margin-bottom:5px;color:#666;}";
    html += ".progress-bar{background:#e0e0e0;height:10px;border-radius:5px;overflow:hidden;}";
    html += ".progress-fill{background:linear-gradient(90deg,#667eea,#764ba2);height:100%;transition:width 0.3s;}";
    html += ".actions{display:flex;gap:10px;justify-content:center;flex-wrap:wrap;}";
    html += "button{background:linear-gradient(135deg,#667eea,#764ba2);color:white;border:none;padding:12px 24px;border-radius:6px;cursor:pointer;font-size:1em;font-weight:600;transition:all 0.3s;}";
    html += "button:hover{transform:scale(1.05);box-shadow:0 4px 12px rgba(102,126,234,0.4);}";
    html += "button.danger{background:linear-gradient(135deg,#f093fb,#f5576c);}";
    html += "button.danger:hover{box-shadow:0 4px 12px rgba(245,87,108,0.4);}";
    html += ".signal-bar{display:inline-block;width:12px;height:12px;border-radius:2px;background:#667eea;margin-right:3px;}";
    html += ".signal-bar.active{background:#4CAF50;}";
    html += "</style></head><body>";
    html += "<div class='container'>";
    html += "<div class='header'><h1>" + String(PROJECT_NAME) + "</h1><p>Version " + String(PROJECT_VERSION) + "</p></div>";
    
    html += "<div class='cards-grid'>";
    
    // CARTE 1: Matériel
    html += "<div class='card'>";
    html += "<div class='card-title'>⚡ Matériel</div>";
    html += "<div class='card-item'><span class='card-label'>Board:</span><span class='card-value'>" + String(BOARD_NAME) + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>Chip ID:</span><span class='card-value mono'>0x" + String(chipId, HEX) + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>SDK:</span><span class='card-value'>" + String(ESP.getSdkVersion()) + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>CPU:</span><span class='card-value'>" + String(cpuFreq) + " MHz (2 cœurs)</span></div>";
    html += "</div>";
    
    // CARTE 2: Mémoire Flash
    html += "<div class='card'>";
    html += "<div class='card-title'>💾 Flash</div>";
    html += "<div class='card-item'><span class='card-label'>Taille:</span><span class='card-value'>" + String(flashSize / (1024 * 1024)) + " MB</span></div>";
    html += "<div class='card-item'><span class='card-label'>Vitesse:</span><span class='card-value'>" + String(flashSpeed) + " MHz</span></div>";
    html += "<div class='card-item'><span class='card-label'>Mode:</span><span class='card-value'>QIO</span></div>";
    html += "</div>";
    
    // CARTE 3: RAM (Heap)
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
    
    // CARTE 4: PSRAM (si disponible)
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
    
    // CARTE 5: WiFi
    html += "<div class='card'>";
    html += "<div class='card-title'>📶 Réseau WiFi</div>";
    html += "<div class='card-item'><span class='card-label'>SSID:</span><span class='card-value'>" + WiFi.SSID() + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>IP:</span><span class='card-value mono'>" + WiFi.localIP().toString() + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>MAC:</span><span class='card-value mono'>" + WiFi.macAddress() + "</span></div>";
    int rssi = WiFi.RSSI();
    int signalPercent = (rssi + 100) * 2; // Approx -50dBm -> 100%, -100dBm -> 0%
    signalPercent = (signalPercent > 100) ? 100 : (signalPercent < 0) ? 0 : signalPercent;
    html += "<div class='card-item'><span class='card-label'>Signal:</span><span class='card-value'>" + String(rssi) + " dBm</span></div>";
    html += "<div class='progress-container'>";
    html += "<div class='progress-label'><span>Force signal</span><span>" + String(signalPercent) + "%</span></div>";
    html += "<div class='progress-bar'><div class='progress-fill' style='width:" + String(signalPercent) + "%;'></div></div>";
    html += "</div></div>";
    
    // CARTE 6: Système
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
    
    // CARTE 7: Réseau Détaillé
    html += "<div class='card'>";
    html += "<div class='card-title'>🔗 Détails Réseau</div>";
    html += "<div class='card-item'><span class='card-label'>Masque:</span><span class='card-value mono'>" + WiFi.subnetMask().toString() + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>Passerelle:</span><span class='card-value mono'>" + WiFi.gatewayIP().toString() + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>DNS:</span><span class='card-value mono'>" + WiFi.dnsIP().toString() + "</span></div>";
    html += "</div>";
    
    // Boutons d'action
    html += "<div class='actions' style='margin-top:30px;'>";
    html += "<button onclick='location.reload()'>🔄 Actualiser</button>";
    html += "<button class='danger' onclick='if(confirm(\"Êtes-vous sûr de vouloir redémarrer ?\")) fetch(\"/reboot\")'>🔴 Redémarrer</button>";
    html += "</div>";
    
    html += "</div></body></html>";
    
    server.send(200, "text/html", html);
    LOG_PRINTLN("Page web servie");
}

void handleReboot() {
    server.send(200, "text/plain", "Red\u00e9marrage en cours...");
    LOG_PRINTLN("Red\u00e9marrage demand\u00e9 via web");
    delay(1000);
    ESP.restart();
}

void handleNotFound() {
    server.send(404, "text/plain", "Page non trouv\u00e9e");
}

// --- FONCTIONS OLED ---
#ifdef HAS_OLED
void setupOled() {
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    
    if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        LOG_PRINTLN("Erreur: OLED non détecté !");
        return;
    }
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(PROJECT_NAME);
    display.println("v" + String(PROJECT_VERSION));
    display.display();
    LOG_PRINTLN("OLED initialisé avec succès");
}

void updateOledStatus(const char* status, int progress = -1) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    
    // Titre
    display.println(PROJECT_NAME);
    display.println("v" + String(PROJECT_VERSION));
    display.println();
    
    // Statut
    display.println(status);
    
    // Barre de progression si demandée
    if (progress >= 0 && progress <= 100) {
        int barWidth = (OLED_WIDTH - 4) * progress / 100;
        display.drawRect(0, 40, OLED_WIDTH, 10, SSD1306_WHITE);
        display.fillRect(2, 42, barWidth, 6, SSD1306_WHITE);
        display.setCursor(0, 52);
        display.print(progress);
        display.println("%");
    }
    
    display.display();
}

void updateOledConnected(const char* ssid, IPAddress ip) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    
    // Titre
    display.println(PROJECT_NAME);
    display.println("v" + String(PROJECT_VERSION));
    display.println();
    
    // Infos WiFi
    display.println("WiFi: " + String(ssid));
    display.println();
    display.print("IP: ");
    display.println(ip);
    
    display.display();
}
#endif

// --- CALLBACKS BOUTON ---

// Simple Clic
void handleClick() {
    LOG_PRINTLN(">> Clic détecté ! Action simple.");
    // Exemple : Changer un mode, allumer une lampe...
}

// Appui Long (ex: 1 seconde)
void handleLongPress() {
    LOG_PRINTLN(">> APPUI LONG détecté ! Redémarrage...");
    
    #ifdef HAS_NEOPIXEL
        pixels.setPixelColor(0, pixels.Color(255, 0, 255)); // Flash Violet pour confirmer
        pixels.show();
    #endif
    
    #ifdef HAS_OLED
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0, 20);
        display.println("REBOOT...");
        display.display();
    #endif
    
    delay(1000); // Pause pour afficher le message
    ESP.restart();
}

// --- FONCTIONS WIFI ---
void setupWifi() {
    LOG_PRINTLN("--- Démarrage WiFiMulti ---");
    int numNetworks = sizeof(WIFI_NETWORKS) / sizeof(WIFI_NETWORKS[0]);
    
    for (int i = 0; i < numNetworks; i++) {
        wifiMulti.addAP(WIFI_NETWORKS[i][0], WIFI_NETWORKS[i][1]);
    }

    LOG_PRINT("Connexion WiFi en cours...");
    
    #ifdef HAS_OLED
        updateOledStatus("Connexion WiFi...", 0);
    #endif
    
    #ifdef HAS_NEOPIXEL
        pixels.setPixelColor(0, pixels.Color(50, 50, 0)); // Jaune
        pixels.show();
    #endif

    // Tentatives de connexion avec affichage de la progression
    int attempts = 0;
    const int maxAttempts = 20;
    
    while(wifiMulti.run() != WL_CONNECTED && attempts < maxAttempts) {
        delay(500);
        attempts++;
        int progress = (attempts * 100) / maxAttempts;
        
        #ifdef HAS_OLED
            updateOledStatus("Connexion WiFi...", progress);
        #endif
        
        LOG_PRINT(".");
    }
    
    if(wifiMulti.run() == WL_CONNECTED) {
        LOG_PRINTLN(" OK !");
        LOG_PRINT("SSID: "); LOG_PRINTLN(WiFi.SSID());
        LOG_PRINT("IP: "); LOG_PRINTLN(WiFi.localIP());
        
        #ifdef HAS_OLED
            updateOledConnected(WiFi.SSID().c_str(), WiFi.localIP());
        #endif
        
        #ifdef HAS_NEOPIXEL
            pixels.setPixelColor(0, pixels.Color(0, 50, 0)); // Vert
            pixels.show();
        #endif
    } else {
        LOG_PRINTLN(" Echec !");
        #ifdef HAS_OLED
            updateOledStatus("WiFi: Echec", 100);
        #endif
    }
}

// --- SETUP ---
void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(1000); // Wait for Serial USB

    LOG_PRINTLN("\n=== DEMARRAGE PROJET ===");
    LOG_PRINTF("Board: %s\n", BOARD_NAME);
    LOG_PRINTF("Version: %s\n", PROJECT_VERSION);

    // Init OLED
    #ifdef HAS_OLED
        setupOled();
    #endif

    // Init NeoPixel
    #ifdef HAS_NEOPIXEL
        pixels.begin();
        pixels.setBrightness(30);
        pixels.clear();
        pixels.show();
    #endif

    // Init LED Builtin (si existante)
    #ifdef PIN_LED_BUILTIN
        pinMode(PIN_LED_BUILTIN, OUTPUT);
    #endif

    // Config Bouton
    btn.attachClick(handleClick);
    btn.attachLongPressStart(handleLongPress);
    btn.setPressMs(1000); // Durée pour considérer un appui long (ms)

    setupWifi();
    
    // Démarrage Serveur Web
    if(WiFi.status() == WL_CONNECTED) {
        server.on("/", handleRoot);
        server.on("/reboot", handleReboot);
        server.onNotFound(handleNotFound);
        server.begin();
        LOG_PRINTLN("Serveur web d\u00e9marr\u00e9 sur http://" + WiFi.localIP().toString());
        
        #ifdef HAS_OLED
            delay(2000); // Laisser le temps de voir l'IP
        #endif
    }
}

// --- LOOP (DOIT TOURNER VITE) ---
void loop() {
    // 1. Surveillance Bouton (CRITIQUE : doit être appelé tout le temps)
    btn.tick();

    // 2. Gestion Serveur Web
    server.handleClient();

    // 3. Gestion WiFi (Reconnexion auto)
    if(wifiMulti.run() != WL_CONNECTED) {
        // Optionnel : Gestion LED rouge si perte de connexion
    }

    // 3. Heartbeat Non-Bloquant (remplace delay)
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        ledState = !ledState;

        // Action visuelle selon la carte
        #ifdef PIN_LED_BUILTIN
            digitalWrite(PIN_LED_BUILTIN, ledState);
        #endif

        #ifdef HAS_NEOPIXEL
            // Petit battement de coeur bleu sur le pixel sans effacer la couleur de fond
            // (Note: pour faire pro, on pourrait complexifier, mais ici on fait simple)
            if(wifiMulti.run() == WL_CONNECTED) {
                pixels.setPixelColor(0, ledState ? pixels.Color(0, 50, 0) : pixels.Color(0, 10, 0)); // Vert fort / Vert faible
            } else {
                 pixels.setPixelColor(0, ledState ? pixels.Color(50, 0, 0) : pixels.Color(10, 0, 0)); // Rouge fort / faible
            }
            pixels.show();
        #endif
    }
}