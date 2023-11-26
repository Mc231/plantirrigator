// IrrigatorWebServer.cpp
#include "IrrigatorWebServer.h"

IrrigatorWebServer::IrrigatorWebServer(IrrigatorController& irrigatorController)
    : irrigatorController(irrigatorController) {}

void IrrigatorWebServer::begin() {
    webServer.on("/status", HTTP_GET, std::bind(&IrrigatorWebServer::handleGetStatus, this));
    webServer.on("/wifi", HTTP_GET, std::bind(&IrrigatorWebServer::handleGetWiFi, this));
    webServer.on("/wifi/forget", HTTP_POST, std::bind(&IrrigatorWebServer::handleWiFiForget, this));
    webServer.on("/mqtt", HTTP_GET, std::bind(&IrrigatorWebServer::handleGetMQTT, this));
    webServer.on("/mqtt", HTTP_POST, std::bind(&IrrigatorWebServer::handleSetMQTT, this));
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
    irrigatorController.reboot();
}

void IrrigatorWebServer::handleGetMQTT() {
    String response = irrigatorController.getMqttConfig();
    webServer.send(200, "application/json", response);
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
    String html = "<html><body style='text-align: center;'>"
                  "<h1>Irrigator App</h1>"
                  "<div id='wifiSSID'></div>"
                  "<form action='/wifi/forget' method='post'>"
                  "<button type='submit'>Forget Wi-Fi</button>"
                  "</form>"
                  "<h2>MQTT Configuration</h2>"
                  "<form action='/mqtt' method='post'>"
                  "Server: <input type='text' id='mqttServer' name='server'><br>"
                  "Port: <input type='text' id='mqttPort' name='port'><br>"
                  "Username: <input type='text' id='mqttUsername' name='username'><br>"
                  "Password: <input type='password' id='mqttPassword' name='password'><br>"
                  "Status Topic: <input type='text' id='mqttStatusTopic' name='status_topic'><br>"
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
                  "  document.getElementById('mqttPort').value = data.port || '';"
                  "  document.getElementById('mqttUsername').value = data.username || '';"
                  "  document.getElementById('mqttPassword').value = data.password || '';"
                  "  document.getElementById('mqttStatusTopic').value = data.status_topic || '';"
                  "}).catch(error => console.error('Error:', error));"
                  "</script>"
                  "</body></html>";
    webServer.send(200, "text/html", html);
}
