# Guide de Configuration Matérielle

**Version :** 0.9.0
**Dernière mise à jour :** 2026-01-03

Ce guide explique comment configurer et câbler les périphériques matériels pour Base_ESP32_Multi.

---

## 📋 Table des matières

1. [Cartes supportées](#cartes-supportées)
2. [Configuration rapide](#configuration-rapide)
3. [Brochages GPIO](#brochages-gpio)
4. [Guides de câblage](#guides-de-câblage)
5. [Activer/Désactiver les fonctionnalités](#activerdésactiver-les-fonctionnalités)
6. [Sécurité et bonnes pratiques](#sécurité-et-bonnes-pratiques)

---

## Cartes supportées

### ESP32-S3 DevKitC-1 (Recommandé)

**Variantes :**
- **N16R8** - 16MB Flash + 8MB PSRAM (meilleures performances)
- **N8R8** - 8MB Flash + 8MB PSRAM

**Caractéristiques :**
- Connecteur USB-C
- USB-to-Serial intégré
- Plus de broches GPIO disponibles
- Support PSRAM (RAM externe rapide)
- Processeur dual-core 240MHz

**Environnement PlatformIO :**
```ini
[platformio]
default_envs = esp32s3_n16r8
```

### ESP32 DevKitC (Classic)

**Variante :** 4MB Flash, pas de PSRAM

**Environnement PlatformIO :**
```ini
[platformio]
default_envs = esp32devkitc
```

---

## Configuration rapide

### Étape 1 : Éditer board_config.h

Ouvrez `include/board_config.h` et activez/désactivez les fonctionnalités selon votre matériel :

```cpp
// ==========================================================
// FLAGS DE FONCTIONNALITÉS MATÉRIELLES
// ==========================================================

// Activez uniquement les fonctionnalités présentes sur votre carte

// --- Écrans ---
#define HAS_OLED           // Écran OLED SSD1306 via I2C
#define HAS_ST7789         // Écran TFT ST7789 via SPI

// --- LEDs ---
#define HAS_LED_BUILTIN    // LED intégrée (toujours disponible)
#define HAS_LED_RGB        // LED RGB (3 LEDs séparées)
#define HAS_NEOPIXEL       // Ruban LED NeoPixel/WS2812B
//#define HAS_NEOPIXEL_MATRIX  // Matrice NeoPixel 8x8

// --- Capteurs ---
//#define HAS_DHT          // Capteur température DHT11/DHT22
//#define HAS_PIR          // Capteur de mouvement PIR
//#define HAS_ULTRASONIC   // Capteur distance HC-SR04

// --- Périphériques ---
//#define HAS_BUZZER       // Buzzer piézo
```

**Important :** Décommentez (retirez `//`) pour activer, ajoutez `//` pour désactiver.

---

## Brochages GPIO

### ESP32-S3 DevKitC-1

#### Écrans

**OLED SSD1306 (I2C) :**
| Signal | GPIO | Description |
|--------|------|-------------|
| SDA | 15 | I2C Data (pull-up 4.7kΩ vers 3.3V requis) |
| SCL | 16 | I2C Clock (pull-up 4.7kΩ vers 3.3V requis) |
| VCC | 3.3V | Alimentation |
| GND | GND | Masse |

**ST7789/ILI9341 TFT (SPI) :**
| Signal | GPIO | Alias | Description |
|--------|------|-------|-------------|
| MOSI | 11 | DIN/SDA | Master Out Slave In |
| SCLK | 12 | SCK/CLK | Horloge SPI |
| CS | 10 | TCS | Chip Select |
| DC | 9 | A0/RS | Data/Command |
| RST | 14 | RESET | Reset (actif bas) |
| BL | 7 | LED | Rétroéclairage (PWM possible) |

---

#### LEDs

**LED RGB (3 LEDs séparées) :**
| LED | GPIO | Résistance | Description |
|-----|------|------------|-------------|
| Rouge | 21 | 220-470Ω | LED rouge avec résistance série |
| Verte | 41 | 220-470Ω | LED verte avec résistance série |
| Bleue | 42 | 220-470Ω | LED bleue avec résistance série |

**Ruban NeoPixel/WS2812B :**
| Signal | GPIO | Description |
|--------|------|-------------|
| DIN | 48 | Entrée données (signal 3.3V, LED alimentée 5V) |
| VCC | 5V | Alimentation (source externe recommandée) |
| GND | GND | Masse commune avec ESP32 |

**Matrice NeoPixel 8×8 (WS2812B-64) :**
| Signal | GPIO | Description |
|--------|------|-------------|
| DIN | 3 | Entrée données matrice |
| VCC | 5V | Alimentation (3A minimum pleine luminosité) |
| GND | GND | Masse commune |

**⚠️ Avertissement alimentation :** 64 LEDs en blanc pleine luminosité consomment ~3.8A. Utilisez une alimentation 5V externe de minimum 3-4A. Connectez GND ESP32 à GND alimentation !

---

#### Boutons

| Bouton | GPIO | Type | Pull-up | Description |
|--------|------|------|---------|-------------|
| BOOT | 0 | Intégré | Externe | Broche de strapping, actif BAS |
| Bouton 1 | 38 | Externe | Interne | Bouton utilisateur 1 |
| Bouton 2 | 39 | Externe | Interne | Bouton utilisateur 2 |

**Câblage (actif BAS) :**
```
Bouton → Broche GPIO
Bouton → GND
```

Pull-up interne activé dans le code via `pinMode(BUTTON_X, INPUT_PULLUP)`.

---

#### Capteurs

**DHT11/DHT22 Température :**
| Signal | GPIO | Pull-up | Description |
|--------|------|---------|-------------|
| DATA | 5 | 10kΩ vers 3.3V | Ligne données |
| VCC | 3.3V | - | Alimentation |
| GND | GND | - | Masse |

**Capteur mouvement PIR :**
| Signal | GPIO | Description |
|--------|------|-------------|
| OUT | 46 | Sortie digitale (HIGH si mouvement détecté) |
| VCC | 5V | Alimentation (vérifiez votre module PIR) |
| GND | GND | Masse |

**⚠️ Avertissement tension :** Si le PIR sort du 5V, utilisez un diviseur de tension (10kΩ + 20kΩ) pour ramener à 3.3V !

**HC-SR04 Distance ultrasonique :**
| Signal | GPIO | Diviseur tension | Description |
|--------|------|------------------|-------------|
| TRIG | 2 | Non | Trigger (sortie 3.3V OK) |
| ECHO | 35 | **Oui** | Echo (sort 5V → nécessite diviseur !) |
| VCC | 5V | - | Alimentation |
| GND | GND | - | Masse |

**Diviseur tension ECHO :**
```
HC-SR04 ECHO → 10kΩ → GPIO 35
                 ↓
               20kΩ → GND

Vout = 5V × (20k / 30k) = 3.3V ✓
```

**Capteur lumière LDR :**
| Composant | Connexion | Description |
|-----------|-----------|-------------|
| LDR | 3.3V → LDR → GPIO 4 | Photorésistance |
| Résistance | GPIO 4 → 10kΩ → GND | Résistance pull-down |

**Lecture :** Valeur analogique via `analogRead(LIGHT_SENSOR)` (0-4095).

---

## Guides de câblage

### Câbler un écran OLED (SSD1306)

**Matériel nécessaire :**
- Module OLED 128×64 SSD1306 I2C
- 4 fils de connexion
- 2× résistances 4.7kΩ (pull-ups, si pas sur module OLED)

**Étapes :**

1. **Activer dans le code :**
   ```cpp
   // include/board_config.h
   #define HAS_OLED
   ```

2. **Câblage (ESP32-S3) :**
   ```
   OLED VCC → ESP32 3.3V
   OLED GND → ESP32 GND
   OLED SDA → ESP32 GPIO 15
   OLED SCL → ESP32 GPIO 16
   ```

3. **Ajouter pull-ups (si nécessaire) :**
   - Certains modules OLED ont des pull-ups intégrés
   - Si l'écran ne fonctionne pas, ajoutez résistance 4.7kΩ : SDA → 3.3V
   - Et résistance 4.7kΩ : SCL → 3.3V

4. **Téléverser et tester**

5. **Vérifier moniteur série :**
   ```
   --- Initialisation des écrans ---
   ✓ OLED initialisé
   ```

**Dépannage :**
- **Pas d'affichage :** Vérifiez adresse I2C (défaut : 0x3C)
- **Affichage brouillé :** Vérifiez OLED_WIDTH et OLED_HEIGHT dans `config.h`
- **Pas de réponse :** Vérifiez pull-ups, câblage, et alimentation 3.3V

---

### Câbler un écran TFT (ST7789)

**Matériel nécessaire :**
- Écran TFT ST7789 240×240 (ou ILI9341 240×320)
- 8 fils de connexion

**Étapes :**

1. **Activer dans le code :**
   ```cpp
   // include/board_config.h
   #define HAS_ST7789
   ```

2. **Câblage (ESP32-S3) :**
   ```
   TFT VCC → ESP32 3.3V (ou 5V, vérifiez module)
   TFT GND → ESP32 GND
   TFT MOSI (DIN) → ESP32 GPIO 11
   TFT SCK (CLK) → ESP32 GPIO 12
   TFT CS → ESP32 GPIO 10
   TFT DC (RS/A0) → ESP32 GPIO 9
   TFT RST → ESP32 GPIO 14
   TFT BL (LED) → ESP32 GPIO 7
   ```

3. **Configurer taille écran :**
   ```cpp
   // include/config.h
   #define TFT_WIDTH  240
   #define TFT_HEIGHT 240   // Ou 320 pour ILI9341
   #define TFT_ROTATION 2   // 0-3 pour différentes orientations
   ```

4. **Téléverser et tester**

---

### Câbler un ruban LED NeoPixel

**Matériel nécessaire :**
- Ruban LED WS2812B
- Alimentation externe 5V (1A par 20 LEDs recommandé)
- 1× résistance 470Ω (optionnel mais recommandé pour ligne données)
- 1× condensateur 1000µF (optionnel mais recommandé)

**Étapes :**

1. **Activer dans le code :**
   ```cpp
   // include/board_config.h
   #define HAS_NEOPIXEL
   #define NEOPIXEL_NUM 30  // Nombre de LEDs
   ```

2. **Câblage (ESP32-S3) :**
   ```
   ESP32 GPIO 48 → (résistance 470Ω) → Ruban LED DIN
   ESP32 GND → GND Alimentation → Ruban LED GND
   Alimentation 5V → Ruban LED VCC
   (Condensateur 1000µF entre 5V et GND près des LEDs)
   ```

**⚠️ Critique :** Toujours connecter GND ESP32 au GND alimentation !

3. **Calcul puissance :**
   - Chaque LED consomme ~60mA en blanc pleine luminosité
   - 30 LEDs × 60mA = 1.8A → utilisez alimentation 2A+
   - Alimentation USB ESP32 (500mA) n'est PAS suffisante pour >8 LEDs

**Dépannage :**
- **LEDs ne s'allument pas :** Vérifiez alimentation 5V, connexion GND
- **Première LED fonctionne, pas les autres :** Ligne données cassée
- **Couleurs aléatoires :** Alimentation insuffisante ou GND non connecté

---

### Câbler matrice NeoPixel 8×8

**Nouveau dans v0.9.0 !**

**Matériel :**
- Matrice WS2812B 8×8 (64 LEDs)
- Alimentation externe 5V 3-4A

**Étapes :**

1. **Activer :**
   ```cpp
   #define HAS_NEOPIXEL_MATRIX
   ```

2. **Câblage (ESP32-S3) :**
   ```
   ESP32 GPIO 3 → Matrice DIN
   ESP32 GND → GND Alim → Matrice GND
   Alim 5V → Matrice VCC
   ```

3. **Alimentation requise :**
   - 64 LEDs × 60mA = 3.84A en pleine luminosité !
   - **Alimentation 3A minimum requise**

---

## Activer/Désactiver les fonctionnalités

### Méthode : Utiliser #define (Recommandé)

Éditez `include/board_config.h` :

```cpp
// Activer fonctionnalité (décommenté)
#define HAS_OLED

// Désactiver fonctionnalité (commenté)
//#define HAS_DHT
```

Le firmware compile automatiquement uniquement le code des fonctionnalités activées, réduisant l'utilisation de la mémoire flash.

---

## Sécurité et bonnes pratiques

### Niveaux de tension

**⚠️ CRITIQUE :** Les broches GPIO ESP32 sont **3.3V UNIQUEMENT**. Ne jamais appliquer 5V directement !

**Sûr :**
- Capteurs 3.3V → GPIO ESP32 ✓
- GPIO ESP32 → Entrées appareil 5V (généralement OK) ✓

**DANGEREUX :**
- Sortie capteur 5V → GPIO ESP32 ❌ (utilisez diviseur de tension !)
- Relais 12V → GPIO ESP32 ❌ (utilisez transistor + optocoupleur !)

### Formule diviseur de tension

Pour convertir 5V en 3.3V :

```
       5V
        │
       [R1 = 10kΩ]
        │
        ├─────→ GPIO ESP32 (3.3V)
        │
       [R2 = 20kΩ]
        │
       GND

Vout = Vin × (R2 / (R1 + R2))
     = 5V × (20kΩ / 30kΩ)
     = 3.3V ✓
```

### Limites de courant

**Limites courant GPIO ESP32 :**
- Par broche : 12mA (max absolu 40mA)
- Total toutes broches : ~200mA

**Solutions pour appareils haute consommation :**
- **LEDs :** Résistance série 220-470Ω
- **Buzzer :** Transistor (NPN 2N2222)
- **Relais :** Transistor + diode flyback
- **Moteur :** Driver moteur (L298N, TB6612)

### Résistances Pull-up/Pull-down

**I2C (SDA/SCL) :**
- **Requis :** 4.7kΩ pull-up vers 3.3V
- Souvent intégré aux modules, ajoutez si manquant

**Boutons :**
- **Pull-ups internes disponibles** via mode `INPUT_PULLUP`
- Externe 10kΩ pour fiabilité

### Guidelines alimentation

**Alimentation USB (500mA) :**
- ESP32 seul : ✓
- ESP32 + OLED : ✓
- ESP32 + TFT : ✓
- ESP32 + 8 NeoPixels : ✓
- ESP32 + 30 NeoPixels : ❌ (nécessite alimentation externe)

**Alimentation externe 5V (2A+) :**
- Requis pour rubans NeoPixel (>8 LEDs)
- Requis pour moteurs, relais
- Connectez GND ESP32 au GND alimentation !

---

## Résumé

- ✅ Éditez `board_config.h` pour activer/désactiver les fonctionnalités
- ✅ Suivez les tableaux GPIO pour votre carte (ESP32-S3 vs Classic)
- ✅ Utilisez toujours logique 3.3V ou diviseurs pour capteurs 5V
- ✅ Ajoutez résistances série pour LEDs
- ✅ Utilisez alimentation externe pour NeoPixels, moteurs, relais
- ✅ Connectez toutes les masses ensemble (ESP32, alimentation, périphériques)
- ✅ Ajoutez résistances pull-up pour I2C (4.7kΩ)

Bon montage ! 🔧
