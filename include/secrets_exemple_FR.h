/**
 * @file secrets_exemple.h
 * @brief Fichier de configuration exemple pour les identifiants WiFi
 * @version 0.9.1
 *
 * IMPORTANT : Ceci est un fichier EXEMPLE. Vous devez :
 * 1. Copier ce fichier vers 'secrets.h' dans le même répertoire
 * 2. Éditer 'secrets.h' avec vos vrais identifiants WiFi
 * 3. NE JAMAIS versionner 'secrets.h' (il est dans .gitignore)
 *
 * Utilisation :
 * - Le système tentera de se connecter aux réseaux dans l'ordre (SSID1, SSID2, SSID3, etc.)
 * - Ajoutez autant de réseaux que nécessaire en créant WIFI_SSID4/WIFI_PASS4, etc.
 * - Laissez les réseaux inutilisés avec des chaînes vides ("") pour les désactiver
 */

#ifndef SECRETS_H
#define SECRETS_H

// ============================================================
// Configuration Réseaux WiFi (Support Multi-Réseaux)
// ============================================================
// L'ESP32 tentera de se connecter à ces réseaux dans l'ordre.
// Ajoutez vos identifiants WiFi ci-dessous. Vous pouvez définir jusqu'à 10 réseaux.

// Réseau 1 (Principal)
const char* WIFI_SSID1 = "VotreReseauMaison";   // Remplacer par votre SSID WiFi
const char* WIFI_PASS1 = "VotreMotDePasse123";  // Remplacer par votre mot de passe WiFi

// Réseau 2 (Secondaire - Bureau, backup, etc.)
const char* WIFI_SSID2 = "ReseauBureau";        // Remplacer ou laisser vide "" pour désactiver
const char* WIFI_PASS2 = "MotDePasseBureau456"; // Remplacer ou laisser vide "" pour désactiver

// Réseau 3 (Partage mobile, réseau invité, etc.)
const char* WIFI_SSID3 = "";                    // Optionnel - laisser vide pour désactiver
const char* WIFI_PASS3 = "";                    // Optionnel - laisser vide pour désactiver

// Réseau 4 (Optionnel)
const char* WIFI_SSID4 = "";                    // Optionnel - laisser vide pour désactiver
const char* WIFI_PASS4 = "";                    // Optionnel - laisser vide pour désactiver

// ============================================================
// NOTES IMPORTANTES :
// ============================================================
// - Les mots de passe sont sensibles à la casse
// - Les noms SSID doivent correspondre exactement (espaces inclus)
// - Pour les réseaux WPA2/WPA3, utilisez le mot de passe directement
// - Pour les réseaux ouverts (sans mot de passe), utilisez une chaîne vide : ""
// - Longueur maximale du mot de passe : 63 caractères
// - Longueur maximale du SSID : 32 caractères
//
// Exemple pour réseau ouvert :
//   const char* WIFI_SSID3 = "WiFiPublicGratuit";
//   const char* WIFI_PASS3 = "";
//
// Pour ajouter plus de réseaux, créez simplement WIFI_SSID5/WIFI_PASS5, etc.
// et mettez à jour le code de configuration WiFi dans main.cpp en conséquence.
// ============================================================

#endif // SECRETS_H
