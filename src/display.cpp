/**
 * @file display.cpp
 * @brief Implémentation du module de gestion des écrans
 * @version 0.9.0
 * @date 2026-01-03
 */

#include "display.h"

// --- INSTANCIATION DES OBJETS ---
#ifdef HAS_OLED
    Adafruit_SSD1306 display_oled(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);
#endif

#ifdef HAS_ST7789
    // Adafruit_ST7789 display_tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
    Adafruit_ST7789 display_tft = Adafruit_ST7789(&SPI, TFT_CS, TFT_DC, TFT_RST);
#endif

// =============================================================================
// FONCTIONS PUBLIQUES - INTERFACE UNIFIÉE
// =============================================================================

void setupDisplays() {
    LOG_PRINTLN("--- Initialisation des écrans ---");
    
    #ifdef HAS_OLED
        if (setupOled()) {
            LOG_PRINTLN("✓ OLED initialisé");
        } else {
            LOG_PRINTLN("✗ OLED non détecté");
        }
    #endif
    
    #ifdef HAS_ST7789
        if (setupST7789()) {
            LOG_PRINTLN("✓ ST7789 initialisé");
        } else {
            LOG_PRINTLN("✗ ST7789 non détecté");
        }
    #endif
}

void displayStartup(const char* projectName, const char* projectVersion) {
    #ifdef HAS_OLED
        display_oled.clearDisplay();
        display_oled.setTextSize(1);
        display_oled.setTextColor(SSD1306_WHITE);
        display_oled.setCursor(0, 0);
        display_oled.println(projectName);
        display_oled.println("v" + String(projectVersion));
        display_oled.display();
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

// =============================================================================
// FONCTIONS INTERNES - OLED
// =============================================================================

#ifdef HAS_OLED
bool setupOled() {
    Wire.begin(I2C_SDA, I2C_SCL);
    
    if (!display_oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        return false;
    }
    
    display_oled.clearDisplay();
    display_oled.display();
    return true;
}

void updateOledStatus(const char* status, int progress) {
    display_oled.clearDisplay();
    display_oled.setTextSize(1);
    display_oled.setCursor(0, 0);
    
    // Titre
    display_oled.println(PROJECT_NAME);
    display_oled.println("v" + String(PROJECT_VERSION));
    display_oled.println();
    
    // Statut
    display_oled.println(status);
    
    // Barre de progression si demandée
    if (progress >= 0 && progress <= 100) {
        int barWidth = (OLED_WIDTH - 4) * progress / 100;
        display_oled.drawRect(0, 40, OLED_WIDTH, 10, SSD1306_WHITE);
        display_oled.fillRect(2, 42, barWidth, 6, SSD1306_WHITE);
        display_oled.setCursor(0, 52);
        display_oled.print(progress);
        display_oled.println("%");
    }
    
    display_oled.display();
}

void updateOledConnected(const char* ssid, IPAddress ip) {
    display_oled.clearDisplay();
    display_oled.setTextSize(1);
    display_oled.setCursor(0, 0);
    
    // Titre
    display_oled.println(PROJECT_NAME);
    display_oled.println("v" + String(PROJECT_VERSION));
    display_oled.println();
    
    // Infos WiFi
    display_oled.println("WiFi:");
    display_oled.println(ssid);
    display_oled.println();
    display_oled.print("IP: ");
    display_oled.println(ip);
    
    display_oled.display();
}
#endif

// =============================================================================
// FONCTIONS INTERNES - ST7789
// =============================================================================

#ifdef HAS_ST7789
bool setupST7789() {
    // Initialisation du SPI matériel
    SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
    
    // Configuration du backlight
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH); // Allume le rétroéclairage
    
    // Initialisation du TFT
    display_tft.init(TFT_WIDTH, TFT_HEIGHT);
    display_tft.setRotation(TFT_ROTATION);
    display_tft.fillScreen(ST77XX_BLACK);
    
    return true; // ST7789 ne retourne pas d'erreur, on suppose qu'il est présent
}

void displayST7789Startup(const char* projectName, const char* projectVersion) {
    display_tft.fillScreen(ST77XX_BLACK);
    display_tft.setTextColor(ST77XX_CYAN);
    display_tft.setTextSize(3);
    
    // Calcul du centrage horizontal pour le nom du projet
    int16_t x1, y1;
    uint16_t w, h;
    display_tft.getTextBounds(projectName, 0, 0, &x1, &y1, &w, &h);
    int centerX = (TFT_WIDTH - w) / 2;

    display_tft.setCursor(centerX, 80);
    display_tft.println(projectName);

    // Version
    display_tft.setTextSize(2);
    display_tft.setTextColor(ST77XX_WHITE);
    String versionStr = "v" + String(projectVersion);
    display_tft.getTextBounds(versionStr.c_str(), 0, 0, &x1, &y1, &w, &h);
    centerX = (TFT_WIDTH - w) / 2;
    display_tft.setCursor(centerX, 120);
    display_tft.println(versionStr);
}

void displayST7789Progress(int progress) {
    // Zone pour le texte de statut
    display_tft.fillRect(0, 40, TFT_WIDTH, 30, ST77XX_BLACK);
    display_tft.setTextSize(2);
    display_tft.setTextColor(ST77XX_YELLOW);

    int16_t x1, y1;
    uint16_t w, h;
    display_tft.getTextBounds("Connexion WiFi...", 0, 0, &x1, &y1, &w, &h);
    int centerX = (TFT_WIDTH - w) / 2;
    display_tft.setCursor(centerX, 50);
    display_tft.println("Connexion WiFi...");

    // Barre de progression
    int barWidth = TFT_WIDTH - 40;
    int barHeight = 30;
    int barX = 20;
    int barY = 160;

    // Efface la zone de la barre
    display_tft.fillRect(barX - 2, barY - 2, barWidth + 4, barHeight + 20, ST77XX_BLACK);

    // Contour de la barre
    display_tft.drawRect(barX, barY, barWidth, barHeight, ST77XX_WHITE);

    // Remplissage de la progression
    int fillWidth = (barWidth - 4) * progress / 100;
    if (fillWidth > 0) {
        display_tft.fillRect(barX + 2, barY + 2, fillWidth, barHeight - 4, ST77XX_GREEN);
    }

    // Pourcentage
    display_tft.setTextSize(2);
    display_tft.setTextColor(ST77XX_WHITE);
    String percentStr = String(progress) + "%";
    display_tft.getTextBounds(percentStr.c_str(), 0, 0, &x1, &y1, &w, &h);
    centerX = (TFT_WIDTH - w) / 2;
    display_tft.setCursor(centerX, barY + barHeight + 5);
    display_tft.println(percentStr);
}

void displayST7789Connected(const char* ssid, IPAddress ip) {
    display_tft.fillScreen(ST77XX_BLACK);
    
    // Titre
    display_tft.setTextSize(2);
    display_tft.setTextColor(ST77XX_CYAN);
    int16_t x1, y1;
    uint16_t w, h;
    display_tft.getTextBounds(PROJECT_NAME, 0, 0, &x1, &y1, &w, &h);
    int centerX = (TFT_WIDTH - w) / 2;
    display_tft.setCursor(centerX, 20);
    display_tft.println(PROJECT_NAME);

    // Version
    display_tft.setTextSize(1);
    display_tft.setTextColor(ST77XX_WHITE);
    String versionStr = "v" + String(PROJECT_VERSION);
    display_tft.getTextBounds(versionStr.c_str(), 0, 0, &x1, &y1, &w, &h);
    centerX = (TFT_WIDTH - w) / 2;
    display_tft.setCursor(centerX, 45);
    display_tft.println(versionStr);

    // Ligne de séparation
    display_tft.drawLine(20, 60, TFT_WIDTH - 20, 60, ST77XX_CYAN);
    
    // WiFi connecté
    display_tft.setTextSize(2);
    display_tft.setTextColor(ST77XX_GREEN);
    display_tft.setCursor(20, 80);
    display_tft.println("WiFi Connecte");
    
    // SSID
    display_tft.setTextSize(1);
    display_tft.setTextColor(ST77XX_YELLOW);
    display_tft.setCursor(20, 110);
    display_tft.println("Reseau:");
    display_tft.setTextColor(ST77XX_WHITE);
    display_tft.setCursor(20, 125);
    display_tft.println(ssid);
    
    // Adresse IP
    display_tft.setTextColor(ST77XX_YELLOW);
    display_tft.setCursor(20, 150);
    display_tft.println("Adresse IP:");
    display_tft.setTextColor(ST77XX_WHITE);
    display_tft.setTextSize(2);
    display_tft.setCursor(20, 170);
    display_tft.println(ip);
}

void displayST7789Failed() {
    display_tft.fillScreen(ST77XX_BLACK);
    
    display_tft.setTextSize(3);
    display_tft.setTextColor(ST77XX_RED);
    
    int16_t x1, y1;
    uint16_t w, h;
    display_tft.getTextBounds("ERREUR", 0, 0, &x1, &y1, &w, &h);
    int centerX = (TFT_WIDTH - w) / 2;

    display_tft.setCursor(centerX, 80);
    display_tft.println("ERREUR");

    display_tft.setTextSize(2);
    display_tft.setTextColor(ST77XX_WHITE);
    display_tft.getTextBounds("WiFi Echec", 0, 0, &x1, &y1, &w, &h);
    centerX = (TFT_WIDTH - w) / 2;
    display_tft.setCursor(centerX, 120);
    display_tft.println("WiFi Echec");
}
#endif
