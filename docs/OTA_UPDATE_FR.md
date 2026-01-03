# Guide des Mises à Jour OTA

**Version :** 0.9.0
**Dernière mise à jour :** 2026-01-03

Ce guide explique comment mettre à jour votre firmware ESP32 Over-The-Air (OTA) sans avoir besoin d'un câble USB. Base_ESP32_Multi supporte deux méthodes OTA : téléversement via navigateur web et téléversement réseau via Arduino IDE/PlatformIO.

---

## 📋 Table des matières

1. [Qu'est-ce que l'OTA ?](#quest-ce-que-lota)
2. [Méthodes OTA](#méthodes-ota)
3. [OTA via navigateur web (Recommandé)](#ota-via-navigateur-web-recommandé)
4. [OTA réseau via PlatformIO](#ota-réseau-via-platformio)
5. [Considérations de sécurité](#considérations-de-sécurité)
6. [Dépannage](#dépannage)
7. [Bonnes pratiques](#bonnes-pratiques)

---

## Qu'est-ce que l'OTA ?

**OTA (Over-The-Air)** permet de téléverser un nouveau firmware vers votre ESP32 **sans fil** via WiFi, sans connecter de câble USB.

### Avantages

- ✅ **Pas d'accès physique nécessaire** - Mettez à jour des appareils difficiles d'accès
- ✅ **Déploiement plus rapide** - Mettez à jour plusieurs appareils rapidement
- ✅ **Usure réduite** - Pas d'usure du port USB par connexions répétées
- ✅ **Mises à jour à distance** - Mettez à jour n'importe où sur votre réseau
- ✅ **Interface conviviale** - Interface web pour utilisateurs non-techniques

### Prérequis

- L'ESP32 doit être connecté au WiFi
- Mémoire flash suffisante (OTA nécessite de l'espace pour l'ancien et le nouveau firmware)
- Même réseau que votre ordinateur (ou redirection de port pour mises à jour à distance)

---

## Méthodes OTA

Base_ESP32_Multi v0.9.0 supporte **deux méthodes OTA** :

| Méthode | Interface | Idéal pour | Difficulté |
|---------|-----------|------------|------------|
| **OTA navigateur web** | Upload HTTP via page web | Utilisateurs finaux, mises à jour rapides | ⭐ Facile |
| **OTA réseau** | Arduino IDE / PlatformIO | Développeurs, débogage | ⭐⭐ Intermédiaire |

Les deux méthodes sont incluses dans le firmware par défaut !

---

## OTA via navigateur web (Recommandé)

### Vue d'ensemble

Téléversez un fichier firmware `.bin` compilé via un navigateur web. Parfait pour les non-développeurs !

### Instructions étape par étape

#### 1. Compiler votre firmware

D'abord, compilez votre projet pour générer un fichier `.bin` :

**Avec PlatformIO :**

```bash
pio run
```

Le fichier firmware sera situé à :
```
.pio/build/<environnement>/firmware.bin
```

Par exemple :
- `.pio/build/esp32s3_n16r8/firmware.bin`
- `.pio/build/esp32devkitc/firmware.bin`

#### 2. Accéder à la page OTA

1. Assurez-vous que votre ESP32 est allumé et connecté au WiFi
2. Vérifiez le moniteur série pour l'adresse IP :
   ```
   Serveur web démarré sur http://192.168.1.100
     - Page OTA : http://192.168.1.100/ota
   ```

3. Ouvrez un navigateur web et allez à :
   ```
   http://<Adresse-IP-ESP32>/ota
   ```
   Exemple : `http://192.168.1.100/ota`

#### 3. Téléverser le firmware

1. **Cliquez sur "Choose File"** et sélectionnez votre fichier firmware `.bin`
2. **Cliquez sur "Upload Firmware"**
3. **Attendez le téléversement** - Une barre de progression affichera le statut
4. **Succès !** - L'appareil redémarrera automatiquement avec le nouveau firmware

**Progression du téléversement :**
```
⏳ Uploading firmware... [████████████] 100%
✅ Upload successful! Device rebooting...
```

#### 4. Vérifier la mise à jour

Après le redémarrage (~10 secondes) :

1. La page web redirigera automatiquement vers le tableau de bord
2. Vérifiez le numéro de version dans l'en-tête
3. Ou vérifiez le moniteur série pour la nouvelle version :
   ```
   === Base_ESP32_Multi v0.9.1 ===  ← Nouvelle version
   ```

---

### Fonctionnalités OTA Web

**✅ Support glisser-déposer** - Déposez les fichiers `.bin` directement sur la zone de téléversement
**✅ Validation de fichier** - Avertit si vous téléversez un fichier non-.bin
**✅ Progression en temps réel** - Voir le pourcentage et les données transférées
**✅ Validation inline** - Pas de confirmations popup agaçantes
**✅ Redémarrage auto** - L'appareil redémarre automatiquement après téléversement réussi
**✅ Gestion d'erreurs** - Messages d'erreur clairs en cas d'échec

---

## OTA réseau via PlatformIO

### Vue d'ensemble

Téléversez le firmware directement depuis PlatformIO en utilisant l'adresse réseau de l'ESP32. Idéal pour le développement !

### Prérequis

1. ESP32 connecté au WiFi (même réseau que votre ordinateur)
2. ArduinoOTA activé dans le firmware (déjà inclus dans v0.9.0)
3. Connaître l'adresse IP de votre ESP32

### Configuration

#### 1. Trouver l'adresse IP de votre ESP32

Vérifiez le moniteur série après le démarrage :
```
Serveur web démarré sur http://192.168.1.100
```

Ou vérifiez la liste des clients DHCP de votre routeur.

#### 2. Configurer platformio.ini

Ajoutez ce qui suit à votre environnement dans `platformio.ini` :

```ini
[env:esp32s3_n16r8]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino

; ... paramètres existants ...

; Configuration OTA
upload_protocol = espota
upload_port = 192.168.1.100      ; ← Remplacez par l'IP de votre ESP32
upload_flags =
    --port=3232
    --auth=                       ; Laissez vide si pas de mot de passe
```

**Pour plusieurs environnements**, ajoutez à chacun que vous souhaitez téléverser en OTA.

#### 3. Téléverser via OTA

```bash
pio run --target upload
```

Ou utilisez l'interface PlatformIO : bouton **Upload** (comme pour USB)

### Processus de téléversement OTA

Vous verrez une sortie comme :

```
Uploading .pio/build/esp32s3_n16r8/firmware.bin
Uploading: [============      ] 60%
Uploading: [==================] 100%
Success
```

L'ESP32 redémarrera automatiquement avec le nouveau firmware !

---

### Fonctionnalités OTA Réseau

Le firmware inclut un retour visuel pendant l'OTA réseau :

#### Écran TFT (si disponible)
- Affiche le titre "OTA UPDATE"
- Barre de progression en temps réel (0-100%)
- Vitesse de téléversement et temps estimé
- Messages de succès/erreur

#### LED NeoPixel (si disponible)
- **Bleu** - Mise à jour OTA en cours
- **Vert** - OTA réussie
- **Rouge** - Erreur OTA

#### Moniteur série
Logs détaillés :
```
ArduinoOTA démarré sur le port 3232
Hostname: Base_ESP32_Multi
Mise à jour OTA démarrée
Progression: 0%
Progression: 25%
Progression: 50%
Progression: 75%
Progression: 100%
Mise à jour OTA terminée ! Redémarrage...
```

---

## Considérations de sécurité

### Sécurité par défaut

Par défaut, **aucun mot de passe** n'est requis pour les mises à jour OTA. C'est pratique mais **pas sécurisé** pour des appareils en production !

### Ajouter une protection par mot de passe OTA

#### Pour l'OTA Web

Éditez `include/web_interface.h` et ajoutez l'authentification :

```cpp
void handleOTA() {
    // Ajouter l'authentification HTTP Basic
    if (!server.authenticate("admin", "votre-mot-de-passe-ici")) {
        return server.requestAuthentication();
    }

    String html = generateOTAPage();
    server.send(200, "text/html", html);
}
```

#### Pour l'OTA Réseau (ArduinoOTA)

Éditez `src/main.cpp` dans la fonction `setupOTA()` :

```cpp
void setupOTA() {
    ArduinoOTA.setPort(3232);
    ArduinoOTA.setHostname(PROJECT_NAME);

    // Ajouter protection par mot de passe
    ArduinoOTA.setPassword("votre-mot-de-passe-ota");  // ← Ajoutez cette ligne

    // ... reste de la configuration ...
}
```

Puis mettez à jour `platformio.ini` :

```ini
upload_flags =
    --port=3232
    --auth=votre-mot-de-passe-ota     ; ← Ajoutez le mot de passe
```

### Sécurité réseau

**⚠️ Pratiques de sécurité importantes :**

1. **Utilisez des mots de passe** - Définissez toujours des mots de passe OTA pour les appareils hors réseau domestique
2. **Réseaux de confiance uniquement** - Activez l'OTA uniquement sur des réseaux WiFi de confiance
3. **Règles de pare-feu** - Bloquez le port 3232 depuis l'accès externe
4. **HTTPS** - Pour la production, envisagez HTTPS pour l'OTA web (nécessite certificats SSL)
5. **Vérifiez le firmware** - Revérifiez les fichiers .bin avant téléversement

**Pour les appareils en production**, envisagez :
- Implémenter la vérification de signature du firmware
- Utiliser un VPN pour l'accès OTA à distance
- Désactiver l'OTA après déploiement (via flag de compilation)

---

## Dépannage

### Problème : "Page OTA inaccessible"

**Symptômes :**
- Le navigateur affiche "Connexion refusée" ou "Page introuvable"
- Impossible d'accéder à `http://<IP>/ota`

**Solutions :**
1. Vérifiez que l'ESP32 est connecté au WiFi (vérifiez le moniteur série)
2. Pingez l'ESP32 : `ping 192.168.1.100` (remplacez par votre IP)
3. Vérifiez si le serveur web a démarré (le moniteur série doit afficher "Serveur web démarré")
4. Assurez-vous d'être sur le même réseau que l'ESP32
5. Essayez d'abord la page racine : `http://<IP>/` (tableau de bord)
6. Désactivez temporairement le pare-feu pour tester
7. Essayez le mode navigation privée/incognito

---

### Problème : "Le téléversement échoue à 0%" ou "Connexion perdue"

**Symptômes :**
```
⏳ Uploading firmware...
❌ Network error. Check connection and try again.
```

**Solutions :**

1. **Mémoire insuffisante** - L'ESP32 n'a pas assez d'espace flash libre
   - Vérifiez si taille firmware.bin < flash disponible
   - Libérez de l'espace en réduisant code/assets

2. **Signal WiFi faible** - Rapprochez l'ESP32 du routeur pendant la mise à jour

3. **Problèmes d'alimentation** - Utilisez une alimentation USB de qualité (2A minimum)
   - Le téléversement OTA est gourmand en énergie
   - Une alimentation faible peut causer des crashs

4. **Congestion réseau** - Mettez en pause les autres activités réseau pendant la mise à jour

5. **Pare-feu bloquant** - Désactivez temporairement le pare-feu

6. **Mauvaise adresse IP** - Revérifiez l'IP actuelle de l'ESP32

---

### Problème : "Le téléversement se termine mais l'appareil ne démarre pas"

**Symptômes :**
- Le téléversement affiche 100% de succès
- L'appareil redémarre mais ne démarre pas (boot loop)
- Le moniteur série affiche du texte illisible ou des erreurs répétées

**Solutions :**

1. **Mauvais fichier firmware** - Assurez-vous d'avoir téléversé le bon .bin pour votre carte
   - Le firmware ESP32-S3 ne fonctionnera pas sur ESP32 Classic
   - Vérifiez que les partitions correspondent

2. **Téléversement corrompu** - Retéléversez le firmware
   - Essayez l'OTA Web au lieu de l'OTA Réseau (ou vice versa)

3. **Dépassement de partition** - Le firmware est trop volumineux pour la partition
   - Vérifiez la taille de `.pio/build/<env>/firmware.bin`
   - Comparez avec la table de partitions dans `platformio.ini`

4. **Flasher manuellement** - Si l'OTA est cassée, utilisez le câble USB pour récupérer :
   ```bash
   pio run --target upload
   ```

---

### Problème : "OTA réseau introuvable par PlatformIO"

**Symptômes :**
```
Error: Timeout waiting for response
Could not connect to esp32-s3-devkitc-1 on port 3232
```

**Solutions :**

1. **Vérifiez l'adresse IP** - Vérifiez que l'IP ESP32 dans `platformio.ini` correspond à l'IP actuelle
2. **Port 3232 bloqué** - Vérifiez les règles du pare-feu
3. **mDNS ne fonctionne pas** - Utilisez l'adresse IP au lieu du nom d'hôte
4. **ESP32 ne diffuse pas** - Vérifiez le moniteur série pour "ArduinoOTA démarré"
5. **Mauvais réseau** - Assurez-vous que l'ordinateur et l'ESP32 sont sur le même sous-réseau

---

### Problème : "Le téléversement réussit mais affiche l'ancienne version"

**Symptômes :**
- Le téléversement se termine avec succès
- L'appareil redémarre
- Le numéro de version ne change pas

**Solutions :**

1. **Cache du navigateur** - Actualisez la page web (Ctrl+F5 ou Cmd+Shift+R)
2. **Mauvais fichier téléversé** - Vérifiez que vous avez compilé avant de téléverser
3. **Version non mise à jour** - Vérifiez si vous avez réellement changé PROJECT_VERSION dans platformio.ini
4. **Ancienne compilation** - Nettoyez et recompilez :
   ```bash
   pio run --target clean
   pio run
   ```

---

## Bonnes pratiques

### Workflow de développement

1. **Testez localement d'abord** - Utilisez le téléversement USB pour les tests initiaux
2. **Activez l'OTA après que le WiFi fonctionne** - Ne comptez pas sur l'OTA pour le premier téléversement
3. **Gardez l'USB accessible** - Ayez toujours un moyen de récupérer via USB
4. **Versionnez tout** - Mettez à jour le numéro de version à chaque compilation
5. **Testez l'OTA tôt** - Vérifiez que l'OTA fonctionne avant de déployer des appareils à distance

### Déploiement en production

1. **Définissez toujours des mots de passe** - Protégez à la fois l'OTA Web et l'OTA Réseau
2. **Testez le firmware à fond** - Ne déployez jamais de firmware non testé via OTA
3. **Ayez un plan de rollback** - Gardez la version précédente du firmware prête
4. **Surveillez les téléversements** - Regardez le moniteur série pendant les mises à jour OTA
5. **Déploiement progressif** - Mettez à jour un appareil d'abord, vérifiez, puis mettez à jour les autres
6. **Documentez les versions** - Tenez un CHANGELOG de ce que chaque version change

### Mesures de sécurité

```cpp
// Dans main.cpp - Ajoutez des vérifications de sécurité OTA
void setupOTA() {
    ArduinoOTA.setHostname(PROJECT_NAME);
    ArduinoOTA.setPassword("mon-mot-de-passe-securise");

    ArduinoOTA.onStart([]() {
        // Empêcher les écritures pendant l'OTA
        // Fermez les fichiers, arrêtez les tâches, etc.
    });

    ArduinoOTA.onError([](ota_error_t error) {
        // Logger l'erreur pour le débogage
        LOG_PRINTF("Erreur OTA[%u]: ", error);
        if (error == OTA_AUTH_ERROR) LOG_PRINTLN("Échec Auth");
        else if (error == OTA_BEGIN_ERROR) LOG_PRINTLN("Échec Begin");
        else if (error == OTA_CONNECT_ERROR) LOG_PRINTLN("Échec Connect");
        else if (error == OTA_RECEIVE_ERROR) LOG_PRINTLN("Échec Receive");
        else if (error == OTA_END_ERROR) LOG_PRINTLN("Échec End");

        // Ne pas redémarrer en cas d'erreur - permettre une nouvelle tentative
    });

    ArduinoOTA.begin();
}
```

---

## Liste de vérification téléversement OTA

Avant chaque mise à jour OTA :

- [ ] Firmware compilé avec succès (pas d'erreurs)
- [ ] Numéro de version mis à jour dans platformio.ini
- [ ] Testé sur un appareil via USB d'abord
- [ ] L'ESP32 a une connexion WiFi stable
- [ ] Bonne alimentation connectée (2A+ recommandé)
- [ ] Sauvegarde du firmware précédent
- [ ] Savoir comment récupérer via USB si l'OTA échoue
- [ ] Moniteur série prêt pour surveiller le téléversement

---

## Obtenir de l'aide

Si vous rencontrez des problèmes non couverts ici :

1. **Vérifiez le moniteur série** - La plupart des erreurs y sont journalisées
2. **Vérifiez la connectivité réseau** - Pouvez-vous pinger l'ESP32 ?
3. **Testez avec USB d'abord** - Assurez-vous que le firmware fonctionne avant l'OTA
4. **Vérifiez les Issues GitHub** - Quelqu'un a peut-être eu le même problème

**Issues GitHub :** https://github.com/morfredus/Base_ESP32_Multi/issues

---

## Résumé

- ✅ **Deux méthodes OTA :** Navigateur Web (facile) et Réseau (PlatformIO)
- ✅ **OTA Web :** Téléversez des fichiers .bin via page web à `http://<IP>/ota`
- ✅ **OTA Réseau :** Configurez `upload_port` dans platformio.ini
- ✅ **Sécurité :** Ajoutez des mots de passe pour les appareils en production
- ✅ **Dépannage :** Vérifiez WiFi, alimentation et taille du firmware
- ✅ **Bonnes pratiques :** Testez localement, versionnez tout, surveillez les téléversements

Bonnes mises à jour sans fil ! 📡
