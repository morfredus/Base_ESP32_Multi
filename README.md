# Base_ESP32_S3

Template de démarrage universel pour projets ESP32 et ESP32-S3 sous PlatformIO.
Inclut la gestion automatique du WiFi, une structure multi-environnements et le support des LED de statut (NeoPixel) et écrans OLED.

## 🚀 Fonctionnalités
* **Multi-Cartes** : Prêt pour ESP32-S3 (N16R8, N8R8) et ESP32 Classic (DevKitC).
* **WiFiMulti** : Tente de se connecter à une liste de réseaux connus.
* **Serveur Web** *(v0.5.0)* : Interface web moderne avec design par cartouches (cards), responsive et facile à lire, affichant toutes les infos système (version, board, WiFi, RAM, Flash, PSRAM, uptime, température).
* **Sécurité** : Les identifiants WiFi sont exclus de Git (`secrets.h`).
* **Feedback Visuel** : Gestion automatique de la LED RGB (Pin 48 sur S3) pour indiquer l'état WiFi (Jaune=Connexion, Vert=OK, Rouge=Erreur).
* **Affichage OLED** : Support des écrans SSD1306 128x64 avec affichage du nom du projet, version, progression de connexion WiFi, SSID et adresse IP.
* **Bouton Intelligent** : Appui long (1s) redémarre l'ESP32 avec confirmation visuelle.
* **Structure Propre** : Séparation de la config projet (`config.h`) et du mapping hardware (`board_config.h`).

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

### 3. Configurer l'OLED (optionnel)
Si vous utilisez un écran OLED SSD1306, le support est activé par défaut dans `include/config.h`.

**Connexion I2C :**
- **ESP32-S3** : SDA=GPIO42, SCL=GPIO41
- **ESP32 Classic** : SDA=GPIO21, SCL=GPIO22
- **Adresse I2C** : 0x3C (par défaut, modifiable dans `config.h`)

**Affichage automatique :**
- Nom du projet et version au démarrage
- Barre de progression durant la connexion WiFi
- SSID et adresse IP une fois connecté

Pour désactiver l'OLED, commentez `#define HAS_OLED` dans `config.h`.

## 📋 Changelog

Pour consulter l'historique complet des versions et modifications, voir [CHANGELOG.md](CHANGELOG.md).

### Version actuelle : v0.5.0 (2025-12-04)
**Nouveautés principales :**
- 🎨 **Interface web redessinée** : Design moderne avec cartouches (cards) pour meilleure lisibilité
- 📱 **Responsive & Compact** : Grille auto-responsive adaptée à tous les écrans
- 🎯 **Visuellement attrayant** : Gradient moderne, animations, boutons icônisés
- 📊 **Données organisées** : 7 cartes distinctes pour Matériel, Flash, RAM, PSRAM, WiFi, Système, Réseau détaillé
- ✨ **Améliorations UX** : Barres de progression pour signal WiFi, uptime en format lisible (Xh Ym Zs)