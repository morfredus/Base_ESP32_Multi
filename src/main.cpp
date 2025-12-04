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
    html += "body{font-family:Arial,sans-serif;margin:20px;background:#f0f0f0;}";
    html += ".container{max-width:800px;margin:0 auto;background:white;padding:20px;border-radius:10px;box-shadow:0 2px 10px rgba(0,0,0,0.1);}";
    html += "h1{color:#333;border-bottom:2px solid #4CAF50;padding-bottom:10px;}";
    html += "h2{color:#555;border-bottom:1px solid #ddd;padding-bottom:5px;margin-top:20px;}";
    html += ".info{margin:10px 0;padding:10px;background:#e8f5e9;border-left:4px solid #4CAF50;display:flex;justify-content:space-between;}";
    html += ".info-mem{background:#fff3e0;border-left-color:#FF9800;}";
    html += ".info-cpu{background:#e3f2fd;border-left-color:#2196F3;}";
    html += ".label{font-weight:bold;color:#555;}";
    html += ".value{color:#333;}";
    html += "button{background:#4CAF50;color:white;border:none;padding:10px 20px;border-radius:5px;cursor:pointer;margin:5px;}";
    html += "button:hover{background:#45a049;}";
    html += ".progress-bar{background:#ddd;height:20px;border-radius:10px;overflow:hidden;margin-top:5px;}";
    html += ".progress-fill{background:#4CAF50;height:100%;transition:width 0.3s;}";
    html += "</style></head><body>";
    html += "<div class='container'>";
    html += "<h1>" + String(PROJECT_NAME) + " v" + String(PROJECT_VERSION) + "</h1>";
    
    // Section Matériel
    html += "<h2>⚡ Matériel</h2>";
    html += "<div class='info info-cpu'><span class='label'>Board:</span><span class='value'>" + String(BOARD_NAME) + "</span></div>";
    html += "<div class='info info-cpu'><span class='label'>Chip ID:</span><span class='value'>0x" + String(chipId, HEX) + "</span></div>";
    html += "<div class='info info-cpu'><span class='label'>SDK Version:</span><span class='value'>" + String(ESP.getSdkVersion()) + "</span></div>";
    html += "<div class='info info-cpu'><span class='label'>Fréquence CPU:</span><span class='value'>" + String(cpuFreq) + " MHz</span></div>";
    html += "<div class='info info-cpu'><span class='label'>Nombre de cœurs:</span><span class='value'>2</span></div>";
    
    // Section Mémoire Flash
    html += "<h2>💾 Mémoire Flash</h2>";
    html += "<div class='info info-mem'><span class='label'>Taille Flash:</span><span class='value'>" + String(flashSize / (1024 * 1024)) + " MB</span></div>";
    html += "<div class='info info-mem'><span class='label'>Vitesse Flash:</span><span class='value'>" + String(flashSpeed) + " MHz</span></div>";
    html += "<div class='info info-mem'><span class='label'>Mode Flash:</span><span class='value'>QIO</span></div>";
    
    // Section RAM
    html += "<h2>📊 Mémoire RAM</h2>";
    uint32_t heapUsed = heapSize - freeHeap;
    float heapPercent = (float)heapUsed / heapSize * 100;
    html += "<div class='info info-mem'>";
    html += "<span class='label'>Heap:</span>";
    html += "<span class='value'>" + String(freeHeap / 1024) + " KB libre / " + String(heapSize / 1024) + " KB (" + String(heapPercent, 1) + "% utilisé)</span>";
    html += "</div>";
    html += "<div class='progress-bar'><div class='progress-fill' style='width:" + String(heapPercent) + "%;'></div></div>";
    
    if(psramSize > 0) {
        uint32_t psramUsed = psramSize - freePsram;
        float psramPercent = (float)psramUsed / psramSize * 100;
        html += "<div class='info info-mem' style='margin-top:10px;'>";
        html += "<span class='label'>PSRAM:</span>";
        html += "<span class='value'>" + String(freePsram / 1024) + " KB libre / " + String(psramSize / (1024 * 1024)) + " MB (" + String(psramPercent, 1) + "% utilisé)</span>";
        html += "</div>";
        html += "<div class='progress-bar'><div class='progress-fill' style='width:" + String(psramPercent) + "%;'></div></div>";
    }
    
    // Section WiFi
    html += "<h2>📶 Réseau WiFi</h2>";
    html += "<div class='info'><span class='label'>SSID:</span><span class='value'>" + WiFi.SSID() + "</span></div>";
    html += "<div class='info'><span class='label'>Adresse IP:</span><span class='value'>" + WiFi.localIP().toString() + "</span></div>";
    html += "<div class='info'><span class='label'>Masque:</span><span class='value'>" + WiFi.subnetMask().toString() + "</span></div>";
    html += "<div class='info'><span class='label'>Passerelle:</span><span class='value'>" + WiFi.gatewayIP().toString() + "</span></div>";
    html += "<div class='info'><span class='label'>DNS:</span><span class='value'>" + WiFi.dnsIP().toString() + "</span></div>";
    html += "<div class='info'><span class='label'>MAC:</span><span class='value'>" + WiFi.macAddress() + "</span></div>";
    html += "<div class='info'><span class='label'>Signal:</span><span class='value'>" + String(WiFi.RSSI()) + " dBm</span></div>";
    
    // Section Système
    html += "<h2>⏱️ Système</h2>";
    html += "<div class='info'><span class='label'>Uptime:</span><span class='value'>" + String(millis() / 1000) + " secondes</span></div>";
    html += "<div class='info'><span class='label'>Température CPU:</span><span class='value'>" + String(temperatureRead(), 1) + " °C</span></div>";
    
    // Boutons d'action
    html += "<div style='margin-top:20px;text-align:center;'>";
    html += "<button onclick='location.reload()'>Actualiser</button>";
    html += "<button onclick='if(confirm(\"Red\u00e9marrer l\\\"ESP32 ?\")) fetch(\"/reboot\")' style='background:#FF5722;'>Red\u00e9marrer</button>";
    html += "</div></div></body></html>";
    
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