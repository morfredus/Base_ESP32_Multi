# 📚 Architecture Modulaire - Guide de Référence (v0.6.0+)

**[English Version](ARCHITECTURE.md)**

## Vue d'ensemble

À partir de la version 0.6.0, l'interface web est organisée en modules séparés pour améliorer :
- **Maintenabilité** : Code plus lisible et facile à modifier
- **Réutilisabilité** : Composants utilisables dans d'autres projets
- **Scalabilité** : Ajout facile de nouvelles pages/routes
- **Documentation** : Code Doxygen complet

## Structure des modules

### 1. `include/web_styles.h` - Feuille de styles CSS
**Responsabilité** : Contenir tous les styles CSS pour l'interface web

```cpp
#include "web_styles.h"

// Accès à : const char* WEB_STYLES
String html = "<style>" + String(WEB_STYLES) + "</style>";
```

**Contient** :
- Reset CSS (`*` sélecteur)
- Layout responsive (Grid, Flexbox)
- Composants (cards, buttons, progress bars)
- Animations et transitions

**Avantages** :
- ✅ CSS centralisé et facilement modifiable
- ✅ Peut être minifié/compressé pour économiser la mémoire
- ✅ Réutilisable dans plusieurs pages HTML

---

### 2. `include/web_pages.h` - Générateur de pages HTML
**Responsabilité** : Générer le contenu HTML de l'interface web

```cpp
#include "web_pages.h"

// Fonction disponible :
String html = generateDashboardPage(
    chipId, flashSize, flashSpeed,
    heapSize, freeHeap,
    psramSize, freePsram,
    cpuFreq
);
```

**Contient** :
- Fonction `generateDashboardPage()` - Génère la page complète du tableau de bord
- Inclusions des styles via `web_styles.h`
- Logique de calcul des pourcentages d'utilisation

**Avantages** :
- ✅ Génération HTML paramétrisée et flexible
- ✅ Facile d'ajouter de nouvelles cartes/sections
- ✅ Séparation entre données et présentation

**Extensibilité** :
```cpp
// Ajouter une nouvelle fonction dans web_pages.h :
String generateStatusPage(const char* status) {
    // Retourner HTML personnalisé
}

// Puis utiliser dans handleStatus() dans web_interface.h
void handleStatus() {
    String html = generateStatusPage("Connected");
    server.send(200, "text/html", html);
}
```

---

### 3. `include/web_interface.h` - Handlers et configuration serveur
**Responsabilité** : Gérer les routes HTTP et les callbacks du serveur web

```cpp
#include "web_interface.h"  // Inclut automatiquement web_pages.h et web_styles.h

// Fonctions disponibles :
void handleRoot();           // GET /
void handleReboot();         // GET /reboot
void handleNotFound();       // 404
void setupWebServer();       // Initialiser les routes
```

**Contient** :
- `handleRoot()` - Génère et envoie la page d'accueil
- `handleReboot()` - Gère les redémarrages
- `handleNotFound()` - Gestion des pages non trouvées
- `setupWebServer()` - Configure toutes les routes

**Dépendances** :
- Utilise `generateDashboardPage()` depuis `web_pages.h`
- Accède à `server` (variable globale depuis `main.cpp`)

---

## Diagramme de dépendances

```
main.cpp
   └── #include "web_interface.h"
       ├── #include "web_pages.h"
       │   └── #include "web_styles.h"
       └── extern WebServer server
```

## Workflow d'une requête HTTP

```
Client -> GET / 
         ↓
    handleRoot() (web_interface.h)
         ↓
    generateDashboardPage() (web_pages.h)
         ↓
    String(WEB_STYLES) (web_styles.h)
         ↓
    server.send(200, "text/html", html)
         ↓
Client <- HTML + CSS + JS
```

---

## Comment ajouter une nouvelle page/route

### Exemple : Ajouter une page `/api/status`

**1. Créer la fonction génératrice dans `web_pages.h`** :
```cpp
String generateStatusJson() {
    String json = "{";
    json += "\"status\":\"connected\",";
    json += "\"uptime\":" + String(millis() / 1000);
    json += "}";
    return json;
}
```

**2. Créer le handler dans `web_interface.h`** :
```cpp
void handleStatus() {
    String json = generateStatusJson();
    server.send(200, "application/json", json);
}
```

**3. Enregistrer la route dans `setupWebServer()`** :
```cpp
void setupWebServer() {
    server.on("/", handleRoot);
    server.on("/reboot", handleReboot);
    server.on("/api/status", handleStatus);  // ← Ajouter ici
    server.onNotFound(handleNotFound);
    server.begin();
    LOG_PRINTLN("Serveur web démarré sur http://" + WiFi.localIP().toString());
}
```

**4. Utiliser dans `main.cpp`** :
```cpp
// Pas besoin de modification ! setupWebServer() gère tout
```

---

## Bonnes pratiques

### 📏 Taille des modules
- `web_styles.h` : ~2-3 KB (CSS minifié)
- `web_pages.h` : ~5-10 KB (HTML généré dynamiquement)
- `web_interface.h` : ~2-3 KB (Handlers et config)

### 🔒 Sécurité
- Les données sensibles (WiFi, secrets) ne passent pas par le web
- Validation des inputs non implémentée (à ajouter si nécessaire)
- Redémarrage demande une confirmation via `confirm()` en JS

### 📱 Responsiveness
- Grille CSS auto-responsive : `grid-template-columns: repeat(auto-fit, minmax(280px, 1fr))`
- Mobile-first design avec media queries si nécessaire
- Tests sur navigateurs mobile recommandés

### ⚡ Optimisations possibles
- Minifier le CSS pour réduire la taille
- Compresser le HTML généré
- Ajouter du caching HTTP
- Servir des assets statiques (CSS/JS external) si stockage disponible

---

## Dépannage

| Problème | Solution |
|----------|----------|
| Page 404 | Vérifier `server.on()` dans `setupWebServer()` |
| CSS ne s'applique pas | Vérifier `#include "web_styles.h"` dans web_pages.h |
| Mémoire insuffisante | Réduire la taille des cartes ou utiliser des handlers pour des pages séparées |
| IP non accessible | Vérifier WiFi connecté avant `setupWebServer()` |

---

## Version

**Modulaire depuis** : v0.6.0 (2025-12-04)

**Fichiers impliqués** :
- ✅ `include/web_styles.h` - Créé en v0.6.0
- ✅ `include/web_pages.h` - Créé en v0.6.0
- ✅ `include/web_interface.h` - Créé en v0.6.0
- ✅ `src/main.cpp` - Refactorisé en v0.6.0 (424 → 271 lignes)
- ✅ `docs/ARCHITECTURE.md` - Guide déplacé et amélioré

---

Pour plus d'informations sur la structure générale du projet, voir [README.md](../README.md).
