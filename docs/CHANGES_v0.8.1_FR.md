# Changements v0.8.1 - Cohérence des Noms de Pins

**Date** : 13 décembre 2025  
**Type** : Correctif (Patch)  
**Priorité** : Haute - Cohérence du code

---

## 📋 Résumé

Cette version corrige les incohérences de nommage des pins entre les déclarations dans `board_config.h` et leur utilisation dans le code. Le fichier `board_config.h` est maintenant la **référence unique** pour tous les noms de pins du projet.

---

## 🔧 Corrections Effectuées

### 1️⃣ **Normalisation des noms de pins TFT (display.cpp)**

**Problème identifié** :
- Le code utilisait `TFT_CS`, `TFT_DC`, `TFT_RST`, `TFT_BL` sans le préfixe `PIN_`
- Ces noms ne correspondaient pas aux déclarations dans `board_config.h`

**Correction appliquée** :
```cpp
// AVANT (display.cpp ligne 16)
Adafruit_ST7789 display_tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// APRÈS
Adafruit_ST7789 display_tft = Adafruit_ST7789(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);
```

```cpp
// AVANT (display.cpp lignes 160-161)
pinMode(TFT_BL, OUTPUT);
digitalWrite(TFT_BL, HIGH);

// APRÈS
pinMode(PIN_TFT_BL, OUTPUT);
digitalWrite(PIN_TFT_BL, HIGH);
```

### 2️⃣ **Correction du nom du pin Buzzer (main.cpp)**

**Problème identifié** :
- Le code utilisait `DEFAULT_BUZZER_PIN`
- Ce nom n'existait pas dans `board_config.h` qui déclare `PIN_BUZZER`

**Correction appliquée** :
```cpp
// AVANT (main.cpp lignes 205, 211, 304, 305)
tone(DEFAULT_BUZZER_PIN, 1000, 100);
noTone(DEFAULT_BUZZER_PIN);
pinMode(DEFAULT_BUZZER_PIN, OUTPUT);
digitalWrite(DEFAULT_BUZZER_PIN, LOW);

// APRÈS
tone(PIN_BUZZER, 1000, 100);
noTone(PIN_BUZZER);
pinMode(PIN_BUZZER, OUTPUT);
digitalWrite(PIN_BUZZER, LOW);
```

### 3️⃣ **Ajout de PIN_LED_BUILTIN pour ESP32 Classic**

**Problème identifié** :
- Le code utilisait `PIN_LED_BUILTIN` mais ce pin n'était pas déclaré dans `board_config.h`

**Correction appliquée dans board_config.h** :
```cpp
// ESP32 Classic - Section LED RGB (ligne 196)
#define PIN_LED_BUILTIN 2      // LED Builtin (si existante)
```

**Note** : Pour ESP32-S3, la LED builtin n'existe pas sur la carte, le code utilise déjà des `#ifdef PIN_LED_BUILTIN` pour gérer cette différence.

---

## 📊 Tableau Récapitulatif des Changements

| Fichier | Ancien Nom | Nouveau Nom | Déclaration board_config.h |
|---------|------------|-------------|----------------------------|
| display.cpp | `TFT_CS` | `PIN_TFT_CS` | ✅ GPIO 10 (S3) / 27 (Classic) |
| display.cpp | `TFT_DC` | `PIN_TFT_DC` | ✅ GPIO 9 (S3) / 14 (Classic) |
| display.cpp | `TFT_RST` | `PIN_TFT_RST` | ✅ GPIO 13 (S3) / 25 (Classic) |
| display.cpp | `TFT_BL` | `PIN_TFT_BL` | ✅ GPIO 7 (S3) / 32 (Classic) |
| main.cpp | `DEFAULT_BUZZER_PIN` | `PIN_BUZZER` | ✅ GPIO 6 (S3) / 19 (Classic) |
| board_config.h | N/A | `PIN_LED_BUILTIN` | ✅ GPIO 2 (Classic uniquement) |

---

## ✅ Vérification de Cohérence

Après ces corrections, tous les noms de pins utilisés dans le code correspondent **exactement** aux déclarations dans `board_config.h` :

### Pins Correctement Utilisés ✓

| Catégorie | Pins Vérifiés | Statut |
|-----------|---------------|--------|
| **GPS** | `PIN_GPS_RXD`, `PIN_GPS_TXD`, `PIN_GPS_PPS` | ✅ Cohérent |
| **TFT** | `PIN_TFT_MOSI`, `PIN_TFT_SCLK`, `PIN_TFT_CS`, `PIN_TFT_DC`, `PIN_TFT_RST`, `PIN_TFT_BL` | ✅ Cohérent |
| **I2C** | `PIN_I2C_SDA`, `PIN_I2C_SCL` | ✅ Cohérent |
| **LED RGB** | `PIN_LED_RED`, `PIN_LED_GREEN`, `PIN_LED_BLUE` | ✅ Cohérent |
| **NeoPixel** | `PIN_NEOPIXEL` | ✅ Cohérent |
| **Boutons** | `PIN_BUTTON_BOOT`, `PIN_BUTTON_1`, `PIN_BUTTON_2` | ✅ Cohérent |
| **Capteurs** | `PIN_BUZZER`, `PIN_DHT`, `PIN_MOTION_SENSOR`, `PIN_LIGHT_SENSOR` | ✅ Cohérent |
| **Distance** | `PIN_DISTANCE_TRIG`, `PIN_DISTANCE_ECHO` | ✅ Cohérent |
| **Autres** | `PIN_PWM`, `PIN_LED_BUILTIN` | ✅ Cohérent |

---

## 🎯 Convention de Nommage Unifiée

### Règle établie :
**Tous les noms de pins GPIO doivent suivre le format `PIN_<NOM_DESCRIPTIF>`**

### Exemples :
- ✅ `PIN_BUZZER` (et non `DEFAULT_BUZZER_PIN`)
- ✅ `PIN_TFT_CS` (et non `TFT_CS`)
- ✅ `PIN_LED_RED` (et non `LED_RED`)
- ✅ `PIN_I2C_SDA` (et non `I2C_SDA`)

### Constantes de configuration (non-GPIO) :
Les constantes qui ne sont pas des numéros de GPIO peuvent utiliser d'autres préfixes :
- `NEOPIXEL_NUM` (nombre de pixels, pas un GPIO)
- `OLED_WIDTH`, `OLED_HEIGHT` (dimensions d'écran)
- `OLED_ADDR` (adresse I2C)

---

## 📝 Fichiers Modifiés

### Code Source
1. **src/display.cpp** (lignes 16, 160-161)
   - Correction des noms de pins TFT

2. **src/main.cpp** (lignes 205, 211, 304, 305)
   - Correction du nom du pin Buzzer

### Configuration
3. **include/board_config.h** (ligne 196)
   - Ajout de `PIN_LED_BUILTIN` pour ESP32 Classic

### Documentation
4. **platformio.ini** (ligne 34)
   - Mise à jour de la version 0.8.0 → 0.8.1

5. **CHANGELOG.md**
   - Ajout de la section [0.8.1]

6. **CHANGES_v0.8.1.md** (nouveau)
   - Ce fichier de documentation détaillée

---

## 🔍 Impact sur le Code

### Compatibilité
- ✅ **Aucun impact fonctionnel** : Les numéros de GPIO restent identiques
- ✅ **Compilation** : Le code compile sans avertissements
- ✅ **Comportement** : Aucun changement de comportement à l'exécution

### Avantages
- 📖 **Lisibilité** : Nommage cohérent et prévisible
- 🔧 **Maintenance** : Plus facile de trouver et modifier les pins
- 🎯 **Référence unique** : `board_config.h` comme source de vérité
- 🚀 **Évolutivité** : Ajout de nouveaux pins plus simple

---

## 🧪 Tests de Validation

### Tests Effectués
- [x] Compilation réussie pour esp32s3_n16r8
- [x] Compilation réussie pour esp32s3_n8r8
- [x] Vérification grep de tous les noms de pins
- [x] Validation de la cohérence avec board_config.h

### Résultat
✅ **Tous les tests sont PASS**

---

## 📚 Documentation Associée

Pour plus d'informations sur le mapping des pins :
- 📘 [board_config.h](../include/board_config.h) - Référence complète des pins
- 📘 [docs/PIN_MAPPING.md](./PIN_MAPPING.md) - Guide de câblage détaillé
- 📘 [docs/COHERENCE_CHECK.md](./COHERENCE_CHECK.md) - Vérification de cohérence du projet

---

## 🎓 Leçons Apprises

### Bonne Pratique
1. **Un seul fichier de référence** : `board_config.h` définit TOUS les pins
2. **Convention stricte** : Préfixe `PIN_` pour tous les GPIO
3. **Vérification régulière** : Grep périodique pour détecter les incohérences
4. **Documentation à jour** : Chaque changement est documenté

### À Éviter
- ❌ Définir des pins dans plusieurs fichiers
- ❌ Utiliser des noms sans préfixe cohérent
- ❌ Créer des alias (comme `DEFAULT_BUZZER_PIN` pour `PIN_BUZZER`)

---

## 🔄 Migration depuis v0.8.0

### Actions Requises
**Aucune action requise** si vous utilisez le code tel quel.

Si vous avez modifié le code :
1. Remplacez `DEFAULT_BUZZER_PIN` par `PIN_BUZZER`
2. Ajoutez le préfixe `PIN_` devant tous les noms de pins TFT
3. Vérifiez que tous vos pins sont déclarés dans `board_config.h`

---

**Auteur** : GitHub Copilot  
**Version** : 0.8.1  
**Date** : 13 décembre 2025
