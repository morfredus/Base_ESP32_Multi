# Base_ESP32_S3

Template de démarrage universel pour projets ESP32 et ESP32-S3 sous PlatformIO.
Inclut la gestion automatique du WiFi, une structure multi-environnements et le support des LED de statut (NeoPixel).

## 🚀 Fonctionnalités
* **Multi-Cartes** : Prêt pour ESP32-S3 (N16R8, N8R8) et ESP32 Classic (DevKitC).
* **WiFiMulti** : Tente de se connecter à une liste de réseaux connus.
* **Sécurité** : Les identifiants WiFi sont exclus de Git (`secrets.h`).
* **Feedback Visuel** : Gestion automatique de la LED RGB (Pin 48 sur S3) pour indiquer l'état WiFi (Jaune=Connexion, Vert=OK, Rouge=Erreur).
* **Structure Propre** : Séparation de la config projet (`config.h`) et du mapping hardware (`board_config.h`).

## 🛠️ Installation & Démarrage

### 1. Cloner le projet
Récupérez ce dossier et renommez-le selon votre nouveau projet.

### 2. Créer le fichier `secrets.h`
Le fichier `include/secrets.h` est ignoré par Git. Vous devez le créer manuellement :

```cpp
// include/secrets.h
#ifndef SECRETS_H
#define SECRETS_H

const char* WIFI_NETWORKS[][2] = {
    {"SSID_Maison", "MotDePasse123"},
    {"SSID_Bureau", "MotDePasse456"},
    {"Partage_Tel", "12345678"}
};

#endif