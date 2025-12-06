# Base_ESP32_S3

Template de démarrage universel pour projets ESP32 et ESP32-S3 sous PlatformIO.
Inclut la gestion automatique du WiFi, une structure multi-environnements et le support des LED de statut (NeoPixel), écrans OLED et écrans TFT couleur.

## 🚀 Fonctionnalités
* **Multi-Cartes** : Prêt pour ESP32-S3 (N16R8, N8R8) et ESP32 Classic (DevKitC).
* **WiFiMulti** : Tente de se connecter à une liste de réseaux connus.
* **Serveur Web Modulaire** *(v0.6.0)* : Interface web moderne avec architecture modulaire - styles CSS séparé, générateur HTML flexible, handlers organisés. Accessible sur http://[IP_ESP32].
* **Sécurité** : Les identifiants WiFi sont exclus de Git (`secrets.h`).
* **Feedback Visuel** : Gestion automatique de la LED RGB (Pin 48 sur S3) pour indiquer l'état WiFi (Jaune=Connexion, Vert=OK, Rouge=Erreur).
* **Affichage Dual-Screen** *(v0.7.0)* : Support simultané OLED SSD1306 (128x64) et TFT ST7789 (240x240 couleur) avec interface unifiée.
  - Affichage du nom du projet et version au démarrage
  - Barre de progression de connexion WiFi
  - SSID et adresse IP une fois connecté
* **Bouton Intelligent** : Appui long (1s) redémarre l'ESP32 avec confirmation visuelle.
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

**Pour en savoir plus** : Voir [docs/ARCHITECTURE.md](./docs/ARCHITECTURE.md)

## 📋 Changelog

Pour consulter l'historique complet des versions et modifications, voir [CHANGELOG.md](CHANGELOG.md).

**Documentation technique** : Consultez [docs/](./docs) pour les guides détaillés et références techniques.

### Version actuelle : v0.7.0 (2025-12-06)
**Nouveautés principales :**
- 🖥️ **Support TFT ST7789** : Écrans couleur haute résolution 240x240
- 🎨 **Module display.h** : Interface unifiée pour OLED + ST7789
- 📚 **Documentation PIN_MAPPING** : Guide complet de connexion des pins (débutants)
- 🎯 **Interface graphique** : Affichage du nom, version, progression WiFi, IP sur les écrans