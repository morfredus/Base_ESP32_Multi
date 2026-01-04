#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>
#include <OneButton.h>

#include "config.h"
#include "secrets.h"
#include "web_interface.h"
#include "display.h"
#include "neopixel.h"

// --- OBJETS ---
WiFiMulti wifiMulti;
WebServer server(80);
OneButton btn(PIN_BUTTON_BOOT, true); // true = Active Low (Bouton poussoir standard vers GND)
OneButton btn1(PIN_BUTTON_1, true);   // Bouton 1 - Cycle RGB
OneButton btn2(PIN_BUTTON_2, true);   // Bouton 2 - Buzzer

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
    
    #if defined(HAS_NEOPIXEL) && defined(TARGET_ESP32_S3)
        setInternalPixelColor(COLOR_PURPLE); // Violet pour confirmer le reboot
    #endif
    
    #if defined(HAS_OLED) || defined(HAS_ST7789)
        #ifdef HAS_OLED
            if (pDisplayOled != nullptr) {
                pDisplayOled->clearDisplay();
                pDisplayOled->setTextSize(2);
                pDisplayOled->setCursor(0, 20);
                pDisplayOled->println("REBOOT...");
                pDisplayOled->display();
            }
        #endif

        #ifdef HAS_ST7789
            if (pDisplayTft != nullptr) {
                // Afficher barre de progression pendant 2 secondes
                pDisplayTft->fillScreen(ST77XX_BLACK);
                pDisplayTft->setTextSize(3);
                pDisplayTft->setTextColor(ST77XX_RED);

                int16_t x1, y1;
                uint16_t w, h;
                pDisplayTft->getTextBounds("REBOOT", 0, 0, &x1, &y1, &w, &h);
                int centerX = (ST7789_WIDTH - w) / 2;
                pDisplayTft->setCursor(centerX, 50);
                pDisplayTft->println("REBOOT");

                // Barre de progression
                int barWidth = ST7789_WIDTH - 40;
                int barHeight = 30;
                int barX = 20;
                int barY = 140;

                unsigned long startTime = millis();
                const unsigned long progressDuration = 2000; // 2 secondes

                while (millis() - startTime < progressDuration) {
                    // Verifier si le bouton BOOT est toujours appuye (Active Low = LOW = appuye)
                    bool buttonStillPressed = (digitalRead(PIN_BUTTON_BOOT) == LOW);

                    if (!buttonStillPressed) {
                        // Bouton relache avant la fin
                        LOG_PRINTLN(">> Bouton relache ! Reboot annule.");
                        isRebooting = false;

                        // Restaurer l'affichage precedent (WiFi connecte ou non)
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
                    pDisplayTft->fillRect(barX - 2, barY - 2, barWidth + 4, barHeight + 20, ST77XX_BLACK);

                    // Contour de la barre
                    pDisplayTft->drawRect(barX, barY, barWidth, barHeight, ST77XX_WHITE);

                    // Remplissage de la progression
                    int fillWidth = (barWidth - 4) * progress / 100;
                    if (fillWidth > 0) {
                        pDisplayTft->fillRect(barX + 2, barY + 2, fillWidth, barHeight - 4, ST77XX_MAGENTA);
                    }

                    // Pourcentage
                    pDisplayTft->setTextSize(2);
                    pDisplayTft->setTextColor(ST77XX_WHITE);
                    String percentStr = String(progress) + "%";
                    pDisplayTft->getTextBounds(percentStr.c_str(), 0, 0, &x1, &y1, &w, &h);
                    centerX = (ST7789_WIDTH - w) / 2;
                    pDisplayTft->setCursor(centerX, barY + barHeight + 5);
                    pDisplayTft->println(percentStr);

                    delay(50); // Mise a jour chaque 50ms
                }

                // Si on est sortie de la boucle normalement, la barre est a 100%
                if (isRebooting && digitalRead(PIN_BUTTON_BOOT) == LOW) {
                    LOG_PRINTLN(">> Barre completee a 100% ! Redemarrage...");
                }
            }
        #endif
    #endif
    
    // Seulement redémarrer si la barre a atteint 100%
    if (isRebooting && digitalRead(PIN_BUTTON_BOOT) == LOW) {
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
                digitalWrite(PIN_LED_RED, HIGH);
                digitalWrite(PIN_LED_GREEN, LOW);
                digitalWrite(PIN_LED_BLUE, LOW);
                LOG_PRINTLN("   -> ROUGE");
                break;
            case 1: // Vert
                digitalWrite(PIN_LED_RED, LOW);
                digitalWrite(PIN_LED_GREEN, HIGH);
                digitalWrite(PIN_LED_BLUE, LOW);
                LOG_PRINTLN("   -> VERT");
                break;
            case 2: // Bleu
                digitalWrite(PIN_LED_RED, LOW);
                digitalWrite(PIN_LED_GREEN, LOW);
                digitalWrite(PIN_LED_BLUE, HIGH);
                LOG_PRINTLN("   -> BLEU");
                break;
            case 3: // Blanc
                digitalWrite(PIN_LED_RED, HIGH);
                digitalWrite(PIN_LED_GREEN, HIGH);
                digitalWrite(PIN_LED_BLUE, HIGH);
                LOG_PRINTLN("   -> BLANC");
                break;
            case 4: // Éteint
                digitalWrite(PIN_LED_RED, LOW);
                digitalWrite(PIN_LED_GREEN, LOW);
                digitalWrite(PIN_LED_BLUE, LOW);
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
    tone(PIN_BUZZER, 1000, 100);
}

// Arrêt appui Bouton 2
void handleButton2PressStop() {
    // Arrêter le bip si encore actif
    noTone(PIN_BUZZER);
}

// --- FONCTIONS WIFI ---
void setupWifi() {
    LOG_PRINTLN("--- Démarrage WiFiMulti ---");
    int numNetworks = sizeof(WIFI_NETWORKS) / sizeof(WIFI_NETWORKS[0]);
    
    for (int i = 0; i < numNetworks; i++) {
        wifiMulti.addAP(WIFI_NETWORKS[i][0], WIFI_NETWORKS[i][1]);
    }

    LOG_PRINT("Connexion WiFi en cours...");
    
    // Affichage initial sur les écrans
    displayWifiProgress(0);
    
    #if defined(HAS_NEOPIXEL) && defined(TARGET_ESP32_S3)
        setInternalPixelColor(COLOR_YELLOW); // Jaune - connexion en cours
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

        #if defined(HAS_NEOPIXEL) && defined(TARGET_ESP32_S3)
            setInternalPixelColor(COLOR_GREEN); // Vert - connecte
        #endif
    } else {
        LOG_PRINTLN(" Echec !");
        displayWifiFailed();
    }
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
        pinMode(PIN_LED_RED, OUTPUT);
        pinMode(PIN_LED_GREEN, OUTPUT);
        pinMode(PIN_LED_BLUE, OUTPUT);
        // Éteindre la LED RGB au démarrage (LOW = éteint)
        digitalWrite(PIN_LED_RED, LOW);
        digitalWrite(PIN_LED_GREEN, LOW);
        digitalWrite(PIN_LED_BLUE, LOW);
    #endif

    // Init NeoPixels (LED interne + Matrice 8x8)
    setupNeoPixels();

    // Init LED Builtin (si existante)
    #ifdef PIN_LED_BUILTIN
        pinMode(PIN_LED_BUILTIN, OUTPUT);
    #endif

    // Init Buzzer
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW); // Éteint au démarrage

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
    
    // Démarrage Serveur Web
    if(WiFi.status() == WL_CONNECTED) {
        setupWebServer();  // Module délégué dans web_interface.h
        delay(3000); // Laisser le temps de voir l'IP sur les écrans
    }
}

// --- LOOP (DOIT TOURNER VITE) ---
void loop() {
    // 1. Surveillance Boutons (CRITIQUE : doit être appelé tout le temps)
    btn.tick();   // Bouton BOOT
    btn1.tick();  // Bouton 1 - RGB
    btn2.tick();  // Bouton 2 - Buzzer

    // 2. Gestion Serveur Web
    server.handleClient();

    // 3. Gestion WiFi (Reconnexion auto)
    if(wifiMulti.run() != WL_CONNECTED) {
        // Optionnel : Gestion LED rouge si perte de connexion
    }

    // 4. Heartbeat Non-Bloquant (remplace delay)
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        ledState = !ledState;

        // Action visuelle selon la carte
        #ifdef PIN_LED_BUILTIN
            digitalWrite(PIN_LED_BUILTIN, ledState);
        #endif

        #if defined(HAS_NEOPIXEL) && defined(TARGET_ESP32_S3)
            // Gerer la neopixel interne selon les etats WiFi et reboot
            if (!isRebooting) {  // Si pas en mode reboot
                if(wifiMulti.run() == WL_CONNECTED) {
                    // WiFi connecte : heartbeat vert
                    internalPixelHeartbeat(COLOR_GREEN, ledState);
                } else {
                    // WiFi non connecte : heartbeat rouge (recherche ou hors ligne)
                    internalPixelHeartbeat(COLOR_RED, ledState);
                }
            }
            // Si isRebooting == true, la neopixel reste violet (deja defini dans handleLongPress)
        #endif
    }
}