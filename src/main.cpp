/**
 * @file main.cpp
 * @brief Point d'entree principal du projet ESP32 Multi-environnement
 * @version 0.8.9
 * @date 2026-01-04
 *
 * CORRECTIF v0.8.9:
 * - AUCUNE instanciation statique d'objets complexes
 * - WiFiMulti, WebServer, OneButton tous en pointeurs
 * - Creation differee de TOUS les objets dans setup()
 * - Evite le "Static Initialization Order Fiasco" sur ESP32-S3
 *
 * CORRECTIF v0.8.8:
 * - Pointeurs null pour OneButton
 *
 * RAPPEL CRITIQUE:
 * Sur ESP32-S3 avec USB CDC, les constructeurs statiques peuvent
 * s'executer AVANT que le hardware soit pret, causant un bootloop.
 * TOUJOURS utiliser des pointeurs + new dans setup().
 */

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

// ===================================================================
// SECTION 1 : POINTEURS GLOBAUX (PAS D'OBJETS STATIQUES!)
// ===================================================================
// CORRECTIF v0.8.9: TOUS les objets sont maintenant des pointeurs
// pour eviter le "Static Initialization Order Fiasco" sur ESP32-S3

// --- WiFi et Serveur Web ---
WiFiMulti* pWifiMulti = nullptr;
WebServer* pServer = nullptr;

// --- Boutons (v0.8.8) ---
OneButton* pBtn = nullptr;    // Bouton BOOT
OneButton* pBtn1 = nullptr;   // Bouton 1 - Cycle RGB
OneButton* pBtn2 = nullptr;   // Bouton 2 - Buzzer

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

// ===================================================================
// SECTION 3 : FONCTIONS WIFI
// ===================================================================

void setupWifi() {
    if (pWifiMulti == nullptr) {
        LOG_PRINTLN("[!!] WiFiMulti non initialise!");
        return;
    }

    LOG_PRINTLN("--- Demarrage WiFiMulti v0.8.9 ---");
    int numNetworks = sizeof(WIFI_NETWORKS) / sizeof(WIFI_NETWORKS[0]);

    for (int i = 0; i < numNetworks; i++) {
        pWifiMulti->addAP(WIFI_NETWORKS[i][0], WIFI_NETWORKS[i][1]);
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

    while(pWifiMulti->run() != WL_CONNECTED && attempts < maxAttempts) {
        delay(500);
        attempts++;
        int progress = (attempts * 100) / maxAttempts;

        // Mise a jour de la progression sur les ecrans
        displayWifiProgress(progress);

        LOG_PRINT(".");
    }

    if(pWifiMulti->run() == WL_CONNECTED) {
        LOG_PRINTLN(" OK !");
        LOG_PRINT("SSID: "); LOG_PRINTLN(WiFi.SSID());
        LOG_PRINT("IP: "); LOG_PRINTLN(WiFi.localIP());

        // Affichage des infos de connexion sur les ecrans
        displayWifiConnected(WiFi.SSID().c_str(), WiFi.localIP());

        #if defined(HAS_NEOPIXEL) && defined(TARGET_ESP32_S3)
            setInternalPixelColor(COLOR_GREEN); // Vert - connecte
        #endif
    } else {
        LOG_PRINTLN(" Echec !");
        displayWifiFailed();
    }
}

// ===================================================================
// SECTION 4 : SETUP
// ===================================================================

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(1000); // Wait for Serial USB (ESP32-S3)

    LOG_PRINTLN("\n=== DEMARRAGE PROJET v0.8.9 ===");
    LOG_PRINTF("Board: %s\n", BOARD_NAME);
    LOG_PRINTF("Version: %s\n", PROJECT_VERSION);

    // ---------------------------------------------------------------
    // 4.1 : CREATION DIFFEREE WiFi et WebServer (v0.8.9)
    // ---------------------------------------------------------------
    LOG_PRINTLN("--- Init WiFi/WebServer v0.8.9 ---");

    pWifiMulti = new WiFiMulti();
    if (pWifiMulti != nullptr) {
        LOG_PRINTLN("[OK] WiFiMulti cree");
    } else {
        LOG_PRINTLN("[!!] Echec creation WiFiMulti");
    }

    pServer = new WebServer(80);
    if (pServer != nullptr) {
        LOG_PRINTLN("[OK] WebServer cree");
    } else {
        LOG_PRINTLN("[!!] Echec creation WebServer");
    }

    // ---------------------------------------------------------------
    // 4.2 : Init Ecrans (OLED et/ou ST7789)
    // ---------------------------------------------------------------
    setupDisplays();
    displayStartup(PROJECT_NAME, PROJECT_VERSION);

    // ---------------------------------------------------------------
    // 4.3 : Init LED RGB integree
    // ---------------------------------------------------------------
    #ifdef HAS_LED_RGB
        pinMode(PIN_LED_RED, OUTPUT);
        pinMode(PIN_LED_GREEN, OUTPUT);
        pinMode(PIN_LED_BLUE, OUTPUT);
        digitalWrite(PIN_LED_RED, LOW);
        digitalWrite(PIN_LED_GREEN, LOW);
        digitalWrite(PIN_LED_BLUE, LOW);
    #endif

    // ---------------------------------------------------------------
    // 4.4 : Init NeoPixels (LED interne + Matrice 8x8)
    // ---------------------------------------------------------------
    setupNeoPixels();

    // ---------------------------------------------------------------
    // 4.5 : Init LED Builtin (ESP32 Classic uniquement)
    // ---------------------------------------------------------------
    #ifdef PIN_LED_BUILTIN
        pinMode(PIN_LED_BUILTIN, OUTPUT);
    #endif

    // ---------------------------------------------------------------
    // 4.6 : Init Buzzer
    // ---------------------------------------------------------------
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);

    // ---------------------------------------------------------------
    // 4.7 : CREATION DIFFEREE DES BOUTONS (v0.8.8)
    // ---------------------------------------------------------------
    LOG_PRINTLN("--- Init Boutons v0.8.9 ---");

    // Bouton BOOT
    pBtn = new OneButton(PIN_BUTTON_BOOT, true);
    if (pBtn != nullptr) {
        pBtn->attachClick(handleClick);
        pBtn->attachLongPressStart(handleLongPress);
        pBtn->setPressMs(1000);
        LOG_PRINTLN("[OK] Bouton BOOT initialise");
    }

    // Bouton 1 - Cycle RGB
    pBtn1 = new OneButton(PIN_BUTTON_1, true);
    if (pBtn1 != nullptr) {
        pBtn1->attachClick(handleButton1Click);
        pBtn1->setPressMs(1000);
        LOG_PRINTLN("[OK] Bouton 1 initialise");
    }

    // Bouton 2 - Buzzer (appui immediat avec delai court)
    pBtn2 = new OneButton(PIN_BUTTON_2, true);
    if (pBtn2 != nullptr) {
        pBtn2->attachLongPressStart(handleButton2PressStart);
        pBtn2->attachLongPressStop(handleButton2PressStop);
        pBtn2->setPressMs(50);
        LOG_PRINTLN("[OK] Bouton 2 initialise");
    }

    setupWifi();
    
    // Démarrage Serveur Web
    if(WiFi.status() == WL_CONNECTED) {
        setupWebServer();  // Module délégué dans web_interface.h
        delay(3000); // Laisser le temps de voir l'IP sur les écrans
    }
}

// ===================================================================
// SECTION 5 : LOOP (DOIT TOURNER VITE)
// ===================================================================

void loop() {
    // 5.1 : Surveillance Boutons (CRITIQUE : doit etre appele tout le temps)
    if (pBtn != nullptr) pBtn->tick();
    if (pBtn1 != nullptr) pBtn1->tick();
    if (pBtn2 != nullptr) pBtn2->tick();

    // 5.2 : Gestion Serveur Web
    if (pServer != nullptr) {
        pServer->handleClient();
    }

    // 5.3 : Gestion WiFi (Reconnexion auto)
    if (pWifiMulti != nullptr && pWifiMulti->run() != WL_CONNECTED) {
        // Optionnel : Gestion LED rouge si perte de connexion
    }

    // 5.4 : Heartbeat Non-Bloquant (remplace delay)
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
            if (!isRebooting) {
                bool wifiConnected = (pWifiMulti != nullptr && pWifiMulti->run() == WL_CONNECTED);
                if (wifiConnected) {
                    internalPixelHeartbeat(COLOR_GREEN, ledState);
                } else {
                    internalPixelHeartbeat(COLOR_RED, ledState);
                }
            }
        #endif
    }
}