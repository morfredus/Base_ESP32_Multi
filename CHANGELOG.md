# Changelog

Toutes les modifications notables de ce projet seront documentées dans ce fichier.

Le format est basé sur [Keep a Changelog](https://keepachangelog.com/fr/1.0.0/),
et ce projet adhère au [Semantic Versioning](https://semver.org/lang/fr/).

## [0.4.0] - 2025-12-04

### Ajouté
- 📊 Page web enrichie avec informations techniques complètes :
  - Section Matériel : Board, Chip ID, SDK, Fréquence CPU, Nombre de cœurs
  - Section Mémoire Flash : Taille, vitesse, mode
  - Section RAM : Heap et PSRAM avec barres de progression visuelles
  - Section WiFi : SSID, IP, masque, passerelle, DNS, MAC, signal
  - Section Système : Uptime, température CPU
- 🎨 Interface web améliorée avec sections colorées et organisation claire
- 📊 Barres de progression pour visualiser l'utilisation de la mémoire
- 🌡️ Affichage de la température du CPU

### Modifié
- 🎨 Design responsive adapté aux informations supplémentaires
- ⚠️ Confirmation avant redémarrage via l'interface web

## [0.3.0] - 2025-12-04

### Ajouté
- 🌐 Serveur web intégré sur le port 80
- 📄 Page d'accueil affichant les informations système (version, board, WiFi, IP, signal, uptime)
- 🔄 Bouton de redémarrage via l'interface web
- 🔧 Fonction de redémarrage opérationnelle via appui long sur le bouton

### Corrigé
- 🐛 Appui long sur le bouton BOOT maintenant fonctionnel (redémarre l'ESP32)
- 🐛 Suppression du delay bloquant dans le callback du bouton
- ✅ Confirmation visuelle sur NeoPixel (violet) et OLED lors du redémarrage

## [0.2.0] - 2025-12-04

### Ajouté
- ✨ Support des écrans OLED SSD1306 (128x64 pixels)
- 📊 Affichage automatique du nom du projet et de la version sur l'OLED
- 📶 Barre de progression visuelle durant la connexion WiFi sur l'OLED
- 🌐 Affichage du SSID et de l'adresse IP une fois connecté sur l'OLED
- ⚙️ Configuration simple via `HAS_OLED` dans `config.h`
- 📚 Librairies Adafruit GFX et Adafruit SSD1306 ajoutées aux dépendances

### Documenté
- 📖 Section dédiée à la configuration OLED dans le README
- 🔌 Instructions de connexion I2C pour ESP32-S3 et ESP32 Classic
- 💡 Guide pour désactiver l'OLED si non utilisé

## [0.1.0] - 2025-12-04

### Ajouté
- 🔧 Configuration multi-environnements PlatformIO
  - ESP32-S3 N16R8 (16MB Flash / 8MB PSRAM)
  - ESP32-S3 N8R8 (8MB Flash / 8MB PSRAM)
  - ESP32 Classic DevKitC (4MB Flash)
- 📡 Gestion WiFiMulti avec support de plusieurs réseaux
- 🔒 Système de secrets (fichier `secrets.h` exclu de Git)
- 💡 Support NeoPixel (LED RGB sur pin 48 pour ESP32-S3)
- 🚦 Feedback visuel de l'état WiFi :
  - Jaune : Connexion en cours
  - Vert : Connecté
  - Rouge : Erreur/déconnecté
- 🔘 Gestion du bouton BOOT avec librairie OneButton
  - Clic simple : Action personnalisable
  - Appui long : Action longue personnalisable
- 📁 Structure de projet modulaire :
  - `config.h` : Configuration générale
  - `board_config.h` : Configuration matérielle spécifique
  - `secrets.h` : Identifiants WiFi (non versionné)
- 📊 Logs série avec décodeur d'exceptions ESP32
- ⚡ Compilation optimisée avec C++17
- 🎯 Heartbeat non-bloquant dans le loop

### Configuration
- ⚙️ Support PSRAM pour ESP32-S3 (QIO + OPI)
- 📦 Dossiers de build personnalisés pour optimiser l'espace disque
- 🚀 Vitesse d'upload configurée à 921600 bauds
- 🔍 Filtres de monitoring série (timestamps + décodeur d'exceptions)

---

## Format des versions

- **MAJEUR** : Changements incompatibles avec les versions précédentes
- **MINEUR** : Ajout de fonctionnalités compatibles avec les versions précédentes
- **CORRECTIF** : Corrections de bugs compatibles avec les versions précédentes
