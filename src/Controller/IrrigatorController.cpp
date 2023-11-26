// IrrigatorController.cpp
#include "IrrigatorController.h"

IrrigatorController::IrrigatorController(WiFiConfigManager& cm, MqttConfigManager& mm)
    : wiFiConfigManager(cm), mqttConfigManager(mm) {}

String IrrigatorController::getStatus() {
    // Fetch Wi-Fi SSID
    auto wifiConfig = wiFiConfigManager.readConfig();
    String wifiSSID = wifiConfig.ssid;

    // Fetch MQTT Config
    auto mqttConfig = mqttConfigManager.readConfig();
    String mqttURL = mqttConfig.server;
    String mqttPort = mqttConfig.port;
    String mqttStatusTopic = mqttConfig.statusTopic;

    // Construct JSON response
    String jsonResponse = "{";
    jsonResponse += "\"ssid\": \"" + wifiSSID + "\",";
    jsonResponse += "\"mqtt_url\": \"" + mqttURL + "\"";
    jsonResponse += "\"mqtt_port\": \"" + mqttPort + "\"";
    jsonResponse += "\"mqtt_status_topic\": \"" + mqttStatusTopic + "\"";
    jsonResponse += "}";

    return jsonResponse;
}

String IrrigatorController::getWiFiConfig() {
    auto config = wiFiConfigManager.readConfig();
    return "{\"ssid\":\"" + config.ssid + "\"}";
}

String IrrigatorController::getMqttConfig() {
    auto config = mqttConfigManager.readConfig();
    return "{\"server\":\"" + config.server + 
            "\", \"port\":\"" + config.port + 
            "\", \"username\":\"" + config.username + 
            "\", \"password\":\"" + config.password +
            "\", \"status_topic\":\"" + config.statusTopic +  "\"}";
}

void IrrigatorController::setMqttConfig(const String& server, const String& port, const String& username, const String& password, String& statusTopic) {
    mqttConfigManager.saveConfig(server, port, username, password, statusTopic);
}

void IrrigatorController::forgotWiFi() {
    wiFiConfigManager.clearConfig();
}

void IrrigatorController::reboot() {
    delay(1000);
    ESP.restart();
}

void IrrigatorController::restoreConfig() {
    wiFiConfigManager.clearConfig();
    mqttConfigManager.clearConfig();
}