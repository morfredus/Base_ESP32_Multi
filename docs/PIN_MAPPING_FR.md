# Guide de Connexion des Pins - Base ESP32-S3

> 📌 **Guide débutant** : Ce document explique comment connecter physiquement les composants à votre carte ESP32-S3 ou ESP32 Classic.

**[English Version](PIN_MAPPING.md)**

## 🎯 Table des matières
- [ESP32-S3 DevKitC-1](#esp32-s3-devkitc-1)
- [ESP32 Classic DevKitC](#esp32-classic-devkitc)
- [Schémas de connexion détaillés](#schémas-de-connexion-détaillés)
- [Conseils pour débutants](#conseils-pour-débutants)

---

## ESP32-S3 DevKitC-1

### 📋 Tableau récapitulatif des pins

| Composant | Signal | Pin GPIO | Description | Notes |
|-----------|--------|----------|-------------|-------|
| **Bouton BOOT** | Button | GPIO 0 | Bouton intégré sur la carte | Déjà présent, pas besoin de brancher |
| **LED RGB (NeoPixel)** | Data | GPIO 48 | LED RGB adressable intégrée | Déjà présente sur la carte |
| **OLED SSD1306** | SDA | GPIO 15 | Données I2C | Connexion vers la broche SDA de l'OLED |
| **OLED SSD1306** | SCL | GPIO 16 | Horloge I2C | Connexion vers la broche SCL de l'OLED |
| **OLED SSD1306** | VCC | 3.3V | Alimentation | Broche 3V3 de l'ESP32 |
| **OLED SSD1306** | GND | GND | Masse | Broche GND de l'ESP32 |
| **TFT ST7789** | MOSI (SDA) | GPIO 11 | Données SPI | Broche SDA du ST7789 |
| **TFT ST7789** | SCLK (SCL) | GPIO 12 | Horloge SPI | Broche SCL du ST7789 |
| **TFT ST7789** | CS | GPIO 10 | Chip Select | Sélection du périphérique |
| **TFT ST7789** | DC | GPIO 9 | Data/Command | Indicateur données/commande |
| **TFT ST7789** | RST | GPIO 13 | Reset | Réinitialisation de l'écran |
| **TFT ST7789** | BL | GPIO 7 | Backlight | Rétroéclairage (LED) |
| **TFT ST7789** | VCC | 3.3V | Alimentation | Broche 3V3 de l'ESP32 |
| **TFT ST7789** | GND | GND | Masse | Broche GND de l'ESP32 |

### 🔌 Schéma de connexion OLED SSD1306 (I2C)

```
ESP32-S3           OLED SSD1306
┌─────────┐        ┌──────────┐
│         │        │          │
│ GPIO 15 ├───────►│ SDA      │
│ GPIO 16 ├───────►│ SCL      │
│    3V3  ├───────►│ VCC      │
│    GND  ├───────►│ GND      │
│         │        │          │
└─────────┘        └──────────┘
```

**Adresse I2C par défaut** : `0x3C` (parfois `0x3D`, à vérifier avec un scanner I2C)

### 🖥️ Schéma de connexion TFT ST7789 (SPI)

```
ESP32-S3           ST7789 TFT
┌─────────┐        ┌──────────┐
│         │        │          │
│ GPIO 11 ├───────►│ MOSI/SDA │ Données
│ GPIO 12 ├───────►│ SCLK/SCL │ Horloge
│ GPIO 10 ├───────►│ CS       │ Chip Select
│ GPIO  9 ├───────►│ DC       │ Data/Command
│ GPIO 13 ├───────►│ RST      │ Reset
│ GPIO  7 ├───────►│ BL       │ Backlight
│    3V3  ├───────►│ VCC      │ Alimentation
│    GND  ├───────►│ GND      │ Masse
│         │        │          │
└─────────┘        └──────────┘
```

---

## ESP32 Classic DevKitC

### 📋 Tableau récapitulatif des pins

| Composant | Signal | Pin GPIO | Description | Notes |
|-----------|--------|----------|-------------|-------|
| **Bouton BOOT** | Button | GPIO 0 | Bouton intégré sur la carte | Déjà présent, pas besoin de brancher |
| **LED Builtin** | LED | GPIO 2 | LED bleue intégrée | Déjà présente sur la carte |
| **OLED SSD1306** | SDA | GPIO 21 | Données I2C | Connexion vers la broche SDA de l'OLED |
| **OLED SSD1306** | SCL | GPIO 22 | Horloge I2C | Connexion vers la broche SCL de l'OLED |
| **OLED SSD1306** | VCC | 3.3V | Alimentation | Broche 3V3 de l'ESP32 |
| **OLED SSD1306** | GND | GND | Masse | Broche GND de l'ESP32 |
| **TFT ST7789** | SCLK (SCL) | GPIO 18 | Horloge SPI | Broche SCL de l'écran |
| **TFT ST7789** | MOSI (SDA) | GPIO 23 | Données SPI | Broche SDA de l'écran |
| **TFT ST7789** | CS | GPIO 27 | Chip Select | Sélection du périphérique |
| **TFT ST7789** | DC | GPIO 14 | Data/Command | Indicateur données/commande |
| **TFT ST7789** | RST | GPIO 25 | Reset | Réinitialisation de l'écran |
| **TFT ST7789** | BL | GPIO 32 | Backlight | Rétroéclairage (LED) |
| **TFT ILI9341** | VCC | 3.3V | Alimentation | Broche 3V3 de l'ESP32 |
| **TFT ILI9341** | GND | GND | Masse | Broche GND de l'ESP32 |

### 🔌 Schéma de connexion OLED SSD1306 (I2C)

```
ESP32 Classic      OLED SSD1306
┌─────────┐        ┌──────────┐
│         │        │          │
│ GPIO 21 ├───────►│ SDA      │
│ GPIO 22 ├───────►│ SCL      │
│    3V3  ├───────►│ VCC      │
│    GND  ├───────►│ GND      │
│         │        │          │
└─────────┘        └──────────┘
```

**Adresse I2C par défaut** : `0x3C` (parfois `0x3D`, à vérifier avec un scanner I2C)

---

## 🛠️ Schémas de connexion détaillés

### OLED SSD1306 - Configuration I2C

L'écran OLED utilise le protocole **I2C** qui nécessite seulement **2 fils de données** (SDA et SCL) plus l'alimentation :

**Avantages du I2C** :
- ✅ Moins de câblage (seulement 4 fils au total)
- ✅ Partage facile du bus (plusieurs appareils I2C sur les mêmes pins)
- ✅ Configuration simple

**⚠️ Points d'attention** :
1. **Vérifier l'adresse I2C** : Généralement `0x3C`, mais certains modules utilisent `0x3D`. Utiliser un scanner I2C si doute.
2. **Alimentation** : Bien respecter 3.3V (pas 5V sur ESP32).
3. **Résistances pull-up** : Souvent déjà présentes sur le module OLED, pas besoin d'ajouter.

### TFT ST7789 / ILI9341 - Configuration SPI

Les écrans TFT utilisent le protocole **SPI** qui nécessite plus de fils mais offre une vitesse supérieure :

**Avantages du SPI** :
- ✅ Très rapide (rafraîchissement fluide)
- ✅ Supporte les écrans couleur haute résolution
- ✅ Pas de conflit d'adresse

**⚠️ Points d'attention** :
1. **Alimentation** : Certains écrans acceptent le 5V, d'autres seulement le 3.3V. **Vérifiez la documentation de votre module !**
2. **Ordre des pins** : Les noms peuvent varier (MOSI/SDA, SCLK/SCL). Se référer au tableau ci-dessus.
3. **Backlight (BL)** : Si l'écran reste noir, vérifiez que le backlight est bien connecté et alimenté (HIGH sur la pin BL).
4. **Chip Select (CS)** : Doit être à LOW pour activer l'écran. Le code s'en charge automatiquement.

---

## 💡 Conseils pour débutants

### 🔍 Identifier les broches de votre ESP32

1. **Cherchez le schéma "pinout"** de votre carte exacte (Google : "ESP32-S3 DevKitC pinout")
2. **Les broches sont numérotées** : Comptez depuis le bord ou repérez les étiquettes sérigraphiées
3. **Utilisez un multimètre** : En mode continuité, testez quelle broche physique correspond au GPIO voulu

### 🔌 Comment brancher proprement

1. **Coupez l'alimentation** : Toujours débrancher le câble USB avant de connecter/déconnecter des composants
2. **Fils de bonne qualité** : Évitez les fils trop longs ou de mauvaise qualité (faux contacts)
3. **Breadboard recommandée** : Pour prototyper sans soudure
4. **Notez vos connexions** : Faites un schéma ou prenez une photo avant de tout débrancher

### ⚡ Sécurité électrique

- ⚠️ **Ne jamais connecter du 5V sur une pin GPIO** (sauf si explicitement compatible)
- ⚠️ **Toujours connecter GND à GND** (masse commune entre ESP32 et périphériques)
- ⚠️ **Vérifier l'alimentation des modules** : 3.3V ou 5V ? (voir datasheet du composant)

### 🐛 Dépannage rapide

| Problème | Cause probable | Solution |
|----------|----------------|----------|
| OLED ne s'allume pas | Mauvaise adresse I2C | Tester `0x3C` puis `0x3D` dans `config.h` |
| TFT reste noir | Backlight non alimenté | Vérifier connexion GPIO 7 (S3) ou GPIO 13 (Classic) |
| TFT affiche n'importe quoi | Mauvais câblage SPI | Revérifier MOSI, SCLK, CS, DC, RST |
| Rien ne fonctionne | Alimentation insuffisante | Utiliser un câble USB de qualité ou alimentation externe |
| Crash au démarrage | Court-circuit | Débrancher tout et rebrancher un composant à la fois |

### 📚 Ressources utiles

- **Scanner I2C** : Utilisez un sketch Arduino "I2C Scanner" pour détecter l'adresse de votre OLED
- **Datasheets** : Toujours consulter la documentation officielle de vos modules
- **Forums** : ESP32.com, Arduino Forum, Reddit r/esp32

---

## 🎓 Résumé pour la configuration dans le code

### Fichier `include/config.h`

Pour **activer** un écran, décommentez la ligne correspondante :

```cpp
// OLED
#define HAS_OLED          // Décommenter pour activer l'OLED SSD1306

// TFT ST7789 (ESP32-S3 seulement dans cette config)
#define HAS_ST7789        // Décommenter pour activer le ST7789
```

### Fichier `include/board_config.h`

Les numéros de GPIO sont **déjà configurés** dans ce fichier selon votre carte.
**Vous n'avez normalement pas besoin de les modifier** sauf si vous utilisez un câblage personnalisé.

---

## ✅ Liste de vérification avant le premier test

- [ ] J'ai identifié ma carte (ESP32-S3 ou ESP32 Classic)
- [ ] J'ai repéré les GPIO sur le pinout de ma carte
- [ ] J'ai vérifié l'alimentation de mes modules (3.3V ou 5V)
- [ ] J'ai connecté tous les fils selon le tableau ci-dessus
- [ ] J'ai vérifié les connexions GND (masse commune)
- [ ] J'ai activé les écrans dans `config.h`
- [ ] J'ai créé mon fichier `secrets.h` avec mes identifiants WiFi
- [ ] Mon câble USB est de bonne qualité et branché fermement

**Prêt à compiler et téléverser ! 🚀**

---

*Document créé pour la version 0.7.0 du projet Base_ESP32_S3*
