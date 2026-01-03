#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

// =========================================================
//         Configuration Pinout ESP32-S3 DevKitC-1 N16R8
// =========================================================
#if defined(TARGET_ESP32_S3)

    #define BOARD_NAME "ESP32-S3 DevKitC-1"

// ============================================================
// RAPPELS DE SÉCURITÉ ESP32-S3
// ============================================================
// - Tension logique GPIO : 3.3 V uniquement (aucune broche n'est 5V tolérante)[cite: 1].
// - Ne jamais appliquer 5V directement sur une GPIO (utiliser diviseur ou interface)[cite: 2].
// - GPIO0 : broche BOOT (strapping) – ne rien connecter qui la force à LOW au boot[cite: 3].
// - GPIO46 : utilisé pour PIR, attention au JTAG / boot[cite: 4].
// - GPS TX et HC-SR04 ECHO : Diviseur de tension OBLIGATOIRE si alimentés en 5V[cite: 5].
// - LEDs : ajouter résistance série 220–470 O (0.25 W)[cite: 6].
// - I2C (GPIO 15/16) : pull-up 4.7 kO vers 3.3 V OBLIGATOIRE[cite: 7].
// - Buzzer : transistor nécessaire (résistance base 1–10 kO)[cite: 8].
// ============================================================

// ------------------------------------
// SYSTÈME & BOOT
// ------------------------------------
#define BUTTON_BOOT    0   // Bouton BOOT – entrée 3.3V, vers GND à l’appui [cite: 3]
#define NEOPIXEL      48   // Data NeoPixel – sortie 3.3V (Soudé sur S3)

// ------------------------------------
// COMMUNICATION (UART & I2C)
// ------------------------------------
// GPS (UART1)
#define GPS_RXD       18   // Entrée RX ESP32 (depuis TX GPS) – 3.3V, diviseur obligatoire si GPS 5V
#define GPS_TXD       17   // Sortie TX ESP32 (vers RX GPS) – 3.3V
#define GPS_PPS        8   // Entrée PPS GPS (1PPS) – 3.3V

// I2C
#define I2C_SDA       15   // SDA – open-drain 3.3V, pull-up externe 4.7 kO [cite: 7, 15]
#define I2C_SCL       16   // SCL – open-drain 3.3V, pull-up externe 4.7 kO [cite: 7, 16]

// ------------------------------------
// AFFICHAGE & STOCKAGE (SPI PARTAGÉ)
// ------------------------------------
// Alias SPI : MOSI=DIN/SDA/SDI/DATA/DI, MISO=DO/SDO, SCLK=SCK/CLK/SCL, CS=CS/CE/SS/TCS, DC=D/C/A0/RS, RST=RST/RESET/RES, BL=BL/LED/BACKLIGHT 

// Bus SPI Physique
#define TFT_MISO      13   // MISO (DO / SDO) – entrée 3.3V 
#define TFT_MOSI      11   // MOSI (DIN / SDA / SDI / DATA / DI) – sortie 3.3V 
#define TFT_SCLK      12   // Horloge SPI (SCK / CLK / SCL) – sortie 3.3V [cite: 13]

// Contrôle TFT ST7789 / ILI9341
#define TFT_CS        10   // Chip Select (CS / CE / SS / TCS) – sortie 3.3V 
#define TFT_DC         9   // Data/Command (D/C / A0 / RS) – sortie 3.3V 
#define TFT_RST       14   // Reset matériel (RST / RESET / RES) – sortie 3.3V 
#define TFT_BL         7   // Rétroéclairage (BL / LED / BACKLIGHT) – sortie 3.3V 
#define TOUCH_CS      -1   // Pas de tactile câblé

#define ILI9341_MISO  TFT_MISO
#define ILI9341_MOSI  TFT_MOSI
#define ILI9341_SCLK  TFT_SCLK
#define ILI9341_CS    TFT_CS
#define ILI9341_DC    TFT_DC
#define ILI9341_RST   TFT_RST
#define ILI9341_BL    TFT_BL

// SD Card
#define SD_MISO       13   // MISO SD – entrée 3.3V
#define SD_MOSI       11   // MOSI SD – sortie 3.3V
#define SD_SCLK       12   // Horloge SD – sortie 3.3V
#define SD_CS          1   // Chip Select SD – sortie 3.3V

// ------------------------------------
// MOTOR CONTROLLER TB6612FNG
// ------------------------------------
#define M_AIN1        41   // Direction moteur A - Entrée 1
#define M_AIN2        42   // Direction moteur A - Entrée 2
#define M_PWMA        40   // Vitesse moteur A - PWM
#define M_BIN1        38   // Direction moteur B - Entrée 1
#define M_BIN2        39   // Direction moteur B - Entrée 2
#define M_PWMB        37   // Vitesse moteur B - PWM
#define M_STBY        21   // Standby (HIGH = actif)

// ------------------------------------
// ENTRÉES UTILISATEUR & ENCODEUR
// ------------------------------------
#define BUTTON_1      35   // Bouton user 1 – entrée 3.3V, pull-up interne
#define BUTTON_2      36   // Bouton user 2 – entrée 3.3V, pull-up interne

// Encodeur Rotatif (HW-040)
#define ROTARY_CLK    47   // Signal A / CLK / S1 – entrée 3.3V, ~10 nF [cite: 18]
#define ROTARY_DT     43   // Signal B / DT / S2 – entrée 3.3V (CORRIGÉ : libère 45)
#define ROTARY_SW     44   // Bouton SW – entrée 3.3V, pull-up interne [cite: 19]

// ------------------------------------
// CAPTEURS & SORTIES
// ------------------------------------
#define LED_RED       19   // LED Rouge – sortie 3.3V, R 220–470 O [cite: 6]
#define LED_GREEN     20   // LED Verte – sortie 3.3V, R 220–470 O [cite: 6]
#define LED_BLUE      45   // LED Bleue – sortie 3.3V, R 220–470 O [cite: 6]

#define DHT            5   // Capteur DHT – DATA 3.3V, pull-up 10 kO
#define MOTION_SENSOR 46   // PIR – entrée 3.3V (strapping GPIO46) [cite: 4]
#define LIGHT_SENSOR   4   // LDR via diviseur – entrée ADC 3.3V max [cite: 20]
#define BUZZER         6   // Commande buzzer – sortie 3.3V, transistor + R 1-10k [cite: 20]
#define PWM           14   // PWM générique – sortie 3.3V

// Distance HC-SR04
#define DISTANCE_TRIG  2   // TRIG – sortie 3.3V [cite: 21]
#define DISTANCE_ECHO 35   // ECHO – entrée 3.3V, diviseur obligatoire [cite: 5]

// Matrice NeoPixel
#define MATRIX8X8_PIN        3   // Data pin 8x8 matrix (CORRIGÉ : libère 18)
#define MATRIX8X8_NUMPIXELS 64   // Number of pixels in 8x8 matrix

// =========================================================
//         MAPPING ESP32 CLASSIQUE (DevKitC)
// =========================================================
#elif defined(TARGET_ESP32_CLASSIC)

    #define BOARD_NAME "ESP32 Classic DevKitC"

// ============================================================
// RAPPELS DE SÉCURITÉ ESP32-WROOM
// ============================================================
// - GPIO 3.3V uniquement[cite: 22].
// - GPIO0 : boot, éviter[cite: 22].
// - GPIO1/3 : UART0 console[cite: 22].
// - GPIO34–39 : entrées uniquement.
// - GPS TX et HC-SR04 ECHO : diviseur obligatoire si 5V.
// - LEDs : résistance série 220–470 O[cite: 24].
// - I2C : pull-up 4.7 kO obligatoire[cite: 24].
// ============================================================

// ------------------------------------
// SYSTÈME & BOOT
// ------------------------------------
#define BUTTON_BOOT    0   // Bouton BOOT – entrée 3.3V, vers GND
#define LED_BUILTIN    2   // LED intégrée (Soudée) – RÉSERVÉ SYSTÈME / BOOT [cite: 28]

// ------------------------------------
// COMMUNICATION (UART & I2C)
// ------------------------------------
// GPS (UART2)
#define GPS_RXD       16   // RX ESP32 – entrée 3.3V, diviseur si GPS 5V
#define GPS_TXD       17   // TX ESP32 – sortie 3.3V
#define GPS_PPS       36   // PPS GPS – entrée 3.3V (Input Only) 

// I2C
#define I2C_SDA       21   // SDA – open-drain 3.3V, pull-up 4.7 kO [cite: 27]
#define I2C_SCL       22   // SCL – open-drain 3.3V, pull-up 4.7 kO [cite: 27]

// ------------------------------------
// AFFICHAGE & STOCKAGE (SPI PARTAGÉ)
// ------------------------------------
// Bus SPI Physique
#define TFT_MISO      -1   // Non câblé sur ce module TFT
#define TFT_MOSI      23   // MOSI – sortie 3.3V
#define TFT_SCLK      18   // Horloge SPI – sortie 3.3V

// Contrôle TFT
#define TFT_CS        27   // Chip Select (CS/CE/SS) – sortie 3.3V [cite: 26]
#define TFT_DC        14   // Data/Command (D/C/A0/RS) – sortie 3.3V
#define TFT_RST       32   // Reset (RST/RESET/RES) – sortie 3.3V (CORRIGÉ : libère 25)
#define TFT_BL        12   // Backlight (BL/LED) – sortie 3.3V (CORRIGÉ : libère 32)

#define ILI9341_MISO  TFT_MISO
#define ILI9341_MOSI  TFT_MOSI
#define ILI9341_SCLK  TFT_SCLK
#define ILI9341_CS    TFT_CS
#define ILI9341_DC    TFT_DC
#define ILI9341_RST   TFT_RST
#define ILI9341_BL    TFT_BL

// SD Card
#define SD_MISO       19   // MISO SD – entrée 3.3V
#define SD_MOSI       23   // MOSI SD – sortie 3.3V (partagé avec TFT)
#define SD_SCLK       18   // Horloge SD – sortie 3.3V (partagé avec TFT)
#define SD_CS          5   // Chip Select SD – sortie 3.3V

// ------------------------------------
// MOTOR CONTROLLER TB6612FNG
// ------------------------------------
#define M_AIN1        26   // Direction moteur A - Entrée 1
#define M_AIN2        25   // Direction moteur A - Entrée 2
#define M_PWMA         4   // Vitesse moteur A - PWM
#define M_BIN1        13   // Direction moteur B - Entrée 1
#define M_BIN2        15   // Direction moteur B - Entrée 2
#define M_PWMB         2   // Vitesse moteur B - PWM (Attention GPIO2 au boot)
#define M_STBY        33   // Standby (HIGH = actif)

// ------------------------------------
// ENTRÉES UTILISATEUR & ENCODEUR
// ------------------------------------
#define BUTTON_1      34   // Bouton user 1 – entrée 3.3V (Input Only) 
#define BUTTON_2      35   // Bouton user 2 – entrée 3.3V (Input Only) 

// Encodeur Rotatif
#define ROTARY_CLK    39   // Signal A / CLK / S1 – entrée 3.3V (Input Only)
#define ROTARY_DT     36   // Signal B / DT / S2 – entrée 3.3V (Input Only)
#define ROTARY_SW      0   // Bouton SW – Partagé avec BOOT

// ------------------------------------
// CAPTEURS & SORTIES
// ------------------------------------
#define LED_RED       13   // LED Rouge – sortie 3.3V, R 220–470 O [cite: 24]
#define LED_GREEN     26   // LED Verte – sortie 3.3V, R 220–470 O [cite: 24, 28]
#define LED_BLUE      33   // LED Bleue – sortie 3.3V, R 220–470 O [cite: 24]

#define DHT           15   // Capteur DHT – DATA 3.3V, pull-up 10 kO
#define LIGHT_SENSOR  39   // LDR via diviseur – entrée ADC 3.3V max [cite: 30]
#define BUZZER        19   // Commande buzzer – sortie 3.3V, transistor + R 1-10k
#define PWM            4   // PWM générique – sortie 3.3V

// Distance
#define DISTANCE_TRIG 14   // TRIG – sortie 3.3V (CORRIGÉ : libère UART0 TX)
#define DISTANCE_ECHO 35   // ECHO – entrée 3.3V (Input Only), diviseur si 5V

// Matrice NeoPixel
#define MATRIX8X8_PIN        32   // Data pin matrix (CORRIGÉ : libère 27)
#define MATRIX8X8_NUMPIXELS 64   

#else
    #error "Aucune cible definie ! Verifiez platformio.ini (TARGET_ESP32_...)"
#endif

#endif // BOARD_CONFIG_H