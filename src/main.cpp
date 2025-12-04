#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <Adafruit_NeoPixel.h>
#include <OneButton.h>

#include "config.h"
#include "board_config.h"
#include "secrets.h"

// --- OBJETS ---
WiFiMulti wifiMulti;
OneButton btn(PIN_BUTTON_BOOT, true); // true = Active Low (Bouton poussoir standard vers GND)

#ifdef HAS_NEOPIXEL
    Adafruit_NeoPixel pixels(NEOPIXEL_NUM, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
#endif

// --- VARIABLES GLOBALES ---
unsigned long previousMillis = 0;
const long interval = 1000;      // Vitesse du clignotement Heartbeat
bool ledState = false;

// --- CALLBACKS BOUTON ---

// Simple Clic
void handleClick() {
    LOG_PRINTLN(">> Clic détecté ! Action simple.");
    // Exemple : Changer un mode, allumer une lampe...
}

// Appui Long (ex: 1 seconde)
void handleLongPress() {
    LOG_PRINTLN(">> APPUI LONG détecté ! (Ex: Reset WiFi ou Reboot)");
    
    #ifdef HAS_NEOPIXEL
        pixels.setPixelColor(0, pixels.Color(255, 0, 255)); // Flash Violet pour confirmer
        pixels.show();
        delay(500); // Petite pause visuelle exceptionnelle
    #endif
}

// --- FONCTIONS WIFI ---
void setupWifi() {
    LOG_PRINTLN("--- Démarrage WiFiMulti ---");
    int numNetworks = sizeof(WIFI_NETWORKS) / sizeof(WIFI_NETWORKS[0]);
    
    for (int i = 0; i < numNetworks; i++) {
        wifiMulti.addAP(WIFI_NETWORKS[i][0], WIFI_NETWORKS[i][1]);
    }

    LOG_PRINT("Connexion WiFi en cours...");
    
    #ifdef HAS_NEOPIXEL
        pixels.setPixelColor(0, pixels.Color(50, 50, 0)); // Jaune
        pixels.show();
    #endif

    // On ne bloque pas indéfiniment ici, on laisse le loop gérer si ça prend du temps
    // Mais on fait une première tentative rapide
    if(wifiMulti.run() == WL_CONNECTED) {
        LOG_PRINTLN(" OK !");
        LOG_PRINT("IP: "); LOG_PRINTLN(WiFi.localIP());
        #ifdef HAS_NEOPIXEL
            pixels.setPixelColor(0, pixels.Color(0, 50, 0)); // Vert
            pixels.show();
        #endif
    } else {
        LOG_PRINTLN(" En attente...");
    }
}

// --- SETUP ---
void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(1000); // Wait for Serial USB

    LOG_PRINTLN("\n=== DEMARRAGE PROJET ===");
    LOG_PRINTF("Board: %s\n", BOARD_NAME);

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
}

// --- LOOP (DOIT TOURNER VITE) ---
void loop() {
    // 1. Surveillance Bouton (CRITIQUE : doit être appelé tout le temps)
    btn.tick();

    // 2. Gestion WiFi (Reconnexion auto)
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