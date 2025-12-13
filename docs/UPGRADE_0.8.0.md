# 🚀 Guide de Mise à Jour - Version 0.8.0

**Date** : 13 Décembre 2025  
**Changement majeur** : Système de contrôle multi-boutons avancé avec gestion RGB et buzzer  
**Statut** : ✅ Prêt pour production

---

## 📋 Résumé des changements

Cette version introduit un **système de gestion avancée des boutons** avec retour visuel (LED RGB) et sonore (Buzzer), ainsi qu'une amélioration majeure de la gestion de la **NeoPixel pour les états WiFi et reboot**.

### 🎯 Objectifs atteints

✅ Gestion complète de 3 boutons indépendants (BOOT, BTN1, BTN2)  
✅ Cycle RGB intuitif (Rouge → Vert → Bleu → Blanc → Éteint)  
✅ Feedback sonore pour chaque action utilisateur  
✅ Reboot sécurisé avec barre de progression annulable  
✅ NeoPixel affichant tous les états (WiFi/Reboot)  
✅ Code robuste et testé  

---

## 📊 Statistiques v0.8.0

| Métrique | Avant (v0.7.0) | Après (v0.8.0) | Variation |
|----------|-----------------|-----------------|-----------|
| Lignes main.cpp | ~180 | ~360 | +100% |
| Boutons gérés | 1 (BOOT) | 3 (BOOT/1/2) | +200% |
| Variables globales | 2 | 4 | +100% |
| Callbacks | 1 | 5 | +400% |
| Flash utilisée | 25.2% | 25.3% | +0.1% |
| RAM utilisée | 14.1% | 14.1% | - |

---

## 🔧 Changements détaillés

### 1️⃣ Configuration (`include/config.h`)

**Avant** :
```cpp
// HAS_LED_RGB et HAS_NEOPIXEL non définis
```

**Après** :
```cpp
#define HAS_LED_RGB
#define HAS_NEOPIXEL
#define NEOPIXEL_NUM 1
```

**Impact** : Active la LED RGB et la NeoPixel  
**Fichier** : `include/config.h` (lignes 35-39)

---

### 2️⃣ Gestion des boutons (`src/main.cpp`)

**Nouveaux objets OneButton** :
```cpp
OneButton btn(PIN_BUTTON_BOOT, true);  // BOOT
OneButton btn1(PIN_BUTTON_1, true);    // RGB Cycle
OneButton btn2(PIN_BUTTON_2, true);    // Buzzer
```

**Nouvelles variables globales** :
```cpp
bool isRebooting = false;   // Flag reboot
int rgbState = 0;           // État cycle RGB
```

**Fichier** : `src/main.cpp` (lignes 15-31)

---

### 3️⃣ Bouton BOOT - Reboot avec barre

**Comportement** :
```
Appui < 1s    : Rien
Appui 1-2s    : Barre progression 2s + Violet NeoPixel
└─ Avant 100% : Annulation + Écran WiFi restauré
└─ À 100%     : Redémarrage ESP32
```

**Fonction** : `handleLongPress()` + `digitalRead()` check  
**Vérification** : Loop teste chaque 50ms si bouton toujours appuyé  
**Affichage** : Barre magenta TFT avec pourcentage  
**Fichier** : `src/main.cpp` (lignes 52-120)

---

### 4️⃣ Bouton 1 - Cycle RGB

**Cycle** :
```
Clic → Red → Green → Blue → White → Off → Red → ...
```

**Pins** (ESP32-S3) :
- PIN_LED_RED = 21 (HIGH = On)
- PIN_LED_GREEN = 41
- PIN_LED_BLUE = 42

**Fonction** : `handleButton1Click()`  
**Fichier** : `src/main.cpp` (lignes 128-170)

---

### 5️⃣ Bouton 2 - Bip Buzzer

**Comportement** :
```
Appui → Bip à la détection (1kHz, 100ms) → Relâchement → Stop
```

**Délai activé** : 50ms (très court pour détection immédiate)  
**Fonction** : `handleButton2PressStart()` + `handleButton2PressStop()`  
**Fichier** : `src/main.cpp` (lignes 172-187)

---

### 6️⃣ Gestion NeoPixel

**États affichés** :
```cpp
// Connecté au WiFi : Vert heartbeat
pixels.setPixelColor(0, pixels.Color(0, 100, 0)); // Fort
pixels.setPixelColor(0, pixels.Color(0, 20, 0));  // Faible

// Recherche / Déconnecté : Rouge heartbeat
pixels.setPixelColor(0, pixels.Color(100, 0, 0)); // Fort
pixels.setPixelColor(0, pixels.Color(20, 0, 0));  // Faible

// Redémarrage : Violet fixe
pixels.setPixelColor(0, pixels.Color(128, 0, 128));
```

**Condition** : `if (!isRebooting)` pour respecter état reboot  
**Fréquence** : Heartbeat toutes les 1 seconde  
**Fichier** : `src/main.cpp` (lignes 341-360)

---

### 7️⃣ Initialisation setup()

**Avant** :
```cpp
btn.attachClick(handleClick);
btn.attachLongPressStop(handleLongPress);
```

**Après** :
```cpp
// Bouton BOOT
btn.attachClick(handleClick);
btn.attachLongPressStart(handleLongPress);
btn.setPressMs(1000);

// Bouton 1 - RGB
btn1.attachClick(handleButton1Click);
btn1.setPressMs(1000);

// Bouton 2 - Buzzer
btn2.attachLongPressStart(handleButton2PressStart);
btn2.attachLongPressStop(handleButton2PressStop);
btn2.setPressMs(50);
```

**Fichier** : `src/main.cpp` (lignes 284-306)

---

### 8️⃣ Loop() - Surveillance des boutons

**Avant** :
```cpp
btn.tick();
```

**Après** :
```cpp
btn.tick();   // BOOT
btn1.tick();  // RGB
btn2.tick();  // Buzzer
```

**Fichier** : `src/main.cpp` (lignes 317-319)

---

## 🧪 Tests de validation

### ✅ Tests effectués

| Test | Résultat | Notes |
|------|----------|-------|
| Cycle RGB bouton 1 | ✅ Validé | Rouge→Vert→Bleu→Blanc→Éteint→Rouge |
| Bip buzzer bouton 2 | ✅ Validé | 1kHz 100ms à l'appui |
| Barre reboot 100% | ✅ Validé | Redémarrage si appui maintenu 2s |
| Annulation reboot | ✅ Validé | Reboot annulé si relâchement avant 100% |
| Écran restauré | ✅ Validé | WiFi connected/failed affichage restauré |
| NeoPixel Vert/Rouge | ✅ Validé | Heartbeat selon état WiFi |
| NeoPixel Violet | ✅ Validé | Violet maintenu pendant reboot |
| Compilation | ✅ SUCCESS | 25.3% Flash, 14.1% RAM |

---

## 📦 Fichiers modifiés

| Fichier | Lignes | Type |
|---------|--------|------|
| `include/config.h` | +3 | Ajout HAS_LED_RGB, HAS_NEOPIXEL |
| `src/main.cpp` | +200 | Refonte système boutons |
| `platformio.ini` | +1 | Version 0.8.0 |
| `CHANGELOG.md` | +50 | Section v0.8.0 |
| `README.md` | +10 | Mise à jour fonctionnalités |

---

## 🚀 Déploiement

### Checklist pré-déploiement ✅

- [x] Code compilé sans erreur
- [x] Tous les tests validés
- [x] Documentation mise à jour
- [x] CHANGELOG.md complet
- [x] Version 0.8.0 dans platformio.ini
- [x] Commit sur Git avec tag v0.8.0
- [x] Push sur GitHub réussi
- [x] Branche main synchronisée

### Instructions pour mise à jour

1. **Pull depuis GitHub** :
   ```bash
   git pull origin main
   git fetch --tags
   ```

2. **Vérifier la version** :
   ```bash
   grep PROJECT_VERSION platformio.ini
   ```

3. **Compiler** :
   ```bash
   platformio run --environment esp32s3_n16r8
   ```

4. **Uploader** :
   ```bash
   platformio run --target upload --environment esp32s3_n16r8
   ```

---

## 🔄 Prochaines étapes possibles

- [ ] Ajouter vibrations (moteur) si hardware disponible
- [ ] Enregistrement des séquences boutons dans logs
- [ ] Configuration boutons via interface web
- [ ] Modes de reboot (warm/hard)
- [ ] Macros boutons multi-appuis

---

## 🆘 Dépannage

### La LED RGB ne change pas de couleur

1. Vérifier que `HAS_LED_RGB` est activé dans `config.h`
2. Tester les pins : RED=21, GREEN=41, BLUE=42
3. Vérifier les résistances de limitation (220-470Ω)
4. Logs : "Bouton 1 cliqué ! État RGB: X"

### Le buzzer ne fait pas de bruit

1. Vérifier PIN_BUZZER = 6
2. Vérifier que le transistor est correctement branché
3. Tester avec `tone(6, 1000, 100);` en terminal série

### Le reboot ne s'active pas

1. Appuyer au moins 1 seconde
2. Maintenir jusqu'à 100% (2 secondes totales)
3. Vérifier PIN_BUTTON_BOOT = 0

### La NeoPixel ne clignote pas

1. Vérifier que `HAS_NEOPIXEL` est activé
2. Vérifier PIN_NEOPIXEL = 48
3. Vérifier la luminosité (setBrightness = 30)

---

**Version** : 0.8.0  
**Statut** : ✅ Prêt pour production  
**Support** : Consultez la documentation complète dans `docs/`
