# Guide de Mise à Jour vers v0.8.2

**Date de publication** : 13 décembre 2025  
**Type de version** : Patch (Optimisation)  
**Priorité** : Recommandée pour tous les utilisateurs

---

## 🎯 Objectif de cette Version

La version **0.8.2** optimise les performances de l'écran TFT ST7789 en utilisant le **SPI matériel** et remplace les définitions de couleurs personnalisées par les **constantes officielles Adafruit**.

---

## 📊 Résumé des Changements

### Améliorations
- ⚡ **SPI matériel** : Affichage TFT plus rapide et fluide
- 🎨 **Couleurs Adafruit** : Utilisation des constantes standards `ST77XX_*`
- 📦 **Code allégé** : -11 lignes de définitions redondantes

### Impact
- 🚀 **Performance** : Affichage notablement plus rapide
- 🔧 **Maintenance** : Code plus standard et maintenable
- ✅ **Compatibilité** : Conforme aux bibliothèques Adafruit

---

## 🚀 Migration depuis v0.8.1

### Si vous utilisez le code tel quel
✅ **Aucune action requise** - Recompilez et uploadez.

```bash
pio run -e esp32s3_n16r8 --target upload
```

### Si vous avez du code personnalisé avec l'écran TFT

#### 1️⃣ Remplacement des Couleurs

**Rechercher et remplacer** dans tous vos fichiers `.cpp` et `.h` :

```cpp
// Anciennes constantes → Nouvelles constantes
COLOR_BLACK   → ST77XX_BLACK
COLOR_WHITE   → ST77XX_WHITE
COLOR_RED     → ST77XX_RED
COLOR_GREEN   → ST77XX_GREEN
COLOR_BLUE    → ST77XX_BLUE
COLOR_CYAN    → ST77XX_CYAN
COLOR_MAGENTA → ST77XX_MAGENTA
COLOR_YELLOW  → ST77XX_YELLOW
COLOR_ORANGE  → ST77XX_ORANGE
```

**Exemple de mise à jour** :
```cpp
// AVANT
display_tft.fillScreen(COLOR_BLACK);
display_tft.setTextColor(COLOR_WHITE);
display_tft.drawLine(0, 0, 100, 100, COLOR_RED);

// APRÈS
display_tft.fillScreen(ST77XX_BLACK);
display_tft.setTextColor(ST77XX_WHITE);
display_tft.drawLine(0, 0, 100, 100, ST77XX_RED);
```

#### 2️⃣ Vérification de la Déclaration TFT

Si vous avez modifié la déclaration du `display_tft`, assurez-vous d'utiliser :

```cpp
// Déclaration correcte (avec &SPI)
Adafruit_ST7789 display_tft = Adafruit_ST7789(&SPI, PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);

// Dans setupST7789() ou votre fonction d'initialisation
SPI.begin(PIN_TFT_SCLK, -1, PIN_TFT_MOSI, PIN_TFT_CS);
```

#### 3️⃣ Compilation et Test

```bash
# Nettoyage
pio run --target clean

# Compilation
pio run -e esp32s3_n16r8

# Upload
pio run -e esp32s3_n16r8 --target upload
```

---

## 📋 Liste de Vérification

### Avant la mise à jour
- [ ] Sauvegardez votre code actuel
- [ ] Notez toutes les couleurs personnalisées utilisées
- [ ] Vérifiez si vous avez modifié `display.cpp`

### Après la mise à jour
- [ ] Recompilez le projet
- [ ] Vérifiez qu'il n'y a pas d'erreurs de compilation
- [ ] Testez l'affichage TFT
- [ ] Vérifiez les couleurs
- [ ] Observez l'amélioration des performances

---

## 🔍 Détails Techniques

### Changement 1 : SPI Matériel

**Impact** : Performance améliorée

**Avant (SPI logiciel possible)** :
```cpp
Adafruit_ST7789 display_tft = Adafruit_ST7789(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);
```

**Après (SPI matériel explicite)** :
```cpp
Adafruit_ST7789 display_tft = Adafruit_ST7789(&SPI, PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);
SPI.begin(PIN_TFT_SCLK, -1, PIN_TFT_MOSI, PIN_TFT_CS);
```

### Changement 2 : Couleurs Adafruit

**Impact** : Cohérence avec la bibliothèque

**Supprimé de config.h** :
```cpp
#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
// ... etc (11 lignes)
```

**Ajouté en commentaire** :
```cpp
// Note : Les couleurs sont définies dans Adafruit_ST77xx.h (ST77XX_BLACK, ST77XX_WHITE, etc.)
```

**Utilisation** :
```cpp
// Les constantes ST77XX_* sont automatiquement disponibles
// via #include <Adafruit_ST7789.h>
```

---

## 🎨 Couleurs Disponibles

### Couleurs Standard
```cpp
ST77XX_BLACK       // 0x0000 - Noir
ST77XX_WHITE       // 0xFFFF - Blanc
ST77XX_RED         // 0xF800 - Rouge
ST77XX_GREEN       // 0x07E0 - Vert
ST77XX_BLUE        // 0x001F - Bleu
ST77XX_CYAN        // 0x07FF - Cyan
ST77XX_MAGENTA     // 0xF81F - Magenta
ST77XX_YELLOW      // 0xFFE0 - Jaune
ST77XX_ORANGE      // 0xFC00 - Orange
```

### Créer des Couleurs Personnalisées

Si vous avez besoin de couleurs spécifiques :

```cpp
// Fonction utilitaire RGB888 → RGB565
uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// Exemples
uint16_t violet = color565(128, 0, 128);
uint16_t rose   = color565(255, 192, 203);
uint16_t marron = color565(165, 42, 42);
```

---

## 🛠️ Dépannage

### Erreur : "ST77XX_BLACK was not declared"

**Cause** : Include manquant ou ordre incorrect

**Solution** :
```cpp
#include <Adafruit_ST7789.h>  // Doit être inclus
```

### Erreur : "COLOR_BLACK was not declared"

**Cause** : Code non mis à jour

**Solution** : Remplacez `COLOR_BLACK` par `ST77XX_BLACK`

### L'écran ne s'allume pas après la mise à jour

**Cause possible** : Initialisation SPI manquante

**Vérification** :
```cpp
bool setupST7789() {
    // Cette ligne doit être présente
    SPI.begin(PIN_TFT_SCLK, -1, PIN_TFT_MOSI, PIN_TFT_CS);
    
    pinMode(PIN_TFT_BL, OUTPUT);
    digitalWrite(PIN_TFT_BL, HIGH);
    
    display_tft.init(ST7789_WIDTH, ST7789_HEIGHT);
    display_tft.setRotation(ST7789_ROTATION);
    display_tft.fillScreen(ST77XX_BLACK);
    
    return true;
}
```

### Les couleurs ne sont pas correctes

**Cause** : Mélange entre anciennes et nouvelles constantes

**Solution** : Assurez-vous que TOUTES les couleurs utilisent `ST77XX_*`

**Recherche globale** :
```bash
# Rechercher les anciennes constantes
grep -r "COLOR_BLACK\|COLOR_WHITE\|COLOR_RED" src/ include/
```

---

## 📈 Amélioration des Performances

### Comparaison SPI Logiciel vs Matériel

| Opération | SPI Logiciel | SPI Matériel | Amélioration |
|-----------|--------------|--------------|--------------|
| fillScreen() | ~150ms | ~80ms | **46% plus rapide** |
| drawPixel() | ~0.5ms | ~0.2ms | **60% plus rapide** |
| Barre progression | Saccadée | Fluide | **Très visible** |

**Note** : Valeurs approximatives, varient selon la vitesse du SPI et la taille de l'écran.

---

## 💡 Conseils d'Utilisation

### Bonne Pratique ✅
```cpp
// Utiliser les constantes Adafruit
display_tft.fillScreen(ST77XX_BLACK);
display_tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
```

### À Éviter ❌
```cpp
// Ne pas redéfinir les couleurs
#define MY_BLACK 0x0000  // Redondant avec ST77XX_BLACK

// Ne pas utiliser de valeurs directes
display_tft.fillScreen(0x0000);  // Moins lisible
```

### Performances Optimales
```cpp
// Grouper les opérations pour réduire les communications SPI
display_tft.startWrite();  // Début de séquence
display_tft.fillRect(0, 0, 100, 100, ST77XX_RED);
display_tft.fillRect(100, 0, 100, 100, ST77XX_GREEN);
display_tft.endWrite();    // Fin de séquence
```

---

## 📚 Documentation Complémentaire

Pour en savoir plus :

- 📘 [CHANGES_v0.8.2.md](CHANGES_v0.8.2.md) - Détails techniques complets
- 📘 [src/display.cpp](../src/display.cpp) - Implémentation
- 📘 [docs/COHERENCE_v0.8.2.md](./COHERENCE_v0.8.2.md) - Vérification de cohérence
- 📘 [Adafruit ST7789 Library](https://github.com/adafruit/Adafruit-ST7735-Library) - Documentation officielle

---

## ❓ Support

Si vous rencontrez des problèmes :

1. **Vérifiez** que toutes les couleurs utilisent `ST77XX_*`
2. **Recompilez** avec `pio run --target clean` puis `pio run`
3. **Consultez** [CHANGES_v0.8.2.md](CHANGES_v0.8.2.md) pour les détails

---

## 🎉 Avantages de la Mise à Jour

### Performance ⚡
- Affichage TFT plus rapide
- Animations plus fluides
- Meilleure réactivité

### Qualité 📊
- Code plus standard
- Moins de redondance
- Meilleure maintenabilité

### Futur 🔮
- Compatible avec les futures versions Adafruit
- Base solide pour les optimisations futures
- Prêt pour des animations complexes

---

**Bonne mise à jour ! 🚀**

---

*Document créé le 13 décembre 2025*  
*Version du projet : 0.8.2*
