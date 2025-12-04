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
#include "web_interface.h"

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

// --- FONCTIONS SERVEUR WEB (DÉLÉGUÉES AUX MODULES) ---
// Les fonctions handleRoot(), handleReboot(), handleNotFound() et setupWebServer()
// sont maintenant définies dans web_interface.h
// Voir : include/web_interface.h

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
        setupWebServer();  // Module délégué dans web_interface.h
        
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