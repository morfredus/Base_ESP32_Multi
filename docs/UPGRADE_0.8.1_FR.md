# Guide de Mise à Jour vers v0.8.1

**Date de publication** : 13 décembre 2025  
**Type de version** : Patch (Correctif)  
**Priorité** : Recommandée pour tous les utilisateurs

---

## 🎯 Objectif de cette Version

La version **0.8.1** est un correctif qui améliore la **cohérence et la maintenabilité** du code en normalisant tous les noms de pins GPIO. Aucun changement fonctionnel, mais une amélioration importante de la qualité du code.

---

## 📊 Résumé des Changements

### Corrections
- ✅ Normalisation complète des noms de pins avec le préfixe `PIN_`
- ✅ `board_config.h` établi comme référence unique
- ✅ Correction de 6 incohérences de nommage

### Impact
- 🔧 **Maintenabilité** : Code plus facile à comprendre et modifier
- 📖 **Lisibilité** : Convention de nommage cohérente
- 🎯 **Référence unique** : Plus de confusion sur les noms de pins

---

## 🚀 Migration depuis v0.8.0

### Si vous utilisez le code tel quel
✅ **Aucune action requise** - Recompilez simplement votre projet.

```bash
pio run -e esp32s3_n16r8
```

### Si vous avez modifié le code

#### 1️⃣ Remplacer les anciens noms de pins

```cpp
// Dans vos fichiers .cpp personnalisés, remplacez :

// Pins TFT
TFT_CS    → PIN_TFT_CS
TFT_DC    → PIN_TFT_DC
TFT_RST   → PIN_TFT_RST
TFT_BL    → PIN_TFT_BL

// Pin Buzzer
DEFAULT_BUZZER_PIN → PIN_BUZZER
```

#### 2️⃣ Vérifier vos déclarations

Si vous avez ajouté des pins personnalisés, assurez-vous qu'ils :
- Sont déclarés dans `board_config.h`
- Suivent la convention `PIN_<NOM_DESCRIPTIF>`
- Utilisent des valeurs GPIO différentes selon la cible (ESP32-S3 ou Classic)

Exemple :
```cpp
// Dans board_config.h
#if defined(TARGET_ESP32_S3)
    #define PIN_MON_CAPTEUR 14  // ESP32-S3
#elif defined(TARGET_ESP32_CLASSIC)
    #define PIN_MON_CAPTEUR 15  // ESP32 Classic
#endif
```

#### 3️⃣ Vérifier la compilation

```bash
# ESP32-S3
pio run -e esp32s3_n16r8

# ESP32 Classic (si applicable)
pio run -e esp32_classic
```

---

## 📋 Liste de Vérification

### Avant la mise à jour
- [ ] Sauvegardez votre code actuel
- [ ] Notez tous les pins personnalisés que vous avez ajoutés
- [ ] Vérifiez votre version actuelle (`PROJECT_VERSION` dans platformio.ini)

### Après la mise à jour
- [ ] Recompilez le projet
- [ ] Vérifiez qu'il n'y a pas d'erreurs de compilation
- [ ] Testez les fonctionnalités principales (WiFi, écrans, boutons)
- [ ] Vérifiez que vos pins personnalisés fonctionnent toujours

---

## 🔍 Détails des Changements

### Fichiers Modifiés

| Fichier | Lignes Modifiées | Description |
|---------|------------------|-------------|
| `src/display.cpp` | 16, 160-161 | Noms pins TFT normalisés |
| `src/main.cpp` | 205, 211, 304, 305 | Pin Buzzer normalisé |
| `include/board_config.h` | 196 | Ajout `PIN_LED_BUILTIN` |
| `platformio.ini` | 34 | Version 0.8.0 → 0.8.1 |
| `CHANGELOG.md` | 8-17 | Ajout section [0.8.1] |

### Nouveaux Fichiers

- `CHANGES_v0.8.1.md` - Documentation détaillée des changements
- `docs/UPGRADE_0.8.1.md` - Ce guide de mise à jour

---

## 🎓 Convention de Nommage

### Règle Générale
**Tous les GPIO doivent utiliser le préfixe `PIN_`**

### Exemples de Nommage Correct ✅

```cpp
// Capteurs
#define PIN_BUZZER         6
#define PIN_DHT            5
#define PIN_MOTION_SENSOR  46
#define PIN_LIGHT_SENSOR   4

// Affichage TFT
#define PIN_TFT_CS    10
#define PIN_TFT_DC    9
#define PIN_TFT_RST   13
#define PIN_TFT_BL    7

// Communication
#define PIN_I2C_SDA   15
#define PIN_I2C_SCL   16
#define PIN_GPS_RXD   18
#define PIN_GPS_TXD   17

// LED et Boutons
#define PIN_LED_RED       21
#define PIN_LED_GREEN     41
#define PIN_LED_BLUE      42
#define PIN_BUTTON_1      38
#define PIN_BUTTON_2      39
```

### Exceptions (Non-GPIO) ✅

```cpp
// Configuration (pas des numéros de GPIO)
#define NEOPIXEL_NUM     1     // Nombre de pixels
#define OLED_WIDTH       128   // Largeur en pixels
#define OLED_HEIGHT      64    // Hauteur en pixels
#define OLED_ADDR        0x3C  // Adresse I2C
#define ST7789_WIDTH     240   // Largeur TFT
```

---

## 🛠️ Dépannage

### Erreur de compilation : "undeclared identifier"

**Symptôme** :
```
error: 'TFT_CS' was not declared in this scope
```

**Solution** :
Ajoutez le préfixe `PIN_` :
```cpp
// Changez
Adafruit_ST7789 tft(TFT_CS, TFT_DC, TFT_RST);

// En
Adafruit_ST7789 tft(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);
```

### Pin personnalisé non reconnu

**Symptôme** :
```
error: 'PIN_MON_CAPTEUR' was not declared
```

**Solution** :
1. Vérifiez que le pin est déclaré dans `board_config.h`
2. Assurez-vous qu'il est dans le bon bloc `#if defined(TARGET_ESP32_S3)`
3. Recompilez avec `pio run --target clean` puis `pio run`

### Conflit de GPIO

**Symptôme** :
Comportement erratique, redémarrages aléatoires

**Solution** :
Vérifiez dans `board_config.h` que :
- Aucun GPIO n'est utilisé deux fois
- Les GPIO réservés (0, 1, 3) ne sont pas utilisés pour autre chose
- Les GPIO input-only (34-39) ne sont pas configurés en OUTPUT

---

## 📚 Documentation Complémentaire

Pour en savoir plus :

- 📘 [CHANGES_v0.8.1.md](CHANGES_v0.8.1.md) - Détails techniques des changements
- 📘 [board_config.h](../include/board_config.h) - Référence complète des pins
- 📘 [COHERENCE_CHECK.md](./COHERENCE_CHECK.md) - Vérification de cohérence
- 📘 [PIN_MAPPING.md](./PIN_MAPPING.md) - Guide de câblage détaillé

---

## 💡 Conseil

> **Bonne pratique** : Toujours consulter `board_config.h` avant d'ajouter un nouveau pin.  
> Ce fichier est la **source de vérité** pour tous les GPIO du projet.

---

## ❓ Support

Si vous rencontrez des problèmes après la mise à jour :

1. **Vérifiez** que tous vos noms de pins utilisent le préfixe `PIN_`
2. **Recompilez** avec `pio run --target clean` puis `pio run`
3. **Consultez** [CHANGES_v0.8.1.md](CHANGES_v0.8.1.md) pour les détails

---

## 🎉 Versions Futures

La v0.8.1 établit une base solide pour les futures améliorations :

- 📍 Convention de nommage cohérente
- 🎯 Référence unique dans `board_config.h`
- 🔧 Code plus maintenable

Les prochaines versions pourront ajouter de nouvelles fonctionnalités en s'appuyant sur cette fondation propre.

---

**Bonne mise à jour !** 🚀

---

*Document créé le 13 décembre 2025*  
*Version du projet : 0.8.1*
