# Journal des modifications

Tous les changements notables de Base_ESP32_Multi sont documentés dans ce fichier.

Le format est basé sur [Keep a Changelog](https://keepachangelog.com/fr/1.0.0/),
et ce projet adhère au [Versionnage Sémantique](https://semver.org/lang/fr/spec/v2.0.0.html).

---

## [0.9.0] - 2026-01-03

### 🚀 Ajouté

#### Système WiFi
- **Configuration WiFi multi-réseaux avec variables individuelles** (#1)
  - Nouveau format : `WIFI_SSID1`/`WIFI_PASS1`, `WIFI_SSID2`/`WIFI_PASS2`, etc.
  - Remplace l'ancien format tableau `WIFI_NETWORKS[][2]`
  - Jusqu'à 4 réseaux supportés par défaut (facilement extensible)
  - Créé `secrets_exemple.h` (EN) et `secrets_exemple_FR.h` (FR) templates
  - Connexion automatique au réseau disponible le plus fort
  - Basculement automatique entre réseaux

#### Mises à jour OTA
- **Mises à jour réseau ArduinoOTA** (#1)
  - Mises à jour OTA via Arduino IDE/PlatformIO
  - Port 3232 (port standard ArduinoOTA)
  - Retour visuel sur écran TFT pendant l'OTA
  - Indicateurs NeoPixel colorés (Bleu = mise à jour, Vert = succès, Rouge = erreur)
  - Barre de progression avec pourcentage et temps estimé
  - Gestion d'erreurs détaillée et logs

- **Interface OTA Web** (#2)
  - Nouvelle page `/ota` pour téléversement firmware via navigateur
  - Support glisser-déposer fichiers `.bin`
  - Suivi progression en temps réel avec XMLHttpRequest
  - Validation fichier (vérification extension `.bin`)
  - Messages statut inline (pas de popups)
  - Redirection auto après téléversement réussi
  - Gestion complète erreurs avec messages conviviaux

#### Support Matériel
- **Attribution GPIO Matrice NeoPixel 8×8** (#1)
  - ESP32-S3 : GPIO 3 pour Matrice NeoPixel (WS2812B-64)
  - ESP32 Classic : GPIO 2 pour Matrice NeoPixel (⚠️ conflit avec LED_BUILTIN)
  - Documentation exigences alimentation matrice 64 LEDs (3A @ 5V)
  - Code exemple compatible bibliothèque Adafruit_NeoMatrix

#### Documentation
- **Guides complets beginner-friendly (EN + FR) :**
  - `docs/GETTING_STARTED.md` / `docs/GETTING_STARTED_FR.md`
  - `docs/WIFI_SETUP.md` / `docs/WIFI_SETUP_FR.md`
  - `docs/OTA_UPDATE.md` / `docs/OTA_UPDATE_FR.md`
  - `docs/HARDWARE_SETUP.md` / `docs/HARDWARE_SETUP_FR.md`
- Sections dépannage complètes
- Schémas câblage et consignes sécurité
- Tables brochage GPIO pour toutes cartes supportées

### 🔄 Modifié

#### Système d'Affichage
- **Renommage constantes ST7789 en nommage TFT générique** (#1)
  - `ST7789_WIDTH` → `TFT_WIDTH`
  - `ST7789_HEIGHT` → `TFT_HEIGHT`
  - `ST7789_ROTATION` → `TFT_ROTATION`
  - Mise à jour toutes références dans `config.h`, `display.cpp`, `main.cpp`
  - Raison : Support types multiples écrans TFT (ST7789, ILI9341, ST7735, etc.)

#### Interface Web
- **Remplacement popups confirm() par validations inline** (#2)
  - Bouton redémarrage affiche maintenant messages statut inline
  - Upload OTA utilise barres progression et divs statut
  - Meilleure expérience utilisateur (pas dialogues bloquants)
  - Messages statut : info (bleu), succès (vert), erreur (rouge), avertissement (orange)

#### Gestion Version
- **Mise à jour PROJECT_VERSION vers 0.9.0** dans tous fichiers :
  - `platformio.ini` : Flags compilation et commentaires
  - `display.h` : En-tête fichier
  - `display.cpp` : En-tête fichier et date (2026-01-03)
  - `web_pages.h` : En-tête fichier

### 🐛 Corrigé

- **Fonctions page OTA** (#2)
  - Ajout fonction manquante `generateOTAPage()` dans `web_pages.h`
  - Ajout fonction manquante `generateOTAResultPage()` dans `web_pages.h`
  - Correction erreurs compilation liées fonctions OTA non déclarées

- **Configuration WiFi** (#2)
  - Création template `secrets.h` depuis `secrets_exemple.h`
  - Correction erreurs compilation "WIFI_SSID not declared"
  - Ajout documentation pour créer `secrets.h` localement

### 📝 Documentation

- **Nouveaux guides complets** (bilingue EN/FR) :
  - Démarrage : Installation, configuration initiale, vérification
  - Configuration WiFi : Config multi-réseaux, sécurité, dépannage
  - Mises à jour OTA : Méthodes OTA web et réseau
  - Configuration Matériel : Brochages GPIO, guides câblage, pratiques sécurité

- **README mis à jour** (EN/FR) :
  - Fonctionnalités version 0.9.0
  - Guide démarrage rapide
  - Liens nouvelle documentation

### 🔒 Sécurité

- **Isolation identifiants WiFi :**
  - `secrets.h` ajouté à `.gitignore`
  - Jamais commité au contrôle de version
  - Fichiers template fournis pour configuration facile

- **Documentation sécurité OTA :**
  - Exemples protection par mot de passe
  - Bonnes pratiques sécurité réseau
  - Avertissements utilisation réseaux ouverts

### ⚙️ Détails Techniques

**Commits :**
- aa70c5b - feat: Major refactor v0.9.0 - WiFi system, OTA, TFT naming, NeoPixel Matrix
- 87cecc2 - fix: Complete v0.9.0 implementation - OTA pages + version update + build fixes

**Fichiers Modifiés :**
- `include/secrets_exemple.h` (créé)
- `include/secrets_exemple_FR.h` (créé)
- `include/board_config.h` (GPIO Matrice NeoPixel ajouté)
- `include/config.h` (renommage ST7789 → TFT)
- `include/web_interface.h` (handlers OTA ajoutés)
- `include/web_pages.h` (générateurs pages OTA ajoutés, validations inline)
- `include/display.h` (version mise à jour)
- `src/main.cpp` (refonte WiFi, intégration ArduinoOTA)
- `src/display.cpp` (constantes TFT mises à jour, version/date mises à jour)
- `platformio.ini` (version mise à jour vers 0.9.0)
- `docs/` (8 nouveaux fichiers documentation)

**Lignes Modifiées :**
- +616 insertions
- -69 suppressions
- 11 fichiers modifiés

---

## [0.8.2-rc.1] - Version Précédente

Voir historique git pour détails versions précédentes.

---

## Légende

- 🚀 **Ajouté** : Nouvelles fonctionnalités
- 🔄 **Modifié** : Changements fonctionnalité existante
- 🐛 **Corrigé** : Corrections bugs
- 🗑️ **Déprécié** : Fonctionnalités bientôt supprimées
- ❌ **Supprimé** : Fonctionnalités supprimées
- 🔒 **Sécurité** : Améliorations sécurité
- 📝 **Documentation** : Changements documentation

---

## Versionnage

Ce projet suit le [Versionnage Sémantique](https://semver.org/lang/fr/) :
- **MAJEUR** (X.0.0) : Changements API incompatibles
- **MINEUR** (0.X.0) : Nouvelle fonctionnalité rétrocompatible
- **PATCH** (0.0.X) : Corrections bugs rétrocompatibles

**Classification v0.9.0 :**
- Bump version MINEUR (0.8 → 0.9)
- Raison : Nouvelles fonctionnalités (WiFi multi-réseaux, OTA, Matrice NeoPixel) ajoutées
- Rétrocompatibilité : Majoritairement compatible, mais nécessite migration `secrets.h`

---

## Guide Migration : 0.8.x → 0.9.0

### Changements Incompatibles

1. **Format Configuration WiFi :**
   - **Ancien format (supprimé) :**
     ```cpp
     const char* WIFI_NETWORKS[][2] = {
         {"SSID1", "PASS1"},
         {"SSID2", "PASS2"}
     };
     ```
   - **Nouveau format (requis) :**
     ```cpp
     const char* WIFI_SSID1 = "SSID1";
     const char* WIFI_PASS1 = "PASS1";
     const char* WIFI_SSID2 = "SSID2";
     const char* WIFI_PASS2 = "PASS2";
     ```

2. **Fichier secrets.h :**
   - Doit créer `include/secrets.h` depuis template :
     ```bash
     cp include/secrets_exemple_FR.h include/secrets.h
     ```
   - Éditer avec vos identifiants WiFi

3. **Constantes Affichage (migration optionnelle) :**
   - Anciennes constantes `ST7789_*` fonctionnent toujours mais dépréciées
   - Recommandé : Mettre à jour code personnalisé pour utiliser constantes `TFT_*`

### Nouvelles Fonctionnalités à Activer

1. **Mises à jour OTA :**
   - Déjà activées par défaut (ArduinoOTA + Web OTA)
   - Accès interface web : `http://<IP-ESP32>/ota`
   - Optionnel : Ajouter protection mot de passe (voir OTA_UPDATE_FR.md)

2. **Matrice NeoPixel :**
   - Décommenter dans `board_config.h` :
     ```cpp
     #define HAS_NEOPIXEL_MATRIX
     ```
   - Câbler sur GPIO 3 (ESP32-S3) ou GPIO 2 (ESP32 Classic)

---

Pour changements détaillés, voir [historique commits](https://github.com/morfredus/Base_ESP32_Multi/commits/main).
