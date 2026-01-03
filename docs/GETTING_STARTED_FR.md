# Démarrage avec Base_ESP32_Multi

**Version :** 0.9.0
**Dernière mise à jour :** 2026-01-03

Bienvenue dans Base_ESP32_Multi ! Ce guide vous aidera à démarrer votre projet ESP32 en quelques minutes.

---

## 📋 Table des matières

1. [Prérequis](#prérequis)
2. [Démarrage rapide](#démarrage-rapide)
3. [Configuration initiale](#configuration-initiale)
4. [Compilation et téléversement](#compilation-et-téléversement)
5. [Vérification de l'installation](#vérification-de-linstallation)
6. [Prochaines étapes](#prochaines-étapes)
7. [Dépannage](#dépannage)

---

## Prérequis

Avant de commencer, assurez-vous d'avoir :

### Matériel
- ESP32-S3 DevKitC-1 (recommandé) ou ESP32 Classic DevKitC
- Câble USB-C (pour ESP32-S3) ou Micro-USB (pour ESP32 Classic)
- Ordinateur sous Windows, macOS ou Linux

### Logiciels
- [Visual Studio Code](https://code.visualstudio.com/)
- [Extension PlatformIO IDE](https://platformio.org/install/ide?install=vscode)
- Git (optionnel, pour cloner le dépôt)

### Connaissances
- Compréhension de base de la programmation Arduino/ESP32
- Familiarité avec la ligne de commande (optionnel mais utile)

---

## Démarrage rapide

### 1. Cloner le dépôt

```bash
git clone https://github.com/morfredus/Base_ESP32_Multi.git
cd Base_ESP32_Multi
```

Ou téléchargez le fichier ZIP depuis GitHub et extrayez-le.

### 2. Ouvrir dans VS Code

```bash
code .
```

Ou ouvrez VS Code et utilisez **Fichier → Ouvrir le dossier** pour sélectionner le répertoire du projet.

### 3. Installer les dépendances

PlatformIO téléchargera automatiquement toutes les bibliothèques nécessaires lors de la première compilation. Aucune installation manuelle requise !

---

## Configuration initiale

### Étape 1 : Configurer les identifiants WiFi

**IMPORTANT :** Vous devez créer un fichier `secrets.h` avec vos identifiants WiFi.

1. Naviguez vers le dossier `include/`
2. Copiez `secrets_exemple_FR.h` pour créer `secrets.h` :

```bash
cp include/secrets_exemple_FR.h include/secrets.h
```

3. Éditez `include/secrets.h` avec vos informations WiFi :

```cpp
// Réseau 1 (Principal)
const char* WIFI_SSID1 = "VotreReseauMaison";    // ← Remplacez par votre nom WiFi
const char* WIFI_PASS1 = "VotreMotDePasse123";   // ← Remplacez par votre mot de passe

// Réseau 2 (Secondaire - Optionnel)
const char* WIFI_SSID2 = "ReseauBureau";         // ← Ou laissez vide ""
const char* WIFI_PASS2 = "MotDePasseBureau456";  // ← Ou laissez vide ""

// Réseaux 3 & 4 (Optionnels)
const char* WIFI_SSID3 = "";                     // Laissez vide si non nécessaire
const char* WIFI_PASS3 = "";
const char* WIFI_SSID4 = "";
const char* WIFI_PASS4 = "";
```

**📌 Note :** Le fichier `secrets.h` est dans `.gitignore` et ne sera jamais versionné (pour la sécurité).

Pour une configuration WiFi détaillée, consultez [WIFI_SETUP_FR.md](WIFI_SETUP_FR.md).

### Étape 2 : Sélectionner la configuration de votre carte

Le projet supporte plusieurs cartes ESP32. Éditez `platformio.ini` et définissez l'environnement par défaut :

```ini
[platformio]
default_envs = esp32s3_n16r8    ; ← Choisissez votre carte ici
```

**Environnements disponibles :**
- `esp32s3_n16r8` - ESP32-S3 avec 16MB Flash + 8MB PSRAM (recommandé)
- `esp32s3_n8r8` - ESP32-S3 avec 8MB Flash + 8MB PSRAM
- `esp32devkitc` - ESP32 Classic (4MB Flash, pas de PSRAM)

### Étape 3 : Configurer les fonctionnalités matérielles

Éditez `include/board_config.h` pour activer/désactiver les fonctionnalités :

```cpp
// Activez les fonctionnalités présentes sur votre carte
#define HAS_LED_BUILTIN    // LED intégrée
#define HAS_LED_RGB        // LED RGB (si disponible)
#define HAS_OLED           // Écran OLED SSD1306 (si disponible)
#define HAS_ST7789         // Écran TFT ST7789 (si disponible)
#define HAS_NEOPIXEL       // Ruban NeoPixel/WS2812B (si disponible)
// etc.
```

Pour une configuration matérielle détaillée, consultez [HARDWARE_SETUP_FR.md](HARDWARE_SETUP_FR.md).

---

## Compilation et téléversement

### Utiliser l'interface PlatformIO (Recommandé pour débutants)

1. **Ouvrir le panneau PlatformIO :**
   - Cliquez sur l'icône PlatformIO (tête d'alien) dans la barre latérale VS Code
   - Ou utilisez : **Affichage → Palette de commandes → PlatformIO: Home**

2. **Compiler le projet :**
   - Cliquez sur **Build** dans la barre d'outils PlatformIO (icône coche)
   - Ou utilisez le panneau Project Tasks : **General → Build**

3. **Téléverser vers l'ESP32 :**
   - Connectez votre ESP32 via USB
   - Cliquez sur **Upload** dans la barre d'outils PlatformIO (icône flèche)
   - Ou utilisez : **General → Upload**

4. **Monitorer la sortie série :**
   - Cliquez sur **Monitor** dans la barre d'outils PlatformIO (icône prise)
   - Ou utilisez : **General → Monitor**
   - Débit : 115200

### Utiliser la ligne de commande

```bash
# Compiler le projet
pio run

# Téléverser vers l'ESP32
pio run --target upload

# Ouvrir le moniteur série
pio device monitor

# Build + Upload + Monitor (tout en un)
pio run --target upload && pio device monitor
```

---

## Vérification de l'installation

### 1. Vérifier la sortie série

Après le téléversement, ouvrez le moniteur série (115200 baud). Vous devriez voir :

```
=== Base_ESP32_Multi v0.9.0 ===
Chip ID: 0xABCDEF
Flash: 16MB @ 80MHz

--- Initialisation des écrans ---
✓ OLED initialisé
✓ ST7789 initialisé

--- Démarrage WiFiMulti ---
  - Réseau ajouté : VotreReseauMaison
Connexion WiFi...
✓ Connecté à : VotreReseauMaison
Adresse IP : 192.168.1.100

Serveur web démarré sur http://192.168.1.100
  - Page OTA : http://192.168.1.100/ota

=== Configuration terminée ===
```

### 2. Vérifier la LED intégrée

La LED intégrée devrait clignoter (motif heartbeat) chaque seconde, indiquant que l'ESP32 fonctionne.

### 3. Tester l'interface web

1. Ouvrez un navigateur web
2. Allez à l'adresse IP affichée dans le moniteur série (ex : `http://192.168.1.100`)
3. Vous devriez voir le tableau de bord avec les informations système

### 4. Tester les boutons (si disponibles)

- **Bouton BOOT (appui long 2s) :** Déclenche un redémarrage avec barre de progression
- **Bouton 1 :** Fait défiler les couleurs LED RGB (si disponible)
- **Bouton 2 :** Émet un bip du buzzer (si disponible)

---

## Prochaines étapes

### Explorer l'interface web

- **Tableau de bord :** Voir les infos système (CPU, RAM, Flash, WiFi, etc.)
- **Mise à jour OTA :** Téléverser un nouveau firmware sans fil
- **Redémarrage :** Redémarrer l'ESP32 à distance

### Personnaliser votre projet

1. **Modifier main.cpp :** Ajoutez votre logique personnalisée dans `loop()`
2. **Créer de nouveaux modules :** Ajoutez des fichiers dans `src/` et `include/`
3. **Ajouter des bibliothèques :** Éditez `platformio.ini` sous `lib_deps`

### Apprendre les fonctionnalités avancées

- [Guide Configuration WiFi](WIFI_SETUP_FR.md) - Configuration multi-réseaux
- [Guide Mise à jour OTA](OTA_UPDATE_FR.md) - Mises à jour firmware sans fil
- [Guide Configuration Matériel](HARDWARE_SETUP_FR.md) - Configuration GPIO et périphériques

---

## Dépannage

### Problème : "secrets.h: No such file or directory"

**Solution :** Vous avez oublié de créer `secrets.h`. Copiez l'exemple :
```bash
cp include/secrets_exemple_FR.h include/secrets.h
```
Puis éditez-le avec vos identifiants WiFi.

---

### Problème : "Échec de connexion WiFi"

**Solutions :**
1. Vérifiez vos identifiants WiFi dans `secrets.h`
2. Assurez-vous que votre réseau WiFi est en 2.4GHz (l'ESP32 ne supporte pas le 5GHz)
3. Vérifiez que le SSID et le mot de passe sont corrects (sensible à la casse)
4. Essayez de rapprocher l'ESP32 de votre routeur

---

### Problème : "Échec du téléversement" ou "Port introuvable"

**Solutions :**
1. Vérifiez la connexion du câble USB
2. Installez les pilotes USB CP210x ou CH340 pour votre carte
3. Essayez un autre câble USB (certains câbles sont uniquement pour la charge)
4. Sélectionnez le bon port COM dans PlatformIO
5. Maintenez le bouton BOOT enfoncé pendant le téléversement (si l'auto-reset échoue)

---

### Problème : Le moniteur série affiche du texte illisible

**Solution :** Assurez-vous que le débit est réglé sur **115200** dans le moniteur série.

---

### Problème : La carte ne répond pas après le téléversement

**Solutions :**
1. Appuyez sur le bouton **RESET** de l'ESP32
2. Déconnectez et reconnectez le câble USB
3. Retéléversez le firmware

---

### Problème : Erreurs de compilation

**Causes courantes :**
1. Fichier `secrets.h` manquant
2. Mauvaise carte sélectionnée dans `platformio.ini`
3. Cache de bibliothèques corrompu - essayez : `pio run --target clean`

---

## Obtenir de l'aide

- **Issues GitHub :** https://github.com/morfredus/Base_ESP32_Multi/issues
- **Documentation :** Consultez le dossier `docs/` pour des guides détaillés
- **Communauté PlatformIO :** https://community.platformio.org/

---

## Et maintenant ?

- ✅ Vous avez configuré Base_ESP32_Multi avec succès !
- 🔧 Personnalisez les fonctionnalités matérielles dans `board_config.h`
- 🌐 Configurez plusieurs réseaux WiFi (voir [WIFI_SETUP_FR.md](WIFI_SETUP_FR.md))
- 📡 Apprenez les mises à jour OTA (voir [OTA_UPDATE_FR.md](OTA_UPDATE_FR.md))
- 🚀 Commencez à créer vos propres fonctionnalités !

Bon codage ! 🎉
