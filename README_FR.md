# Base_ESP32_Multi

Template de démarrage universel pour projets ESP32 et ESP32-S3 sous PlatformIO.
Inclut la gestion automatique du WiFi, une structure multi-environnements et le support des LED de statut (NeoPixel), écrans OLED et écrans TFT couleur.

**Version actuelle : 0.9.1**

**[English Version](README.md)**

## 🚀 Fonctionnalités

* **Multi-Cartes** : Prêt pour ESP32-S3 (N16R8, N8R8) et ESP32 Classic (DevKitC).
* **Trois environnements PlatformIO** :
  - `esp32s3_n16r8` : ESP32-S3 avec 16MB Flash / 8MB PSRAM (principal)
  - `esp32s3_n8r8` : ESP32-S3 avec 8MB Flash / 8MB PSRAM
  - `esp32devkitc` : ESP32 Classic avec 4MB Flash
* **WiFiMulti** : Tente de se connecter à une liste de réseaux connus (jusqu'à 4 réseaux).
* **Mises à jour OTA** *(v0.9.0)* :
  - ArduinoOTA via réseau (port 3232)
  - Interface OTA Web sur `/ota`
* **Serveur Web Modulaire** *(v0.6.0)* : Interface web moderne avec architecture modulaire - styles CSS séparé, générateur HTML flexible, handlers organisés. Accessible sur http://[IP_ESP32].
* **Sécurité** : Les identifiants WiFi sont exclus de Git (`secrets.h`).
* **Feedback Visuel** : Gestion automatique de la LED RGB (Pin 48 sur S3, Pin 2 sur Classic via fallback) pour indiquer l'état WiFi (Jaune=Connexion, Vert=OK, Rouge=Erreur).
* **Affichage Dual-Screen** *(v0.7.0)* : Support simultané OLED SSD1306 (128x64) et TFT ST7789 (240x240 couleur) avec interface unifiée.
  - Affichage du nom du projet et version au démarrage
  - Barre de progression de connexion WiFi
  - SSID et adresse IP une fois connecté
* **Système de boutons avancé** *(v0.8.0)* :
  - **Bouton BOOT** : Reboot avec barre de progression 2 sec (annulable avant 100%)
  - **Bouton 1** : Cycle RGB (Rouge → Vert → Bleu → Blanc → Éteint)
  - **Bouton 2** : Bip Buzzer à l'appui (feedback sonore)
* **NeoPixel intelligente** *(v0.8.0+)* :
  - Violet fixe pendant redémarrage
  - Vert heartbeat quand WiFi connecté
  - Rouge heartbeat quand WiFi en recherche/déconnecté
  - Fallback automatique vers le pin NEOPIXEL_MATRIX sur ESP32 Classic
* **Support Matrice NeoPixel 8×8** *(v0.9.0)* :
  - ESP32-S3 : GPIO 3
  - ESP32 Classic : GPIO 2 (⚠️ partagé avec LED_BUILTIN)
* **Structure Propre** : Séparation de la config projet (`config.h`), mapping hardware (`board_config.h`) et gestion d'affichage (`display.h`).

## 🛠️ Installation & Démarrage

### 1. Cloner le projet
Récupérez ce dossier et renommez-le selon votre nouveau projet.

### 2. Créer le fichier `secrets.h`
Le fichier `include/secrets.h` est ignoré par Git. Vous devez le créer manuellement :

```cpp
// include/secrets.h
#ifndef SECRETS_H
#define SECRETS_H

const char* WIFI_NETWORKS[][2] = {
    {"SSID_Maison", "MotDePasse123"},
    {"SSID_Bureau", "MotDePasse456"},
    {"Partage_Tel", "12345678"}
};

#endif
```

### 3. Configurer les écrans (optionnel)
Les écrans OLED et ST7789 sont activés par défaut dans `include/config.h`.

#### 📍 Connexion des écrans

Pour obtenir les schémas de connexion détaillés et les tableaux GPIO complets, consultez :
**→ [docs/PIN_MAPPING.md](./docs/PIN_MAPPING.md)** 

Guide complet incluant :
- Tableaux récapitulatifs des GPIO pour ESP32-S3 et ESP32 Classic
- Schémas de câblage (ASCII art)
- Conseils de dépannage
- Configuration I2C vs SPI

#### ⚙️ Configuration logicielle

Dans `include/config.h`, vous pouvez activer/désactiver les écrans :

```cpp
// OLED SSD1306 (I2C)
#define HAS_OLED          // Décommenter pour activer

// TFT ST7789 (SPI couleur)
#define HAS_ST7789        // Décommenter pour activer
```

**Note** : Vous pouvez utiliser les deux écrans simultanément !

#### Affichage automatique
- **Au démarrage** : Nom du projet et version
- **Connexion WiFi** : Barre de progression (0-100%)
- **Connecté** : SSID du réseau et adresse IP attribuée
- **Erreur** : Message d'échec de connexion

Pour désactiver un écran, commentez simplement sa ligne `#define` dans `config.h`.

## 🏗️ Architecture Modulaire (v0.6.0+)

Le projet utilise une architecture modulaire pour l'interface web, séparant les responsabilités :

### Modules Web

| Module | Fichier | Responsabilité |
|--------|---------|-----------------|
| **Styles** | `include/web_styles.h` | Contient tout le CSS réutilisable (flexbox, gradients, animations) |
| **Pages** | `include/web_pages.h` | Générateur HTML avec fonction `generateDashboardPage()` |
| **Interface** | `include/web_interface.h` | Handlers HTTP et configuration du serveur web |

### Modules d'Affichage (v0.7.0+)

| Module | Fichier | Responsabilité |
|--------|---------|-----------------|
| **Display** | `include/display.h` | Interface unifiée pour OLED et ST7789 |
| **Display Impl** | `src/display.cpp` | Implémentation des fonctions d'affichage |

### Avantages de cette architecture

- ✅ **Modularité** : Chaque module a une responsabilité unique
- ✅ **Réutilisabilité** : Les styles et générateurs peuvent être utilisés dans d'autres projets
- ✅ **Maintenabilité** : Modifications faciles sans affecter le reste du code
- ✅ **Testabilité** : Chaque module peut être testé indépendamment
- ✅ **Documentation** : Code Doxygen pour tous les modules

### Exemple d'utilisation

```cpp
// main.cpp
#include "display.h"  // Interface d'affichage unifiée

// Setup
setupDisplays();      // Initialise OLED et/ou ST7789
displayStartup(PROJECT_NAME, PROJECT_VERSION);  // Écran de démarrage

// Loop
displayWifiProgress(progress);  // Pendant la connexion WiFi
displayWifiConnected(ssid, ip);  // Une fois connecté
```

**Pour en savoir plus** : Voir [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)

## 📋 Changelog

Pour consulter l'historique complet des versions et modifications, voir [CHANGELOG.md](CHANGELOG.md).

**Documentation technique** : Consultez [docs/](docs/) pour les guides détaillés et références techniques.

### Version actuelle : v0.9.1 (2026-01-03)

**Nouveautés de la v0.9.1 (Correctif) :**
- 🐛 **Correction erreur de compilation NeoPixel sur ESP32 Classic** : Ajout d'un fallback automatique vers `NEOPIXEL_MATRIX` quand `NEOPIXEL` n'est pas défini
- 📝 **Commentaires améliorés** : Documentation pédagogique pour débutants dans `main.cpp` et `config.h`
- 📋 **Cohérence de version** : Tous les fichiers référencent maintenant v0.9.1

**Nouveautés de la v0.9.0 :**
- 🌐 **WiFi multi-réseaux** : Support jusqu'à 4 réseaux avec basculement automatique
- 📡 **Mises à jour OTA** : ArduinoOTA (port 3232) + OTA Web sur `/ota`
- 🔲 **Matrice NeoPixel 8×8** : Support GPIO ajouté (GPIO 3 sur S3, GPIO 2 sur Classic)
- 📚 **Documentation complète** : Guides bilingues (EN/FR) pour débutants

**Nouveautés des v0.8.x :**
- ⚡ **SPI matériel pour TFT** (v0.8.2)
- 🎮 **Système de boutons avancé** : BOOT (reboot), BTN1 (RGB), BTN2 (Buzzer) (v0.8.0)
- 🎨 **NeoPixel intelligente** : Indique WiFi et état de reboot (v0.8.0)

## 📚 Documentation

Toute la documentation est disponible dans le dossier `docs/` en français et en anglais :
- [docs/INDEX_FR.md](docs/INDEX_FR.md) - Table des matières
- [docs/PIN_MAPPING_FR.md](docs/PIN_MAPPING_FR.md) - Guide complet des pins
- [docs/ARCHITECTURE_FR.md](docs/ARCHITECTURE_FR.md) - Architecture logicielle

## 🤝 Contribuer

Les contributions sont les bienvenues ! N'hésitez pas à soumettre des pull requests ou à ouvrir des issues.

## 📜 Licence

Ce projet est sous licence MIT - voir le fichier LICENSE pour plus de détails.

---

**Version du projet** : 0.9.1
**Dernière mise à jour** : 3 janvier 2026
**Cartes compatibles** : ESP32-S3 DevKitC-1 (N16R8, N8R8), ESP32 Classic DevKitC

## 📌 Résumé du Mapping des Pins

### ESP32-S3 DevKitC-1

| Fonction | GPIO | Notes |
|----------|------|-------|
| NeoPixel (simple) | 48 | LED RGB adressable |
| Matrice NeoPixel 8×8 | 3 | WS2812B-64 |
| TFT (SPI) | CS=10, DC=9, RST=14, BL=7 | ST7789 240x240 |
| I2C (OLED) | SDA=15, SCL=16 | SSD1306 128x64 |
| Boutons | BOOT=0, BTN1=38, BTN2=39 | Actif BAS |
| Buzzer | 6 | PWM |
| LED RGB | R=21, G=41, B=42 | LEDs standards |

### ESP32 Classic DevKitC

| Fonction | GPIO | Notes |
|----------|------|-------|
| Matrice NeoPixel 8×8 | 2 | ⚠️ Partagé avec LED_BUILTIN |
| TFT (SPI) | CS=27, DC=14, RST=25, BL=32 | ST7789 |
| I2C (OLED) | SDA=21, SCL=22 | SSD1306 |
| Boutons | BOOT=0, BTN1=5, BTN2=12 | Actif BAS |
| Buzzer | 19 | PWM |
| LED RGB | R=13, G=26, B=33 | LEDs standards |

Pour le mapping complet des pins, voir [docs/PIN_MAPPING_FR.md](docs/PIN_MAPPING_FR.md).