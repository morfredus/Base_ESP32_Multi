# Changelog

Toutes les modifications notables de ce projet seront documentées dans ce fichier.

Le format est basé sur [Keep a Changelog](https://keepachangelog.com/fr/1.0.0/),
et ce projet adhère au [Semantic Versioning](https://semver.org/lang/fr/).

**[English Version](CHANGELOG.md)**

## [0.8.2] - 2025-12-13

### Modifié
- ⚡ **Optimisation TFT ST7789** : Utilisation du SPI matériel au lieu du SPI logiciel
  - Nouvelle déclaration : `Adafruit_ST7789(&SPI, PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST)`
  - Ajout de `SPI.begin()` dans `setupST7789()` pour initialisation explicite
  - Amélioration des performances d'affichage
- 🎨 **Couleurs Adafruit** : Utilisation des constantes officielles de la bibliothèque
  - Remplacement de `COLOR_*` par `ST77XX_*` (ex: `COLOR_BLACK` → `ST77XX_BLACK`)
  - Suppression des définitions personnalisées dans `config.h`
  - Cohérence avec les standards de la bibliothèque Adafruit

### Corrigé
- 🔧 Affichage TFT plus fluide grâce au SPI matériel
- 📦 Réduction du code avec suppression de 11 lignes de définitions redondantes

### Documentation
- 📘 [CHANGES_v0.8.2.md](docs/CHANGES_v0.8.2.md) - Documentation technique complète des optimisations

## [0.8.1] - 2025-12-13

### Corrigé
- 🔧 **Cohérence des noms de pins** : Tous les noms de pins utilisés dans le code correspondent maintenant exactement aux déclarations dans `board_config.h`
  - `display.cpp` : `TFT_CS`, `TFT_DC`, `TFT_RST`, `TFT_BL` → `PIN_TFT_CS`, `PIN_TFT_DC`, `PIN_TFT_RST`, `PIN_TFT_BL`
  - `main.cpp` : `DEFAULT_BUZZER_PIN` → `PIN_BUZZER` (conformément à board_config.h)
  - Ajout de `PIN_LED_BUILTIN` dans board_config.h pour ESP32 Classic (GPIO 2)
- 📋 **board_config.h** comme référence unique pour tous les noms de pins du projet

### Documentation
- 📝 Mise à jour de la documentation pour refléter la cohérence des noms de pins

## [0.8.0] - 2025-12-13

### Ajouté
- 🎮 **Système de contrôle multi-boutons avancé** :
  - Bouton BOOT : Redémarrage avec barre de progression 2 sec (annulable avant 100%)
  - Bouton 1 : Cycle RGB (Rouge → Vert → Bleu → Blanc → Éteint)
  - Bouton 2 : Bip Buzzer à l'appui (1kHz, 100ms)
- 🎨 **Gestion améliorée de la NeoPixel** :
  - Violet fixe pendant séquence reboot
  - Vert heartbeat quand WiFi connecté
  - Rouge heartbeat quand WiFi déconnecté/recherche
- 🔊 **Retour sonore** : Buzzer piézo pour feedback utilisateur (Pin 6)
- 🚨 **Sécurité de redémarrage** : Annulation possible avant 100% avec restauration écran
- 📊 **Barre de progression TFT** : Affichage en magenta, mise à jour chaque 50ms

### Modifié
- 🔄 **main.cpp** : Refonte complète de la gestion des boutons
  - Ajout de 3 objets OneButton (BOOT, BTN1, BTN2)
  - Nouvelles fonctions callback : `handleButton1Click()`, `handleButton2PressStart()`, `handleButton2PressStop()`
  - Modification `handleLongPress()` avec logique de reboot à 100%
  - Amélioration du heartbeat NeoPixel avec condition `isRebooting`
- ⚙️ **config.h** : Activation des définitions HAS_LED_RGB et HAS_NEOPIXEL
- 🎯 **Logique reboot** : Vérification directe du pin via `digitalRead()` pour fiabilité

### Corrigé
- 🐛 LED RGB maintenant controlée correctement (definition HAS_LED_RGB)
- 🐛 NeoPixel affichant tous les états WiFi + reboot
- 🐛 Reboot ne se déclenche que si barre à 100% + bouton toujours appuyé
- 🐛 Restauration automatique de l'écran WiFi après annulation reboot

### Documentation
- 📋 Ajout de `CHANGES_v0.8.0.md` : Document détaillé des 10 changements numérotés
- 📊 Tableaux récapitulatifs des pins et comportements
- 🧪 Section tests effectués avec résultats

## [0.7.0] - 2025-12-06

### Ajouté
- 🖥️ **Support complet des écrans TFT ST7789** : Affichage couleur haute résolution (240x240 ou 240x135)
- 📱 **Module display.h/display.cpp** : Architecture modulaire pour gérer OLED et ST7789 de manière unifiée
- 🎨 **Interface de démarrage graphique** :
  - Affichage du nom du projet et de la version au boot
  - Barre de progression visuelle durant la connexion WiFi
  - Affichage du SSID et de l'adresse IP une fois connecté
  - Messages d'erreur en cas d'échec de connexion
- 📚 **Documentation complète pour débutants** :
  - `docs/PIN_MAPPING.md` : Guide détaillé de connexion des composants
  - Schémas de câblage pour ESP32-S3 et ESP32 Classic
  - Tableaux récapitulatifs des GPIO utilisés
  - Conseils de dépannage et sécurité électrique
- ⚙️ **Configuration flexible** :
  - Support simultané OLED + ST7789
  - Activation/désactivation indépendante via `config.h`
  - Palette de couleurs prédéfinies (RGB565) pour ST7789

### Modifié
- 🎨 **main.cpp refactorisé** : Fonctions d'affichage déléguées au module `display.h`
- 🔧 **Simplification des includes** : Suppression des includes OLED directs dans main.cpp
- 📦 **platformio.ini** : Activation de la bibliothèque Adafruit ST7789
- 📏 **Amélioration de board_config.h** : Ajout de commentaires explicatifs pour chaque pin

### Corrigé
- 🐛 Gestion cohérente de l'affichage entre OLED et ST7789
- 🐛 Message de redémarrage affiché sur tous les écrans disponibles

### Documentation
- 📘 Ajout de `docs/PIN_MAPPING.md` : Guide complet de connexion matérielle
- 📝 Commentaires Doxygen dans `display.h` pour tous les prototypes
- ✅ Liste de vérification pré-compilation pour débutants

## [0.6.0] - 2025-12-04

### Ajouté
- 🏗️ **Architecture modulaire pour l'interface web** :
  - `web_styles.h` : Styles CSS séparé et réutilisable
  - `web_pages.h` : Générateur HTML avec fonction `generateDashboardPage()`
  - `web_interface.h` : Handlers HTTP et gestion du serveur web
- 📚 **Documentation Doxygen complète** : Tous les modules incluent des commentaires Doxygen
- 🔧 **Meilleure séparation des responsabilités** : Logique métier séparée de la présentation
- ♻️ **Code réutilisable** : Les fonctions HTML/CSS peuvent être réutilisées dans d'autres projets

### Modifié
- 🧹 **main.cpp allégé** : De 424 à 271 lignes (36% de réduction)
- 🎯 **Imports simplifiés** : `#include "web_interface.h"` gère tous les besoins web
- 🔄 **Handlers délégués** : `handleRoot()`, `handleReboot()` maintenant dans modules dédiés
- ⚙️ **Fonction setup()** : Utilise maintenant `setupWebServer()` au lieu de config inline

### Corrigé
- 🐛 Organisation du code pour éviter la duplication
- 📦 Réduction de l'empreinte du code binaire (moins en RAM, plus en Flash)

### Documentation
- 📂 **Réorganisation des fichiers de documentation** :
  - Création du dossier `docs/` pour centraliser la documentation technique
  - `docs/ARCHITECTURE.md` : Guide technique complet sur la structure modulaire
  - `docs/UPGRADE_0.6.0.md` : Résumé des changements et étapes de migration
  - `docs/COHERENCE_CHECK.md` : Vérification de cohérence des versions
  - `docs/SYNTHESIS.md` : Synthèse complète avec comparaisons avant/après
- 🎯 **Conformité aux conventions** : README.md et CHANGELOG.md restent à la racine, détails techniques en `docs/`
- 🔗 **Références croisées** : Tous les fichiers utilisent des chemins relatifs pour portabilité

## [0.5.0] - 2025-12-04

### Ajouté
- 🎨 **Refonte majeure de l'interface web** : Design moderne avec cartouches (cards)
- 📱 **Présentation compacte et responsive** : Grille CSS auto-responsive (mobile-friendly)
- 🎯 **Gradient moderne** : Fond dégradé violet (667eea → 764ba2)
- 📊 **Meilleure lisibilité** : Organisation des informations par catégories dans des cartes distinctes
- 🔗 **Séparation des détails réseau** : Carte dédiée pour Masque/Passerelle/DNS
- ✨ **Animations visuelles** : Hover effects et transitions sur les boutons et cartes
- 📈 **Barre de signal WiFi** : Conversion dBm en pourcentage avec visualisation par progression
- ⏱️ **Format uptime amélioré** : Affichage en "Xh Ym Zs" au lieu de simples secondes
- 🔴 **Boutons redessinés** : Style gradient avec icônes (🔄 Actualiser / 🔴 Redémarrer)
- 💾 **Monospace pour valeurs techniques** : IP, MAC, Chip ID en police monospace
- 🌙 **Header en contraste** : Titre et version en blanc sur le gradient

### Modifié
- 🎨 CSS complètement réécrit pour le design card-based
- 📐 Structure HTML optimisée pour flexibilité et maintenabilité
- 🔄 Boutons réorganisés en section "actions" centralisée

### Corrigé
- 🐛 Amélioration de la lisibilité sur petits écrans
- 🐛 Consistance visuelle entre tous les éléments de l'interface

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
