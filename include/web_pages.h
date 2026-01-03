#ifndef WEB_PAGES_H
#define WEB_PAGES_H

/**
 * @file web_pages.h
 * @brief Générateur des pages HTML de l'interface web
 * @version 0.9.0
 *
 * Module dédié à la génération du contenu HTML de l'interface web.
 * Contient les fonctions pour construire les différentes cartes et sections.
 * Inclut maintenant la page OTA avec upload de firmware et validations inline.
 */

#include "web_styles.h"

/**
 * @brief Génère la page HTML complète du tableau de bord
 * @param chipId Identifiant unique du chip ESP32
 * @param flashSize Taille de la mémoire flash en bytes
 * @param flashSpeed Vitesse de la flash en MHz
 * @param heapSize Taille du heap total en bytes
 * @param freeHeap Heap libre en bytes
 * @param psramSize Taille PSRAM en bytes (0 si non présent)
 * @param freePsram PSRAM libre en bytes
 * @param cpuFreq Fréquence CPU en MHz
 * @return String contenant le HTML complet
 */
String generateDashboardPage(
    uint32_t chipId,
    uint32_t flashSize,
    uint32_t flashSpeed,
    uint32_t heapSize,
    uint32_t freeHeap,
    uint32_t psramSize,
    uint32_t freePsram,
    uint32_t cpuFreq
) {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta charset='utf-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>" + String(PROJECT_NAME) + "</title>";
    html += "<style>" + String(WEB_STYLES) + "</style>";
    html += "</head><body>";
    html += "<div class='container'>";
    
    // --- HEADER ---
    html += "<div class='header'>";
    html += "<h1>" + String(PROJECT_NAME) + "</h1>";
    html += "<p>Version " + String(PROJECT_VERSION) + "</p>";
    html += "</div>";
    
    html += "<div class='cards-grid'>";
    
    // --- CARTE 1: Matériel ---
    html += "<div class='card'>";
    html += "<div class='card-title'>⚡ Matériel</div>";
    html += "<div class='card-item'><span class='card-label'>Board:</span><span class='card-value'>" + String(BOARD_NAME) + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>Chip ID:</span><span class='card-value mono'>0x" + String(chipId, HEX) + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>SDK:</span><span class='card-value'>" + String(ESP.getSdkVersion()) + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>CPU:</span><span class='card-value'>" + String(cpuFreq) + " MHz (2 cœurs)</span></div>";
    html += "</div>";
    
    // --- CARTE 2: Mémoire Flash ---
    html += "<div class='card'>";
    html += "<div class='card-title'>💾 Flash</div>";
    html += "<div class='card-item'><span class='card-label'>Taille:</span><span class='card-value'>" + String(flashSize / (1024 * 1024)) + " MB</span></div>";
    html += "<div class='card-item'><span class='card-label'>Vitesse:</span><span class='card-value'>" + String(flashSpeed) + " MHz</span></div>";
    html += "<div class='card-item'><span class='card-label'>Mode:</span><span class='card-value'>QIO</span></div>";
    html += "</div>";
    
    // --- CARTE 3: RAM (Heap) ---
    html += "<div class='card'>";
    html += "<div class='card-title'>📊 RAM (Heap)</div>";
    uint32_t heapUsed = heapSize - freeHeap;
    float heapPercent = (float)heapUsed / heapSize * 100;
    html += "<div class='card-item'><span class='card-label'>Total:</span><span class='card-value'>" + String(heapSize / 1024) + " KB</span></div>";
    html += "<div class='card-item'><span class='card-label'>Libre:</span><span class='card-value'>" + String(freeHeap / 1024) + " KB</span></div>";
    html += "<div class='progress-container'>";
    html += "<div class='progress-label'><span>Utilisation</span><span>" + String(heapPercent, 1) + "%</span></div>";
    html += "<div class='progress-bar'><div class='progress-fill' style='width:" + String(heapPercent) + "%;'></div></div>";
    html += "</div></div>";
    
    // --- CARTE 4: PSRAM (si disponible) ---
    if(psramSize > 0) {
        uint32_t psramUsed = psramSize - freePsram;
        float psramPercent = (float)psramUsed / psramSize * 100;
        html += "<div class='card'>";
        html += "<div class='card-title'>🔋 PSRAM</div>";
        html += "<div class='card-item'><span class='card-label'>Total:</span><span class='card-value'>" + String(psramSize / (1024 * 1024)) + " MB</span></div>";
        html += "<div class='card-item'><span class='card-label'>Libre:</span><span class='card-value'>" + String(freePsram / 1024) + " KB</span></div>";
        html += "<div class='progress-container'>";
        html += "<div class='progress-label'><span>Utilisation</span><span>" + String(psramPercent, 1) + "%</span></div>";
        html += "<div class='progress-bar'><div class='progress-fill' style='width:" + String(psramPercent) + "%;'></div></div>";
        html += "</div></div>";
    }
    
    // --- CARTE 5: WiFi ---
    html += "<div class='card'>";
    html += "<div class='card-title'>📶 Réseau WiFi</div>";
    html += "<div class='card-item'><span class='card-label'>SSID:</span><span class='card-value'>" + WiFi.SSID() + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>IP:</span><span class='card-value mono'>" + WiFi.localIP().toString() + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>MAC:</span><span class='card-value mono'>" + WiFi.macAddress() + "</span></div>";
    int rssi = WiFi.RSSI();
    int signalPercent = (rssi + 100) * 2;
    signalPercent = (signalPercent > 100) ? 100 : (signalPercent < 0) ? 0 : signalPercent;
    html += "<div class='card-item'><span class='card-label'>Signal:</span><span class='card-value'>" + String(rssi) + " dBm</span></div>";
    html += "<div class='progress-container'>";
    html += "<div class='progress-label'><span>Force signal</span><span>" + String(signalPercent) + "%</span></div>";
    html += "<div class='progress-bar'><div class='progress-fill' style='width:" + String(signalPercent) + "%;'></div></div>";
    html += "</div></div>";
    
    // --- CARTE 6: Système ---
    html += "<div class='card'>";
    html += "<div class='card-title'>⏱️ Système</div>";
    unsigned long uptime = millis() / 1000;
    unsigned long hours = uptime / 3600;
    unsigned long minutes = (uptime % 3600) / 60;
    unsigned long seconds = uptime % 60;
    html += "<div class='card-item'><span class='card-label'>Uptime:</span><span class='card-value'>" + String(hours) + "h " + String(minutes) + "m " + String(seconds) + "s</span></div>";
    html += "<div class='card-item'><span class='card-label'>Température:</span><span class='card-value'>" + String(temperatureRead(), 1) + " °C</span></div>";
    html += "</div>";
    
    html += "</div>";  // Fin cards-grid
    
    // --- CARTE 7: Réseau Détaillé ---
    html += "<div class='card'>";
    html += "<div class='card-title'>🔗 Détails Réseau</div>";
    html += "<div class='card-item'><span class='card-label'>Masque:</span><span class='card-value mono'>" + WiFi.subnetMask().toString() + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>Passerelle:</span><span class='card-value mono'>" + WiFi.gatewayIP().toString() + "</span></div>";
    html += "<div class='card-item'><span class='card-label'>DNS:</span><span class='card-value mono'>" + WiFi.dnsIP().toString() + "</span></div>";
    html += "</div>";
    
    // --- BOUTONS D'ACTION ---
    html += "<div class='actions'>";
    html += "<button onclick='location.reload()'>🔄 Actualiser</button>";
    html += "<button onclick='location.href=\"/ota\"'>📡 OTA Update</button>";
    html += "<button class='danger' id='rebootBtn' onclick='rebootDevice()'>🔴 Redémarrer</button>";
    html += "<div id='rebootStatus' style='margin-top:10px;display:none;'></div>";
    html += "</div>";

    html += "<script>";
    html += "function rebootDevice(){";
    html += "document.getElementById('rebootBtn').disabled=true;";
    html += "var status=document.getElementById('rebootStatus');";
    html += "status.style.display='block';";
    html += "status.style.color='orange';";
    html += "status.textContent='⏳ Redémarrage en cours...';";
    html += "fetch('/reboot',{method:'POST'}).then(()=>{";
    html += "status.style.color='green';";
    html += "status.textContent='✅ Redémarrage effectué. Reconnexion dans 10s...';";
    html += "setTimeout(()=>location.reload(),10000);";
    html += "}).catch(()=>{";
    html += "status.style.color='red';";
    html += "status.textContent='❌ Erreur de communication.';";
    html += "document.getElementById('rebootBtn').disabled=false;";
    html += "});";
    html += "}";
    html += "</script>";

    html += "</div></body></html>";

    return html;
}

/**
 * @brief Génère la page d'upload OTA
 * @return String contenant le HTML de la page OTA
 */
String generateOTAPage() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta charset='utf-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>OTA Update - " + String(PROJECT_NAME) + "</title>";
    html += "<style>" + String(WEB_STYLES) + "</style>";
    html += "<style>";
    html += ".upload-zone{border:2px dashed #667eea;border-radius:10px;padding:40px;text-align:center;margin:20px 0;background:#f8f9ff;}";
    html += ".upload-zone:hover{background:#e8e9ff;border-color:#764ba2;}";
    html += "input[type=file]{display:none;}";
    html += ".file-label{background:linear-gradient(135deg,#667eea,#764ba2);color:white;padding:12px 30px;border-radius:6px;cursor:pointer;display:inline-block;font-weight:600;}";
    html += ".file-label:hover{transform:scale(1.05);box-shadow:0 4px 12px rgba(102,126,234,0.4);}";
    html += ".status-msg{margin-top:15px;padding:10px;border-radius:6px;display:none;}";
    html += ".status-msg.info{background:#e3f2fd;color:#1976d2;border-left:4px solid #1976d2;}";
    html += ".status-msg.success{background:#e8f5e9;color:#388e3c;border-left:4px solid #388e3c;}";
    html += ".status-msg.error{background:#ffebee;color:#d32f2f;border-left:4px solid #d32f2f;}";
    html += ".status-msg.warning{background:#fff3e0;color:#f57c00;border-left:4px solid #f57c00;}";
    html += "</style>";
    html += "</head><body>";
    html += "<div class='container'>";

    // Header
    html += "<div class='header'>";
    html += "<h1>📡 OTA Update</h1>";
    html += "<p>" + String(PROJECT_NAME) + " v" + String(PROJECT_VERSION) + "</p>";
    html += "</div>";

    // Upload form
    html += "<div class='card'>";
    html += "<div class='card-title'>Upload Firmware</div>";
    html += "<form id='uploadForm' enctype='multipart/form-data'>";
    html += "<div class='upload-zone'>";
    html += "<p style='font-size:48px;margin:10px 0;'>📦</p>";
    html += "<h3>Select Firmware File</h3>";
    html += "<p style='color:#666;margin:10px 0;'>Choose a .bin file to upload</p>";
    html += "<input type='file' id='fileInput' name='update' accept='.bin'>";
    html += "<label for='fileInput' class='file-label'>Choose File</label>";
    html += "<p id='fileName' style='margin-top:15px;font-weight:600;color:#667eea;'></p>";
    html += "</div>";

    // Status messages (inline validation)
    html += "<div id='statusMsg' class='status-msg'></div>";
    html += "<div id='progressBar' style='display:none;margin-top:15px;'>";
    html += "<div class='progress-label'><span>Upload Progress</span><span id='progressPercent'>0%</span></div>";
    html += "<div class='progress-bar'><div id='progressFill' class='progress-fill' style='width:0%;'></div></div>";
    html += "</div>";

    html += "<button type='submit' id='uploadBtn' class='file-label' style='margin-top:20px;width:100%;display:none;'>Upload Firmware</button>";
    html += "</form>";
    html += "</div>";

    // Back button
    html += "<div class='actions'>";
    html += "<button onclick='location.href=\"/\"'>← Back to Dashboard</button>";
    html += "</div>";

    // JavaScript
    html += "<script>";
    html += "const fileInput=document.getElementById('fileInput');";
    html += "const fileName=document.getElementById('fileName');";
    html += "const uploadBtn=document.getElementById('uploadBtn');";
    html += "const uploadForm=document.getElementById('uploadForm');";
    html += "const statusMsg=document.getElementById('statusMsg');";
    html += "const progressBar=document.getElementById('progressBar');";
    html += "const progressFill=document.getElementById('progressFill');";
    html += "const progressPercent=document.getElementById('progressPercent');";

    html += "fileInput.addEventListener('change',function(){";
    html += "if(this.files.length>0){";
    html += "const file=this.files[0];";
    html += "fileName.textContent=file.name+' ('+Math.round(file.size/1024)+' KB)';";
    html += "uploadBtn.style.display='block';";
    html += "statusMsg.style.display='none';";
    html += "if(!file.name.endsWith('.bin')){";
    html += "showStatus('warning','⚠️ Warning: File should be a .bin firmware file');";
    html += "}";
    html += "}";
    html += "});";

    html += "uploadForm.addEventListener('submit',function(e){";
    html += "e.preventDefault();";
    html += "if(fileInput.files.length===0){";
    html += "showStatus('error','❌ Please select a file first');";
    html += "return;";
    html += "}";
    html += "uploadBtn.disabled=true;";
    html += "fileInput.disabled=true;";
    html += "const formData=new FormData();";
    html += "formData.append('update',fileInput.files[0]);";
    html += "progressBar.style.display='block';";
    html += "showStatus('info','⏳ Uploading firmware...');";

    html += "const xhr=new XMLHttpRequest();";
    html += "xhr.upload.addEventListener('progress',function(e){";
    html += "if(e.lengthComputable){";
    html += "const percent=Math.round((e.loaded/e.total)*100);";
    html += "progressFill.style.width=percent+'%';";
    html += "progressPercent.textContent=percent+'%';";
    html += "}";
    html += "});";

    html += "xhr.addEventListener('load',function(){";
    html += "if(xhr.status===200){";
    html += "showStatus('success','✅ Upload successful! Device rebooting...');";
    html += "setTimeout(()=>{location.href='/';},10000);";
    html += "}else{";
    html += "showStatus('error','❌ Upload failed. Please try again.');";
    html += "uploadBtn.disabled=false;";
    html += "fileInput.disabled=false;";
    html += "}";
    html += "});";

    html += "xhr.addEventListener('error',function(){";
    html += "showStatus('error','❌ Network error. Check connection and try again.');";
    html += "uploadBtn.disabled=false;";
    html += "fileInput.disabled=false;";
    html += "});";

    html += "xhr.open('POST','/update');";
    html += "xhr.send(formData);";
    html += "});";

    html += "function showStatus(type,msg){";
    html += "statusMsg.className='status-msg '+type;";
    html += "statusMsg.textContent=msg;";
    html += "statusMsg.style.display='block';";
    html += "}";
    html += "</script>";

    html += "</div></body></html>";
    return html;
}

/**
 * @brief Génère la page de résultat OTA
 * @param success true si l'upload a réussi, false sinon
 * @param errorMsg Message d'erreur (vide si succès)
 * @return String contenant le HTML de la page de résultat
 */
String generateOTAResultPage(bool success, const char* errorMsg) {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta charset='utf-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>OTA Result - " + String(PROJECT_NAME) + "</title>";
    html += "<style>" + String(WEB_STYLES) + "</style>";
    html += "</head><body>";
    html += "<div class='container'>";

    html += "<div class='header'>";
    if (success) {
        html += "<h1 style='color:#4CAF50;'>✅ Update Successful</h1>";
        html += "<p>Device is rebooting...</p>";
    } else {
        html += "<h1 style='color:#f44336;'>❌ Update Failed</h1>";
        html += "<p>Error: " + String(errorMsg) + "</p>";
    }
    html += "</div>";

    html += "<div class='card'>";
    if (success) {
        html += "<p>The firmware has been successfully updated.</p>";
        html += "<p>The device will reboot automatically in a few seconds.</p>";
        html += "<p>Please wait for reconnection...</p>";
    } else {
        html += "<p>The firmware update failed with the following error:</p>";
        html += "<p style='color:#f44336;font-weight:600;'>" + String(errorMsg) + "</p>";
        html += "<p>Please check your firmware file and try again.</p>";
    }
    html += "</div>";

    html += "<div class='actions'>";
    if (success) {
        html += "<script>setTimeout(()=>location.href='/',10000);</script>";
    } else {
        html += "<button onclick='location.href=\"/ota\"'>← Try Again</button>";
        html += "<button onclick='location.href=\"/\"'>Home</button>";
    }
    html += "</div>";

    html += "</div></body></html>";
    return html;
}

#endif // WEB_PAGES_H
