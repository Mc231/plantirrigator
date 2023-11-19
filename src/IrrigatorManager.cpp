// IrrigatorManager.cpp
#include "IrrigatorManager.h"
#include <ESP8266WiFi.h> // For WiFi functionalities

IrrigatorManager::IrrigatorManager(WiFiConfigManager& cm)
    : configManager(cm), webServer(80) {}

void IrrigatorManager::begin() {
    webServer.on("/status", HTTP_GET, std::bind(&IrrigatorManager::handleStatus, this));
    webServer.on("/wifi", HTTP_GET, std::bind(&IrrigatorManager::handleWiFi, this));
    webServer.on("/wifi/forget", HTTP_POST, std::bind(&IrrigatorManager::handleWiFiForget, this));
    webServer.on("/", HTTP_GET, std::bind(&IrrigatorManager::handleRoot, this));

    webServer.begin();
}

void IrrigatorManager::handleClient() {
    webServer.handleClient();
}

void IrrigatorManager::handleStatus() {
    // For now, just return an empty object or a simple message
    webServer.send(200, "application/json", "{}");
}

void IrrigatorManager::handleWiFi() {
    auto config = configManager.readConfig();
    String response = "{\"ssid\":\"" + config.ssid + "\"}";
    webServer.send(200, "application/json", response);
}

void IrrigatorManager::handleWiFiForget() {
    configManager.clearConfig();
    webServer.send(200, "text/plain", "Wi-Fi config forgotten. Restarting...");
    delay(1000);
    ESP.restart(); // Restart the ESP8266
}

void IrrigatorManager::handleRoot() {
    String html = "<html><body style='text-align: center;'>"
                  "<h1>Irrigator App</h1>"
                  "<div>Wi-Fi: " + WiFi.SSID() + "</div>"
                  "<form action='/wifi/forget' method='post'>"
                  "<button type='submit'>Forget Wi-Fi</button>"
                  "</form></body></html>";
    webServer.send(200, "text/html", html);
}
