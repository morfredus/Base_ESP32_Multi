/**
 * @file display.cpp
 * @brief Implementation du module de gestion des écrans
 * @version 0.8.6
 * @date 2026-01-04
 *
 * CORRECTIF v0.8.6:
 * - AUCUNE instanciation statique (cause du bootloop)
 * - Pointeurs null initialises a nullptr
 * - Creation des objets avec new dans setupDisplays()
 * - Evite le "Static Initialization Order Fiasco"
 *
 * REGLES:
 * - Ne jamais creer d'objets Adafruit statiquement
 * - Toujours verifier nullptr avant utilisation
 */

#include "display.h"

// ===================================================================
// SECTION 1 : POINTEURS GLOBAUX (INITIALISES A NULL)
// ===================================================================
// IMPORTANT: Pas d'instanciation statique ici!
// Les objets sont crees dans setupDisplays() avec new

#ifdef HAS_OLED
    Adafruit_SSD1306* pDisplayOled = nullptr;
#endif

#ifdef HAS_ST7789
    Adafruit_ST7789* pDisplayTft = nullptr;
#endif

// ===================================================================
// SECTION 2 : INTERFACE PUBLIQUE
// ===================================================================

void setupDisplays() {
    LOG_PRINTLN("--- Init Ecrans v0.8.6 ---");

    #ifdef HAS_OLED
        if (setupOled()) {
            LOG_PRINTLN("[OK] OLED initialise");
        } else {
            LOG_PRINTLN("[!!] OLED non detecte");
        }
    #endif

    #ifdef HAS_ST7789
        if (setupST7789()) {
            LOG_PRINTLN("[OK] ST7789 initialise");
        } else {
            LOG_PRINTLN("[!!] ST7789 non detecte");
        }
    #endif

    LOG_PRINTLN("--- Ecrans OK ---");
}

void displayStartup(const char* projectName, const char* projectVersion) {
    #ifdef HAS_OLED
        if (pDisplayOled != nullptr) {
            pDisplayOled->clearDisplay();
            pDisplayOled->setTextSize(1);
            pDisplayOled->setTextColor(SSD1306_WHITE);
            pDisplayOled->setCursor(0, 0);
            pDisplayOled->println(projectName);
            pDisplayOled->println("v" + String(projectVersion));
            pDisplayOled->display();
        }
    #endif

    #ifdef HAS_ST7789
        displayST7789Startup(projectName, projectVersion);
    #endif
}

void displayWifiProgress(int progress) {
    #ifdef HAS_OLED
        updateOledStatus("Connexion WiFi...", progress);
    #endif

    #ifdef HAS_ST7789
        displayST7789Progress(progress);
    #endif
}

void displayWifiConnected(const char* ssid, IPAddress ip) {
    #ifdef HAS_OLED
        updateOledConnected(ssid, ip);
    #endif

    #ifdef HAS_ST7789
        displayST7789Connected(ssid, ip);
    #endif
}

void displayWifiFailed() {
    #ifdef HAS_OLED
        updateOledStatus("WiFi: Echec", 100);
    #endif

    #ifdef HAS_ST7789
        displayST7789Failed();
    #endif
}

// ===================================================================
// SECTION 3 : FONCTIONS INTERNES OLED
// ===================================================================

#ifdef HAS_OLED

bool setupOled() {
    LOG_PRINTLN("[..] Creation OLED SSD1306...");

    // Initialisation I2C
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);

    // Creation DIFFEREE de l'objet (evite crash au boot)
    pDisplayOled = new Adafruit_SSD1306(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);

    if (pDisplayOled == nullptr) {
        LOG_PRINTLN("[!!] Echec allocation OLED");
        return false;
    }

    if (!pDisplayOled->begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        LOG_PRINTLN("[!!] OLED non detecte sur I2C");
        delete pDisplayOled;
        pDisplayOled = nullptr;
        return false;
    }

    pDisplayOled->clearDisplay();
    pDisplayOled->display();
    return true;
}

void updateOledStatus(const char* status, int progress) {
    if (pDisplayOled == nullptr) return;

    pDisplayOled->clearDisplay();
    pDisplayOled->setTextSize(1);
    pDisplayOled->setCursor(0, 0);

    // Titre
    pDisplayOled->println(PROJECT_NAME);
    pDisplayOled->println("v" + String(PROJECT_VERSION));
    pDisplayOled->println();

    // Statut
    pDisplayOled->println(status);

    // Barre de progression si demandee
    if (progress >= 0 && progress <= 100) {
        int barWidth = (OLED_WIDTH - 4) * progress / 100;
        pDisplayOled->drawRect(0, 40, OLED_WIDTH, 10, SSD1306_WHITE);
        pDisplayOled->fillRect(2, 42, barWidth, 6, SSD1306_WHITE);
        pDisplayOled->setCursor(0, 52);
        pDisplayOled->print(progress);
        pDisplayOled->println("%");
    }

    pDisplayOled->display();
}

void updateOledConnected(const char* ssid, IPAddress ip) {
    if (pDisplayOled == nullptr) return;

    pDisplayOled->clearDisplay();
    pDisplayOled->setTextSize(1);
    pDisplayOled->setCursor(0, 0);

    // Titre
    pDisplayOled->println(PROJECT_NAME);
    pDisplayOled->println("v" + String(PROJECT_VERSION));
    pDisplayOled->println();

    // Infos WiFi
    pDisplayOled->println("WiFi:");
    pDisplayOled->println(ssid);
    pDisplayOled->println();
    pDisplayOled->print("IP: ");
    pDisplayOled->println(ip);

    pDisplayOled->display();
}

#endif // HAS_OLED

// ===================================================================
// SECTION 4 : FONCTIONS INTERNES ST7789
// ===================================================================

#ifdef HAS_ST7789

bool setupST7789() {
    LOG_PRINTLN("[..] Creation TFT ST7789...");

    // Initialisation du SPI materiel AVANT creation de l'objet
    SPI.begin(PIN_TFT_SCLK, -1, PIN_TFT_MOSI, PIN_TFT_CS);

    // Configuration du backlight
    pinMode(PIN_TFT_BL, OUTPUT);
    digitalWrite(PIN_TFT_BL, HIGH);

    // Creation DIFFEREE de l'objet (evite crash au boot)
    pDisplayTft = new Adafruit_ST7789(&SPI, PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);

    if (pDisplayTft == nullptr) {
        LOG_PRINTLN("[!!] Echec allocation ST7789");
        return false;
    }

    // Initialisation du TFT
    pDisplayTft->init(ST7789_WIDTH, ST7789_HEIGHT);
    pDisplayTft->setRotation(ST7789_ROTATION);
    pDisplayTft->fillScreen(ST77XX_BLACK);

    return true;
}

void displayST7789Startup(const char* projectName, const char* projectVersion) {
    if (pDisplayTft == nullptr) return;

    pDisplayTft->fillScreen(ST77XX_BLACK);
    pDisplayTft->setTextColor(ST77XX_CYAN);
    pDisplayTft->setTextSize(3);

    // Calcul du centrage horizontal pour le nom du projet
    int16_t x1, y1;
    uint16_t w, h;
    pDisplayTft->getTextBounds(projectName, 0, 0, &x1, &y1, &w, &h);
    int centerX = (ST7789_WIDTH - w) / 2;

    pDisplayTft->setCursor(centerX, 80);
    pDisplayTft->println(projectName);

    // Version
    pDisplayTft->setTextSize(2);
    pDisplayTft->setTextColor(ST77XX_WHITE);
    String versionStr = "v" + String(projectVersion);
    pDisplayTft->getTextBounds(versionStr.c_str(), 0, 0, &x1, &y1, &w, &h);
    centerX = (ST7789_WIDTH - w) / 2;
    pDisplayTft->setCursor(centerX, 120);
    pDisplayTft->println(versionStr);
}

void displayST7789Progress(int progress) {
    if (pDisplayTft == nullptr) return;

    // Zone pour le texte de statut
    pDisplayTft->fillRect(0, 40, ST7789_WIDTH, 30, ST77XX_BLACK);
    pDisplayTft->setTextSize(2);
    pDisplayTft->setTextColor(ST77XX_YELLOW);

    int16_t x1, y1;
    uint16_t w, h;
    pDisplayTft->getTextBounds("Connexion WiFi...", 0, 0, &x1, &y1, &w, &h);
    int centerX = (ST7789_WIDTH - w) / 2;
    pDisplayTft->setCursor(centerX, 50);
    pDisplayTft->println("Connexion WiFi...");

    // Barre de progression
    int barWidth = ST7789_WIDTH - 40;
    int barHeight = 30;
    int barX = 20;
    int barY = 160;

    // Efface la zone de la barre
    pDisplayTft->fillRect(barX - 2, barY - 2, barWidth + 4, barHeight + 20, ST77XX_BLACK);

    // Contour de la barre
    pDisplayTft->drawRect(barX, barY, barWidth, barHeight, ST77XX_WHITE);

    // Remplissage de la progression
    int fillWidth = (barWidth - 4) * progress / 100;
    if (fillWidth > 0) {
        pDisplayTft->fillRect(barX + 2, barY + 2, fillWidth, barHeight - 4, ST77XX_GREEN);
    }

    // Pourcentage
    pDisplayTft->setTextSize(2);
    pDisplayTft->setTextColor(ST77XX_WHITE);
    String percentStr = String(progress) + "%";
    pDisplayTft->getTextBounds(percentStr.c_str(), 0, 0, &x1, &y1, &w, &h);
    centerX = (ST7789_WIDTH - w) / 2;
    pDisplayTft->setCursor(centerX, barY + barHeight + 5);
    pDisplayTft->println(percentStr);
}

void displayST7789Connected(const char* ssid, IPAddress ip) {
    if (pDisplayTft == nullptr) return;

    pDisplayTft->fillScreen(ST77XX_BLACK);

    // Titre
    pDisplayTft->setTextSize(2);
    pDisplayTft->setTextColor(ST77XX_CYAN);
    int16_t x1, y1;
    uint16_t w, h;
    pDisplayTft->getTextBounds(PROJECT_NAME, 0, 0, &x1, &y1, &w, &h);
    int centerX = (ST7789_WIDTH - w) / 2;
    pDisplayTft->setCursor(centerX, 20);
    pDisplayTft->println(PROJECT_NAME);

    // Version
    pDisplayTft->setTextSize(1);
    pDisplayTft->setTextColor(ST77XX_WHITE);
    String versionStr = "v" + String(PROJECT_VERSION);
    pDisplayTft->getTextBounds(versionStr.c_str(), 0, 0, &x1, &y1, &w, &h);
    centerX = (ST7789_WIDTH - w) / 2;
    pDisplayTft->setCursor(centerX, 45);
    pDisplayTft->println(versionStr);

    // Ligne de separation
    pDisplayTft->drawLine(20, 60, ST7789_WIDTH - 20, 60, ST77XX_CYAN);

    // WiFi connecte
    pDisplayTft->setTextSize(2);
    pDisplayTft->setTextColor(ST77XX_GREEN);
    pDisplayTft->setCursor(20, 80);
    pDisplayTft->println("WiFi Connecte");

    // SSID
    pDisplayTft->setTextSize(1);
    pDisplayTft->setTextColor(ST77XX_YELLOW);
    pDisplayTft->setCursor(20, 110);
    pDisplayTft->println("Reseau:");
    pDisplayTft->setTextColor(ST77XX_WHITE);
    pDisplayTft->setCursor(20, 125);
    pDisplayTft->println(ssid);

    // Adresse IP
    pDisplayTft->setTextColor(ST77XX_YELLOW);
    pDisplayTft->setCursor(20, 150);
    pDisplayTft->println("Adresse IP:");
    pDisplayTft->setTextColor(ST77XX_WHITE);
    pDisplayTft->setTextSize(2);
    pDisplayTft->setCursor(20, 170);
    pDisplayTft->println(ip);
}

void displayST7789Failed() {
    if (pDisplayTft == nullptr) return;

    pDisplayTft->fillScreen(ST77XX_BLACK);

    pDisplayTft->setTextSize(3);
    pDisplayTft->setTextColor(ST77XX_RED);

    int16_t x1, y1;
    uint16_t w, h;
    pDisplayTft->getTextBounds("ERREUR", 0, 0, &x1, &y1, &w, &h);
    int centerX = (ST7789_WIDTH - w) / 2;

    pDisplayTft->setCursor(centerX, 80);
    pDisplayTft->println("ERREUR");

    pDisplayTft->setTextSize(2);
    pDisplayTft->setTextColor(ST77XX_WHITE);
    pDisplayTft->getTextBounds("WiFi Echec", 0, 0, &x1, &y1, &w, &h);
    centerX = (ST7789_WIDTH - w) / 2;
    pDisplayTft->setCursor(centerX, 120);
    pDisplayTft->println("WiFi Echec");
}

#endif // HAS_ST7789
