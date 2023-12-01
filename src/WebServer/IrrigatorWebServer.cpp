// IrrigatorWebServer.cpp
#include "IrrigatorWebServer.h"
#include "IrrigatorConfigHTML.h"

IrrigatorWebServer::IrrigatorWebServer(IrrigatorController& irrigatorController)
    : irrigatorController(irrigatorController) {}

void IrrigatorWebServer::begin() {
    webServer.on("/status", HTTP_GET, std::bind(&IrrigatorWebServer::handleGetStatus, this));
    webServer.on("/wifi", HTTP_GET, std::bind(&IrrigatorWebServer::handleGetWiFi, this));
    webServer.on("/wifi/forget", HTTP_POST, std::bind(&IrrigatorWebServer::handleWiFiForget, this));
    webServer.on("/mqtt", HTTP_GET, std::bind(&IrrigatorWebServer::handleGetMQTT, this));
    webServer.on("/mqtt", HTTP_POST, std::bind(&IrrigatorWebServer::handleSetMQTT, this));
    webServer.on("/moisture", HTTP_GET, std::bind(&IrrigatorWebServer::handleGetMoistureLevel, this));
    webServer.on("/relay", HTTP_GET, std::bind(&IrrigatorWebServer::handleGetRelay, this));
    webServer.on("/relay", HTTP_POST, std::bind(&IrrigatorWebServer::handleToggleRelay, this));
    webServer.on("/sensors", HTTP_GET, std::bind(&IrrigatorWebServer::handleGetSensors, this));
    webServer.on("/reboot", HTTP_POST, std::bind(&IrrigatorWebServer::handleReboot, this));
    webServer.on("/", HTTP_GET, std::bind(&IrrigatorWebServer::handleRoot, this));
    webServer.on("/restore", HTTP_POST, std::bind(&IrrigatorWebServer::handleRestore, this));
    webServer.begin();
}

void IrrigatorWebServer::handleClient() {
    webServer.handleClient();
}

void IrrigatorWebServer::handleGetStatus() {
    String status = irrigatorController.getStatus();
    webServer.send(200, "application/json", status);
}

void IrrigatorWebServer::handleGetWiFi() {
    String response = irrigatorController.getWiFiConfig();
    webServer.send(200, "application/json", response);
}

void IrrigatorWebServer::handleWiFiForget() {
    irrigatorController.forgotWiFi();
    webServer.send(200, "text/plain", "Wi-Fi config forgotten. Restarting...");
    irrigatorController.reboot();
}

void IrrigatorWebServer::handleReboot() {
    webServer.send(200, "text/plain", "Rebooting");
    irrigatorController.reboot();
}

void IrrigatorWebServer::handleGetMQTT() {
    String response = irrigatorController.getMqttConfig();
    webServer.send(200, "application/json", response);
}

void IrrigatorWebServer::handleGetMoistureLevel() {
    int level = irrigatorController.getMoistureLevel(10);
    String response = "{\"moisture\":" + String(level) + "}";
    webServer.send(200, "application/json", response); 
}

void IrrigatorWebServer::handleGetRelay() {
    bool state = irrigatorController.getRelayState();
    String response = "{\"relay\":" + String(state) + "}";
    webServer.send(200, "application/json", response); 
}

void IrrigatorWebServer::handleToggleRelay() {
    irrigatorController.toggleRelay();
    webServer.send(200, "text/plain", "Toggled");
}

void IrrigatorWebServer::handleGetSensors() {
    String response = irrigatorController.getMqttConfig();
    webServer.send(200, "text/plain", response);
}

void IrrigatorWebServer::handleSetMQTT() {
    String server = webServer.arg("server");
    String port = webServer.arg("port");
    String username = webServer.arg("username");
    String password = webServer.arg("password");
    String statusTopic = webServer.arg("status_topic");
    irrigatorController.setMqttConfig(server, port, username, password, statusTopic);
    webServer.send(200, "text/plain", "MQTT Configuration updated rebooting");
    irrigatorController.reboot();
}

void IrrigatorWebServer::handleRestore() {
    irrigatorController.restoreConfig();
    webServer.send(200, "text/plain", "Restored rebooting");
    irrigatorController.reboot();
}

void IrrigatorWebServer::handleRoot() {
    webServer.send(200, "text/html", irrigatorConfigHTML);
}
