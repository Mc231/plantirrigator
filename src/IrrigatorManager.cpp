// IrrigatorManager.cpp
#include "IrrigatorManager.h"
#include <ESP8266WiFi.h> // For WiFi functionalities

IrrigatorManager::IrrigatorManager(WiFiConfigManager& cm, MqttConfigManager& mm)
    : wiFiConfigManager(cm), mqttConfigManager(mm), webServer(80) {}

void IrrigatorManager::begin() {
    webServer.on("/status", HTTP_GET, std::bind(&IrrigatorManager::handleStatus, this));
    webServer.on("/wifi", HTTP_GET, std::bind(&IrrigatorManager::handleWiFi, this));
    webServer.on("/wifi/forget", HTTP_POST, std::bind(&IrrigatorManager::handleWiFiForget, this));
    webServer.on("/mqtt", HTTP_GET, std::bind(&IrrigatorManager::handleGetMQTT, this));
    webServer.on("/mqtt", HTTP_POST, std::bind(&IrrigatorManager::handleSetMQTT, this));
    webServer.on("/reboot", HTTP_POST, std::bind(&IrrigatorManager::handleReboot, this));
    webServer.on("/", HTTP_GET, std::bind(&IrrigatorManager::handleRoot, this));
    webServer.on("/restore", HTTP_POST, std::bind(&IrrigatorManager::handleRestore, this));
    webServer.begin();
}

void IrrigatorManager::handleClient() {
    webServer.handleClient();
}

void IrrigatorManager::handleStatus() {
    // Fetch Wi-Fi SSID
    auto wifiConfig = wiFiConfigManager.readConfig();
    String wifiSSID = wifiConfig.ssid;

    // Fetch MQTT Config
    auto mqttConfig = mqttConfigManager.readConfig();
    String mqttURL = mqttConfig.server;

    // Construct JSON response
    String jsonResponse = "{";
    jsonResponse += "\"ssid\": \"" + wifiSSID + "\",";
    jsonResponse += "\"mqtt_url\": \"" + mqttURL + "\"";
    jsonResponse += "}";

    webServer.send(200, "application/json", jsonResponse);
}

void IrrigatorManager::handleWiFi() {
    auto config = wiFiConfigManager.readConfig();
    String response = "{\"ssid\":\"" + config.ssid + "\"}";
    webServer.send(200, "application/json", response);
}

void IrrigatorManager::handleWiFiForget() {
    wiFiConfigManager.clearConfig();
    webServer.send(200, "text/plain", "Wi-Fi config forgotten. Restarting...");
    handleReboot();
}

void IrrigatorManager::handleReboot() {
    delay(1000);
    ESP.restart();
}

void IrrigatorManager::handleGetMQTT() {
    auto config = mqttConfigManager.readConfig();
    String response = "{\"server\":\"" + config.server + 
                      "\", \"username\":\"" + config.username + 
                      "\", \"password\":\"" + config.password + "\"}";
    webServer.send(200, "application/json", response);
}

void IrrigatorManager::handleSetMQTT() {
    String server = webServer.arg("server");
    String username = webServer.arg("username");
    String password = webServer.arg("password");
    mqttConfigManager.saveConfig(server, username, password);
    webServer.send(200, "text/plain", "MQTT Configuration updated rebooting");
    handleReboot();
}

void IrrigatorManager::handleRestore() {
    wiFiConfigManager.clearConfig();
    mqttConfigManager.clearConfig();
    webServer.send(200, "text/plain", "Restored rebooting");
    handleReboot();
}

void IrrigatorManager::handleRoot() {
    String html = "<html><body style='text-align: center;'>"
                  "<h1>Irrigator App</h1>"
                  "<div id='wifiSSID'></div>"
                  "<form action='/wifi/forget' method='post'>"
                  "<button type='submit'>Forget Wi-Fi</button>"
                  "</form>"
                  "<h2>MQTT Configuration</h2>"
                  "<form action='/mqtt' method='post'>"
                  "Server: <input type='text' id='mqttServer' name='server'><br>"
                  "Username: <input type='text' id='mqttUsername' name='username'><br>"
                  "Password: <input type='password' id='mqttPassword' name='password'><br>"
                  "<button type='submit'>Save Changes</button>"
                 "</form>"
                  "<form action='/reboot' method='post'>"
                  "<button type='submit'>Reboot</button>"
                  "</form>"
                  "</form>"
                  "<form action='/restore' method='post'>"
                  "<button type='submit'>Restore</button>"
                  "</form>"
           
                  "<script>"
                  "fetch('/wifi').then(response => response.json()).then(data => {"
                  "  document.getElementById('wifiSSID').innerText = 'Wi-Fi: ' + data.ssid;"
                  "}).catch(error => console.error('Error:', error));"
                  "fetch('/mqtt').then(response => response.json()).then(data => {"
                  "  document.getElementById('mqttServer').value = data.server || '';"
                  "  document.getElementById('mqttUsername').value = data.username || '';"
                  "  document.getElementById('mqttPassword').value = data.password || '';"
                  "}).catch(error => console.error('Error:', error));"
                  "</script>"
                  "</body></html>";
    webServer.send(200, "text/html", html);
}
