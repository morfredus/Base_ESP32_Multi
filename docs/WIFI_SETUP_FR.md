# Guide de Configuration WiFi

**Version :** 0.9.0
**Dernière mise à jour :** 2026-01-03

Ce guide explique comment configurer la connectivité WiFi pour Base_ESP32_Multi, incluant le support multi-réseaux et le dépannage avancé.

---

## 📋 Table des matières

1. [Vue d'ensemble](#vue-densemble)
2. [Configuration rapide](#configuration-rapide)
3. [Configuration multi-réseaux](#configuration-multi-réseaux)
4. [Bonnes pratiques de sécurité](#bonnes-pratiques-de-sécurité)
5. [Configuration avancée](#configuration-avancée)
6. [Dépannage](#dépannage)

---

## Vue d'ensemble

Base_ESP32_Multi utilise **WiFiMulti** pour supporter plusieurs réseaux WiFi avec basculement automatique. Cela signifie :

- ✅ Définir jusqu'à **10 réseaux WiFi** (facilement extensible)
- ✅ **Connexion automatique** au réseau disponible le plus fort
- ✅ **Reconnexion automatique** si le réseau actuel échoue
- ✅ **Stockage sécurisé** des identifiants (fichier `secrets.h` gitignored)
- ✅ **Basculement facile** entre maison, bureau, hotspot mobile, etc.

**Standards WiFi supportés :**
- 802.11 b/g/n (2.4 GHz uniquement)
- Chiffrement WPA/WPA2/WPA3 Personnel
- Réseaux ouverts (sans mot de passe)

**⚠️ Important :** L'ESP32 ne supporte **pas** le WiFi 5GHz (802.11ac/ax). Utilisez uniquement des réseaux 2.4GHz.

---

## Configuration rapide

### Étape 1 : Créer secrets.h

Le fichier `secrets.h` contient vos identifiants WiFi et n'est **pas suivi par git** pour la sécurité.

```bash
# Depuis la racine du projet
cp include/secrets_exemple_FR.h include/secrets.h
```

### Étape 2 : Éditer les identifiants WiFi

Ouvrez `include/secrets.h` dans votre éditeur et configurez au moins un réseau :

```cpp
// Réseau 1 (Principal) - REQUIS
const char* WIFI_SSID1 = "MonWiFiMaison";      // Nom de votre réseau WiFi
const char* WIFI_PASS1 = "MonMotDePasse123";   // Mot de passe WiFi

// Réseau 2 (Secondaire) - OPTIONNEL
const char* WIFI_SSID2 = "";  // Laissez vide si non nécessaire
const char* WIFI_PASS2 = "";

// Réseau 3 (Mobile/Invité) - OPTIONNEL
const char* WIFI_SSID3 = "";
const char* WIFI_PASS3 = "";

// Réseau 4 (Extra) - OPTIONNEL
const char* WIFI_SSID4 = "";
const char* WIFI_PASS4 = "";
```

### Étape 3 : Compiler et téléverser

```bash
pio run --target upload
```

### Étape 4 : Vérifier la connexion

Ouvrez le moniteur série (115200 baud) et vérifiez :

```
--- Démarrage WiFiMulti ---
  - Réseau ajouté : MonWiFiMaison
Connexion WiFi...
✓ Connecté à : MonWiFiMaison
Adresse IP : 192.168.1.100
```

---

## Configuration multi-réseaux

### Cas d'usage 1 : Réseaux Maison + Bureau

Parfait pour les appareils qui se déplacent entre différents lieux :

```cpp
const char* WIFI_SSID1 = "ReseauMaison";
const char* WIFI_PASS1 = "MotDePasseMaison";

const char* WIFI_SSID2 = "ReseauBureau";
const char* WIFI_PASS2 = "MotDePasseBureau";

const char* WIFI_SSID3 = "";
const char* WIFI_PASS3 = "";

const char* WIFI_SSID4 = "";
const char* WIFI_PASS4 = "";
```

**Comment ça fonctionne :**
- L'ESP32 essaie d'abord `ReseauMaison`
- Si indisponible, il essaie `ReseauBureau`
- Bascule automatiquement quand vous changez de lieu

---

### Cas d'usage 2 : Maison + Hotspot Mobile de Secours

Utile pour les tests ou quand votre réseau principal est en panne :

```cpp
const char* WIFI_SSID1 = "ReseauMaison";
const char* WIFI_PASS1 = "MotDePasseMaison";

const char* WIFI_SSID2 = "iPhone-Hotspot";
const char* WIFI_PASS2 = "hotspot123";

const char* WIFI_SSID3 = "";
const char* WIFI_PASS3 = "";

const char* WIFI_SSID4 = "";
const char* WIFI_PASS4 = "";
```

---

### Cas d'usage 3 : Plusieurs réseaux avec WiFi Ouvert

Certains réseaux ne nécessitent pas de mot de passe (réseaux ouverts) :

```cpp
const char* WIFI_SSID1 = "MaisonSecurisee";
const char* WIFI_PASS1 = "MotDePasseForт";

const char* WIFI_SSID2 = "ReseauInvite";      // Réseau ouvert
const char* WIFI_PASS2 = "";                   // Mot de passe vide pour réseaux ouverts

const char* WIFI_SSID3 = "";
const char* WIFI_PASS3 = "";

const char* WIFI_SSID4 = "";
const char* WIFI_PASS4 = "";
```

**⚠️ Avertissement sécurité :** Les réseaux ouverts transmettent les données non chiffrées. Évitez-les pour des opérations sensibles.

---

### Ajouter plus de 4 réseaux

Pour ajouter les réseaux 5, 6, 7, etc. :

**1. Éditez `include/secrets.h` :**

```cpp
// Ajoutez ces lignes
const char* WIFI_SSID5 = "Reseau5";
const char* WIFI_PASS5 = "MotDePasse5";

const char* WIFI_SSID6 = "Reseau6";
const char* WIFI_PASS6 = "MotDePasse6";
```

**2. Éditez `src/main.cpp` dans la fonction `setupWifi()` :**

```cpp
void setupWifi() {
    LOG_PRINTLN("--- Démarrage WiFiMulti ---");

    if (strlen(WIFI_SSID1) > 0) {
        wifiMulti.addAP(WIFI_SSID1, WIFI_PASS1);
        LOG_PRINTF("  - Réseau ajouté : %s\n", WIFI_SSID1);
    }
    // ... SSID2, SSID3, SSID4 existants ...

    // Ajoutez ces nouveaux blocs :
    if (strlen(WIFI_SSID5) > 0) {
        wifiMulti.addAP(WIFI_SSID5, WIFI_PASS5);
        LOG_PRINTF("  - Réseau ajouté : %s\n", WIFI_SSID5);
    }
    if (strlen(WIFI_SSID6) > 0) {
        wifiMulti.addAP(WIFI_SSID6, WIFI_PASS6);
        LOG_PRINTF("  - Réseau ajouté : %s\n", WIFI_SSID6);
    }
    // etc.
}
```

---

## Bonnes pratiques de sécurité

### ✅ À FAIRE :

1. **Gardez secrets.h privé :**
   - Ne commitez jamais `secrets.h` sur git (déjà dans `.gitignore`)
   - Ne partagez jamais de captures d'écran ou logs contenant des mots de passe

2. **Utilisez des mots de passe forts :**
   - Minimum 12 caractères
   - Mélange de majuscules, minuscules, chiffres, symboles
   - Évitez les mots du dictionnaire

3. **Utilisez le chiffrement WPA2/WPA3 :**
   - Évitez WEP (sécurité compromise)
   - Préférez WPA3 si votre routeur le supporte

4. **Changez vos identifiants régulièrement :**
   - Changez les mots de passe WiFi périodiquement
   - Mettez à jour `secrets.h` après les changements

5. **Séparez les réseaux :**
   - Utilisez un réseau invité pour les appareils IoT si possible
   - Isole les appareils IoT de votre réseau principal

### ❌ À NE PAS FAIRE :

1. **Ne codez jamais les identifiants en dur dans main.cpp :**
   - ❌ Mauvais : `wifiMulti.addAP("MonWiFi", "password123");`
   - ✅ Bon : Utilisez les variables de `secrets.h`

2. **N'utilisez pas de réseaux ouverts pour des données sensibles :**
   - Les mises à jour OTA sur réseaux ouverts sont risquées
   - N'importe qui peut intercepter votre trafic

3. **Ne commitez pas secrets.h :**
   - Déjà empêché par `.gitignore`
   - Vérifiez doublement avant de pusher sur GitHub

---

## Configuration avancée

### Timeout de connexion

Par défaut, l'ESP32 essaie de se connecter pendant ~20 tentatives. Pour ajuster :

**Éditez `src/main.cpp` :**

```cpp
void setupWifi() {
    // ... configuration réseau ...

    int attempts = 0;
    const int maxAttempts = 30;  // ← Augmentez le timeout (défaut : 20)

    while (wifiMulti.run() != WL_CONNECTED && attempts < maxAttempts) {
        // ... logique de connexion ...
    }
}
```

### Configuration IP statique

Pour utiliser une IP fixe au lieu de DHCP :

**Éditez `src/main.cpp` après la connexion réseau :**

```cpp
void setupWifi() {
    // ... après connexion réussie ...

    if (wifiMulti.run() == WL_CONNECTED) {
        LOG_PRINTLN("✓ Connecté !");

        // Optionnel : Définir une IP statique
        IPAddress local_IP(192, 168, 1, 100);     // IP souhaitée
        IPAddress gateway(192, 168, 1, 1);        // IP du routeur
        IPAddress subnet(255, 255, 255, 0);       // Masque de sous-réseau
        IPAddress primaryDNS(8, 8, 8, 8);         // DNS Google
        IPAddress secondaryDNS(8, 8, 4, 4);       // DNS Google backup

        if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
            LOG_PRINTLN("Configuration IP statique échouée");
        }
    }
}
```

**⚠️ Attention :** Assurez-vous que l'IP statique est :
- En dehors de la plage DHCP de votre routeur
- Pas déjà utilisée par un autre appareil
- Dans le même sous-réseau que votre routeur

### Gestion de l'énergie WiFi

Pour optimiser pour la batterie ou la stabilité :

```cpp
void setupWifi() {
    // ... après connexion ...

    // Performance maximale (consommation élevée)
    WiFi.setSleep(false);

    // OU : Mode économie d'énergie (peut augmenter la latence)
    // WiFi.setSleep(true);
}
```

---

## Dépannage

### Problème : "Aucun réseau ajouté à WiFiMulti"

**Symptômes :**
```
--- Démarrage WiFiMulti ---
(Aucun message de réseau ajouté)
Connexion WiFi...
Échec de connexion WiFi
```

**Causes :**
1. Toutes les variables SSID sont des chaînes vides `""`
2. `secrets.h` pas créé (la compilation échouerait)

**Solution :**
- Assurez-vous qu'au moins WIFI_SSID1 n'est pas vide
- Vérifiez les fautes de frappe dans `secrets.h`

---

### Problème : "Échec connexion WiFi après 20 tentatives"

**Symptômes :**
```
--- Démarrage WiFiMulti ---
  - Réseau ajouté : MonWiFi
Connexion WiFi........
Échec de connexion WiFi après 20 tentatives
```

**Solutions :**

1. **Vérifiez SSID et mot de passe :**
   - Vérifiez l'orthographe (sensible à la casse)
   - Vérifiez les espaces supplémentaires : `"MonWiFi"` ≠ `"MonWiFi "`
   - Essayez de vous connecter avec votre téléphone pour vérifier les identifiants

2. **Vérifiez 2.4GHz vs 5GHz :**
   - L'ESP32 fonctionne uniquement avec 2.4GHz
   - Si votre routeur est bi-bande, assurez-vous que 2.4GHz est activé
   - Certains routeurs ont des SSID séparés pour chaque bande

3. **Force du signal :**
   - Rapprochez l'ESP32 du routeur
   - Retirez les objets métalliques entre l'ESP32 et le routeur
   - Vérifiez la connexion de l'antenne (si antenne externe)

4. **Paramètres du routeur :**
   - Désactivez le filtrage d'adresses MAC (ou ajoutez l'ESP32 à la liste blanche)
   - Désactivez l'isolation AP sur les réseaux invités
   - Vérifiez la limite maximale de clients (les routeurs ont des limites de connexion)
   - Essayez de définir un canal WiFi fixe (1, 6 ou 11 pour moins d'interférences)

5. **Reset de l'ESP32 :**
   - Certaines cartes ESP32 nécessitent un reset matériel après le flash
   - Appuyez sur le bouton RESET
   - Ou redémarrez la carte

---

### Problème : "Connecté mais pas d'internet"

**Symptômes :**
```
✓ Connecté à : MonWiFi
Adresse IP : 192.168.1.100
(Serveur web accessible localement, mais OTA échoue)
```

**Solutions :**
1. Vérifiez la connexion internet de votre routeur
2. Vérifiez les paramètres DNS (essayez `8.8.8.8` manuellement)
3. Vérifiez si votre réseau a des règles de pare-feu bloquant l'ESP32
4. Essayez de pinger la passerelle : ajoutez dans `setup()` :
   ```cpp
   if (WiFi.ping(WiFi.gatewayIP())) {
       LOG_PRINTLN("Passerelle accessible");
   }
   ```

---

### Problème : "Connexion se déconnecte fréquemment"

**Symptômes :**
- WiFi se connecte, puis se déconnecte après quelques minutes
- Se reconnecte automatiquement mais instable

**Solutions :**

1. **Problèmes d'alimentation :**
   - Le WiFi ESP32 consomme beaucoup de courant (~250mA en pointe)
   - Utilisez une alimentation USB de qualité (2A minimum)
   - Ajoutez un condensateur 100µF entre 3V3 et GND

2. **Interférences :**
   - Éloignez des fours micro-ondes, appareils Bluetooth
   - Essayez un canal WiFi différent sur votre routeur
   - Réduisez la distance au routeur

3. **Désactivez l'économie d'énergie :**
   ```cpp
   WiFi.setSleep(false);
   ```

4. **Implémentez la reconnexion automatique :**
   Le firmware inclut déjà la logique de reconnexion dans `loop()` :
   ```cpp
   void loop() {
       if (WiFi.status() != WL_CONNECTED) {
           LOG_PRINTLN("WiFi déconnecté, reconnexion...");
           wifiMulti.run();
       }
   }
   ```

---

### Problème : "secrets.h introuvable pendant la compilation"

**Symptômes :**
```
fatal error: secrets.h: No such file or directory
```

**Solution :**
Vous avez oublié de créer `secrets.h` :
```bash
cp include/secrets_exemple_FR.h include/secrets.h
```
Puis éditez-le avec vos identifiants WiFi.

---

### Déboguer les problèmes WiFi

Ajoutez le débogage WiFi détaillé :

**Éditez `platformio.ini` :**

```ini
build_flags =
    -DCORE_DEBUG_LEVEL=5        ; Niveau de débogage maximum
    -DLOG_ENABLED=1
```

**Recompilez et vérifiez la sortie série pour :**
- Résultats du scan WiFi
- Tentatives de connexion
- Négociation DHCP
- Codes d'erreur

---

## Tester votre configuration

### Script de test

Ajoutez ceci dans `loop()` temporairement pour tester le basculement réseau :

```cpp
void loop() {
    // ... code existant ...

    // Statut WiFi toutes les 10 secondes
    static unsigned long lastCheck = 0;
    if (millis() - lastCheck > 10000) {
        lastCheck = millis();

        LOG_PRINTF("Statut WiFi: %s\n", WiFi.status() == WL_CONNECTED ? "Connecté" : "Déconnecté");
        if (WiFi.status() == WL_CONNECTED) {
            LOG_PRINTF("  SSID: %s\n", WiFi.SSID().c_str());
            LOG_PRINTF("  IP: %s\n", WiFi.localIP().toString().c_str());
            LOG_PRINTF("  Signal: %d dBm\n", WiFi.RSSI());
        }
    }
}
```

**Bonne force de signal :**
- -30 à -50 dBm : Excellent
- -50 à -60 dBm : Bon
- -60 à -70 dBm : Moyen
- En dessous de -70 dBm : Faible (peut causer des problèmes)

---

## Obtenir de l'aide

Si vous avez toujours des problèmes :

1. **Vérifiez le moniteur série** (115200 baud) pour les messages d'erreur
2. **Testez avec votre téléphone** - Pouvez-vous vous connecter avec les mêmes identifiants ?
3. **Essayez un autre réseau** - Le problème est-il spécifique au réseau ?
4. **Signalez sur GitHub** avec :
   - Sortie série (retirez les mots de passe !)
   - Modèle et paramètres du routeur
   - Variante de carte ESP32

**Issues GitHub :** https://github.com/morfredus/Base_ESP32_Multi/issues

---

## Résumé

- ✅ Créez `secrets.h` depuis `secrets_exemple_FR.h`
- ✅ Configurez au moins un réseau (WIFI_SSID1/PASS1)
- ✅ Ajoutez plusieurs réseaux pour le basculement automatique
- ✅ Utilisez des mots de passe WPA2/WPA3 forts
- ✅ Gardez `secrets.h` privé (gitignored)
- ✅ Testez avec le moniteur série pour vérifier la connexion

Bon réseautage ! 🌐
