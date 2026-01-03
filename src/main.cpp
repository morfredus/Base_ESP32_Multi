#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>
#include <ArduinoOTA.h>
#include <Adafruit_NeoPixel.h>
#include <OneButton.h>

#include "config.h"
#include "board_config.h"
#include "secrets.h"
#include "web_interface.h"
#include "display.h"

// --- OBJETS ---
WiFiMulti wifiMulti;
WebServer server(80);
OneButton btn(BUTTON_BOOT, true); // true = Active Low (Bouton poussoir standard vers GND)
OneButton btn1(BUTTON_1, true);   // Bouton 1 - Cycle RGB
OneButton btn2(BUTTON_2, true);   // Bouton 2 - Buzzer

#ifdef HAS_NEOPIXEL
    Adafruit_NeoPixel pixels(NEOPIXEL_NUM, NEOPIXEL, NEO_GRB + NEO_KHZ800);
#endif

// --- VARIABLES GLOBALES ---
unsigned long previousMillis = 0;
const long interval = 1000;      // Vitesse du clignotement Heartbeat
bool ledState = false;
bool isRebooting = false;        // Flag pour empêcher les faux reboot

// État du cycle RGB pour le bouton 1 (0=Rouge, 1=Vert, 2=Bleu, 3=Blanc, 4=Éteint, puis boucle)
int rgbState = 0;

// --- FONCTIONS SERVEUR WEB (DÉLÉGUÉES AUX MODULES) ---
// Les fonctions handleRoot(), handleReboot(), handleNotFound() et setupWebServer()
// sont maintenant définies dans web_interface.h
// Voir : include/web_interface.h

// Les fonctions d'affichage OLED et ST7789 sont maintenant dans display.h / display.cpp

// --- CALLBACKS BOUTON ---

// Simple Clic du Bouton BOOT - Non utilisé pour le moment
void handleClick() {
    LOG_PRINTLN(">> Clic détecté sur le bouton BOOT ! Action simple.");
    // Exemple : Changer un mode, allumer une lampe...
}

// Appui Long du Bouton BOOT - Redémarrage avec barre de progression
void handleLongPress() {
    LOG_PRINTLN(">> APPUI LONG détecté ! Redémarrage...");
    isRebooting = true; // Marquer le début du reboot
    
    #ifdef HAS_NEOPIXEL
        pixels.setPixelColor(0, pixels.Color(128, 0, 128)); // Violet pour confirmer le reboot
        pixels.show();
    #endif
    
    #if defined(HAS_OLED) || defined(HAS_ST7789)
        #ifdef HAS_OLED
            display_oled.clearDisplay();
            display_oled.setTextSize(2);
            display_oled.setCursor(0, 20);
            display_oled.println("REBOOT...");
            display_oled.display();
        #endif
        
        #ifdef HAS_ST7789
            // Afficher barre de progression pendant 2 secondes
            display_tft.fillScreen(ST77XX_BLACK);
            display_tft.setTextSize(3);
            display_tft.setTextColor(ST77XX_RED);
            
            int16_t x1, y1;
            uint16_t w, h;
            display_tft.getTextBounds("REBOOT", 0, 0, &x1, &y1, &w, &h);
            int centerX = (TFT_WIDTH - w) / 2;
            display_tft.setCursor(centerX, 50);
            display_tft.println("REBOOT");

            // Barre de progression
            int barWidth = TFT_WIDTH - 40;
            int barHeight = 30;
            int barX = 20;
            int barY = 140;
            
            unsigned long startTime = millis();
            const unsigned long progressDuration = 2000; // 2 secondes
            bool completedFully = false;
            
            while (millis() - startTime < progressDuration) {
                // Vérifier si le bouton BOOT est toujours appuyé (Active Low = LOW = appuyé)
                bool buttonStillPressed = (digitalRead(BUTTON_BOOT) == LOW);
                
                if (!buttonStillPressed) {
                    // Bouton relâché avant la fin
                    LOG_PRINTLN(">> Bouton relâché ! Reboot annulé.");
                    isRebooting = false;
                    
                    // Restaurer l'affichage précédent (WiFi connecté ou non)
                    if(WiFi.status() == WL_CONNECTED) {
                        displayWifiConnected(WiFi.SSID().c_str(), WiFi.localIP());
                    } else {
                        displayWifiFailed();
                    }
                    break;
                }
                
                unsigned long elapsed = millis() - startTime;
                int progress = (elapsed * 100) / progressDuration;
                
                // Efface la zone de la barre
                display_tft.fillRect(barX - 2, barY - 2, barWidth + 4, barHeight + 20, ST77XX_BLACK);
                
                // Contour de la barre
                display_tft.drawRect(barX, barY, barWidth, barHeight, ST77XX_WHITE);
                
                // Remplissage de la progression
                int fillWidth = (barWidth - 4) * progress / 100;
                if (fillWidth > 0) {
                    display_tft.fillRect(barX + 2, barY + 2, fillWidth, barHeight - 4, ST77XX_MAGENTA);
                }
                
                // Pourcentage
                display_tft.setTextSize(2);
                display_tft.setTextColor(ST77XX_WHITE);
                String percentStr = String(progress) + "%";
                display_tft.getTextBounds(percentStr.c_str(), 0, 0, &x1, &y1, &w, &h);
                centerX = (TFT_WIDTH - w) / 2;
                display_tft.setCursor(centerX, barY + barHeight + 5);
                display_tft.println(percentStr);
                
                delay(50); // Mise à jour chaque 50ms
            }
            
            // Si on est sortie de la boucle normalement, la barre est à 100%
            if (isRebooting && digitalRead(BUTTON_BOOT) == LOW) {
                LOG_PRINTLN(">> Barre complétée à 100% ! Redémarrage...");
                completedFully = true;
            }
        #endif
    #endif
    
    // Seulement redémarrer si la barre a atteint 100%
    if (isRebooting && digitalRead(BUTTON_BOOT) == LOW) {
        delay(500); // Pause pour afficher le message
        ESP.restart();
    }
    isRebooting = false;
}

// Relâchement du Bouton BOOT - Gestion automatique via digitalRead pendant la barre
// (Fonction supprimée - la vérification se fait directement dans handleLongPress)

// Clic Bouton 1 - Cycle des couleurs RGB
void handleButton1Click() {
    LOG_PRINT(">> Bouton 1 cliqué ! État RGB: ");
    LOG_PRINTLN(rgbState);
    
    // Cycle: Rouge (0) -> Vert (1) -> Bleu (2) -> Blanc (3) -> Éteint (4) -> Rouge (0)
    #ifdef HAS_LED_RGB
        switch (rgbState) {
            case 0: // Rouge
                digitalWrite(LED_RED, HIGH);
                digitalWrite(LED_GREEN, LOW);
                digitalWrite(LED_BLUE, LOW);
                LOG_PRINTLN("   -> ROUGE");
                break;
            case 1: // Vert
                digitalWrite(LED_RED, LOW);
                digitalWrite(LED_GREEN, HIGH);
                digitalWrite(LED_BLUE, LOW);
                LOG_PRINTLN("   -> VERT");
                break;
            case 2: // Bleu
                digitalWrite(LED_RED, LOW);
                digitalWrite(LED_GREEN, LOW);
                digitalWrite(LED_BLUE, HIGH);
                LOG_PRINTLN("   -> BLEU");
                break;
            case 3: // Blanc
                digitalWrite(LED_RED, HIGH);
                digitalWrite(LED_GREEN, HIGH);
                digitalWrite(LED_BLUE, HIGH);
                LOG_PRINTLN("   -> BLANC");
                break;
            case 4: // Éteint
                digitalWrite(LED_RED, LOW);
                digitalWrite(LED_GREEN, LOW);
                digitalWrite(LED_BLUE, LOW);
                LOG_PRINTLN("   -> ETEINT");
                break;
        }
    #endif
    
    // Passage à l'état suivant
    rgbState = (rgbState + 1) % 5;
}

// Appui Bouton 2 - Bip Buzzer (à l'appui, pas au relâchement)
void handleButton2PressStart() {
    LOG_PRINTLN(">> Bouton 2 appuyé ! Bip buzzer...");
    
    // Émettre un bip : pulse le buzzer à 1000 Hz pendant 100ms
    tone(BUZZER, 1000, 100);
}

// Arrêt appui Bouton 2
void handleButton2PressStop() {
    // Arrêter le bip si encore actif
    noTone(BUZZER);
}

// --- FONCTIONS WIFI ---
void setupWifi() {
    LOG_PRINTLN("--- Démarrage WiFiMulti ---");

    // Add WiFi networks (check if SSID is not empty before adding)
    if (strlen(WIFI_SSID1) > 0) {
        wifiMulti.addAP(WIFI_SSID1, WIFI_PASS1);
        LOG_PRINTF("  - Réseau ajouté : %s\n", WIFI_SSID1);
    }
    if (strlen(WIFI_SSID2) > 0) {
        wifiMulti.addAP(WIFI_SSID2, WIFI_PASS2);
        LOG_PRINTF("  - Réseau ajouté : %s\n", WIFI_SSID2);
    }
    if (strlen(WIFI_SSID3) > 0) {
        wifiMulti.addAP(WIFI_SSID3, WIFI_PASS3);
        LOG_PRINTF("  - Réseau ajouté : %s\n", WIFI_SSID3);
    }
    if (strlen(WIFI_SSID4) > 0) {
        wifiMulti.addAP(WIFI_SSID4, WIFI_PASS4);
        LOG_PRINTF("  - Réseau ajouté : %s\n", WIFI_SSID4);
    }

    LOG_PRINT("Connexion WiFi en cours...");
    
    // Affichage initial sur les écrans
    displayWifiProgress(0);
    
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
        
        // Mise à jour de la progression sur les écrans
        displayWifiProgress(progress);
        
        LOG_PRINT(".");
    }
    
    if(wifiMulti.run() == WL_CONNECTED) {
        LOG_PRINTLN(" OK !");
        LOG_PRINT("SSID: "); LOG_PRINTLN(WiFi.SSID());
        LOG_PRINT("IP: "); LOG_PRINTLN(WiFi.localIP());
        
        // Affichage des infos de connexion sur les écrans
        displayWifiConnected(WiFi.SSID().c_str(), WiFi.localIP());
        
        #ifdef HAS_NEOPIXEL
            pixels.setPixelColor(0, pixels.Color(0, 50, 0)); // Vert
            pixels.show();
        #endif
    } else {
        LOG_PRINTLN(" Echec !");
        displayWifiFailed();
    }
}

// --- FONCTIONS OTA ---
void setupOTA() {
    LOG_PRINTLN("--- Configuration ArduinoOTA ---");

    // Port (defaults to 3232)
    ArduinoOTA.setPort(3232);

    // Hostname (defaults to esp32-[MAC])
    ArduinoOTA.setHostname(PROJECT_NAME);

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
    // If you want password protection, uncomment the line below:
    // ArduinoOTA.setPassword("admin");

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else {  // U_SPIFFS
            type = "filesystem";
        }
        LOG_PRINTLN("OTA: Début de la mise à jour " + type);

        #ifdef HAS_ST7789
            display_tft.fillScreen(ST77XX_BLACK);
            display_tft.setTextSize(2);
            display_tft.setTextColor(ST77XX_CYAN);
            display_tft.setCursor(20, 100);
            display_tft.println("OTA Update...");
            display_tft.setCursor(20, 130);
            display_tft.setTextColor(ST77XX_WHITE);
            display_tft.println(type);
        #endif

        #ifdef HAS_NEOPIXEL
            pixels.setPixelColor(0, pixels.Color(0, 0, 255)); // Bleu pour OTA
            pixels.show();
        #endif
    });

    ArduinoOTA.onEnd([]() {
        LOG_PRINTLN("\nOTA: Mise à jour terminée");

        #ifdef HAS_ST7789
            display_tft.fillScreen(ST77XX_BLACK);
            display_tft.setTextSize(2);
            display_tft.setTextColor(ST77XX_GREEN);
            display_tft.setCursor(20, 100);
            display_tft.println("OTA Complete!");
            display_tft.setCursor(20, 130);
            display_tft.setTextColor(ST77XX_WHITE);
            display_tft.println("Rebooting...");
        #endif

        #ifdef HAS_NEOPIXEL
            pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // Vert pour succès
            pixels.show();
        #endif
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        unsigned int percent = (progress / (total / 100));
        LOG_PRINTF("OTA Progress: %u%%\r", percent);

        #ifdef HAS_ST7789
            // Barre de progression
            int barWidth = TFT_WIDTH - 40;
            int barHeight = 20;
            int barX = 20;
            int barY = 170;

            // Efface la zone de la barre
            display_tft.fillRect(barX - 2, barY - 2, barWidth + 4, barHeight + 25, ST77XX_BLACK);

            // Contour de la barre
            display_tft.drawRect(barX, barY, barWidth, barHeight, ST77XX_WHITE);

            // Remplissage de la progression
            int fillWidth = (barWidth - 4) * percent / 100;
            if (fillWidth > 0) {
                display_tft.fillRect(barX + 2, barY + 2, fillWidth, barHeight - 4, ST77XX_CYAN);
            }

            // Pourcentage
            display_tft.setTextSize(1);
            display_tft.setTextColor(ST77XX_WHITE);
            String percentStr = String(percent) + "%";
            int16_t x1, y1;
            uint16_t w, h;
            display_tft.getTextBounds(percentStr.c_str(), 0, 0, &x1, &y1, &w, &h);
            int centerX = (TFT_WIDTH - w) / 2;
            display_tft.fillRect(0, barY + barHeight + 5, TFT_WIDTH, 15, ST77XX_BLACK);
            display_tft.setCursor(centerX, barY + barHeight + 8);
            display_tft.println(percentStr);
        #endif
    });

    ArduinoOTA.onError([](ota_error_t error) {
        LOG_PRINTF("OTA Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) LOG_PRINTLN("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) LOG_PRINTLN("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) LOG_PRINTLN("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) LOG_PRINTLN("Receive Failed");
        else if (error == OTA_END_ERROR) LOG_PRINTLN("End Failed");

        #ifdef HAS_ST7789
            display_tft.fillScreen(ST77XX_BLACK);
            display_tft.setTextSize(2);
            display_tft.setTextColor(ST77XX_RED);
            display_tft.setCursor(20, 100);
            display_tft.println("OTA ERROR!");
        #endif

        #ifdef HAS_NEOPIXEL
            pixels.setPixelColor(0, pixels.Color(255, 0, 0)); // Rouge pour erreur
            pixels.show();
        #endif
    });

    ArduinoOTA.begin();
    LOG_PRINTLN("ArduinoOTA prêt");
    LOG_PRINTF("Hostname: %s\n", PROJECT_NAME);
    LOG_PRINTF("IP: %s\n", WiFi.localIP().toString().c_str());
}

// --- SETUP ---
void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(1000); // Wait for Serial USB

    LOG_PRINTLN("\n=== DEMARRAGE PROJET ===");
    LOG_PRINTF("Board: %s\n", BOARD_NAME);
    LOG_PRINTF("Version: %s\n", PROJECT_VERSION);

    // Init écrans (OLED et/ou ST7789)
    setupDisplays();
    displayStartup(PROJECT_NAME, PROJECT_VERSION);

    // Init LED RGB intégrée
    #ifdef HAS_LED_RGB
        pinMode(LED_RED, OUTPUT);
        pinMode(LED_GREEN, OUTPUT);
        pinMode(LED_BLUE, OUTPUT);
        // Éteindre la LED RGB au démarrage (LOW = éteint)
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_BLUE, LOW);
    #endif

    // Init NeoPixel
    #ifdef HAS_NEOPIXEL
        pixels.begin();
        pixels.setBrightness(30);
        pixels.clear();
        pixels.show();
    #endif

    // Init LED Builtin (si existante)
    #ifdef LED_BUILTIN
        pinMode(LED_BUILTIN, OUTPUT);
    #endif

    // Init Buzzer
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW); // Éteint au démarrage

    // Config Bouton BOOT
    btn.attachClick(handleClick);
    btn.attachLongPressStart(handleLongPress);
    btn.setPressMs(1000); // Durée pour considérer un appui long (ms)

    // Config Bouton 1 - Cycle RGB
    btn1.attachClick(handleButton1Click);
    btn1.setPressMs(1000);

    // Config Bouton 2 - Buzzer (à l'appui immédiat avec délai très court)
    btn2.attachLongPressStart(handleButton2PressStart);
    btn2.attachLongPressStop(handleButton2PressStop);
    btn2.setPressMs(50);  // Très court délai pour détecter l'appui immédiat

    setupWifi();

    // Configuration OTA et Serveur Web si WiFi connecté
    if(WiFi.status() == WL_CONNECTED) {
        setupOTA();         // Configuration ArduinoOTA
        setupWebServer();   // Module délégué dans web_interface.h
        delay(3000);        // Laisser le temps de voir l'IP sur les écrans
    }
}

// --- LOOP (DOIT TOURNER VITE) ---
void loop() {
    // 1. Surveillance Boutons (CRITIQUE : doit être appelé tout le temps)
    btn.tick();   // Bouton BOOT
    btn1.tick();  // Bouton 1 - RGB
    btn2.tick();  // Bouton 2 - Buzzer

    // 2. Gestion ArduinoOTA
    ArduinoOTA.handle();

    // 3. Gestion Serveur Web
    server.handleClient();

    // 4. Gestion WiFi (Reconnexion auto)
    if(wifiMulti.run() != WL_CONNECTED) {
        // Optionnel : Gestion LED rouge si perte de connexion
    }

    // 5. Heartbeat Non-Bloquant (remplace delay)
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        ledState = !ledState;

        // Action visuelle selon la carte
        #ifdef LED_BUILTIN
            digitalWrite(LED_BUILTIN, ledState);
        #endif

        #ifdef HAS_NEOPIXEL
            // Gérer la neopixel selon les états WiFi et reboot
            if (!isRebooting) {  // Si pas en mode reboot
                if(wifiMulti.run() == WL_CONNECTED) {
                    // WiFi connecté : heartbeat vert
                    pixels.setPixelColor(0, ledState ? pixels.Color(0, 100, 0) : pixels.Color(0, 20, 0)); // Vert fort / Vert faible
                } else {
                    // WiFi non connecté : heartbeat rouge (recherche ou hors ligne)
                    pixels.setPixelColor(0, ledState ? pixels.Color(100, 0, 0) : pixels.Color(20, 0, 0)); // Rouge fort / faible
                }
            }
            // Si isRebooting == true, la neopixel reste violet (déjà défini dans handleLongPress)
            pixels.show();
        #endif
    }
}