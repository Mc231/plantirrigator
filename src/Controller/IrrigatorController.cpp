// IrrigatorController.cpp
#include "IrrigatorController.h"

IrrigatorController::IrrigatorController(WiFiConfigManager& cm, MqttConfigManager& mm, MoistureSensor* moistureSensor, RelaySensor* relaySensor, MqttManager* mqttManager)
    : wiFiConfigManager(cm), mqttConfigManager(mm), moistureSensor(moistureSensor), relaySensor(relaySensor), mqttManager(mqttManager) {}

String IrrigatorController::getStatus() {
    // Fetch Wi-Fi SSID
    auto wifiConfig = wiFiConfigManager.readConfig();
    String wifiSSID = wifiConfig.ssid;

    // Fetch MQTT Config
    auto mqttConfig = mqttConfigManager.readConfig();
    String mqttURL = mqttConfig.server;
    String mqttPort = mqttConfig.port;
    String mqttStatusTopic = mqttConfig.statusTopic;
    bool mqttConnected = mqttManager->isConnected();

    // Moisture
    int moisture = moistureSensor->getMoistureLevel(10);

    // Relay
    bool relay = relaySensor->getCurrentState();

    // Construct JSON response
    String jsonResponse = "{";
    jsonResponse += "\"ssid\": \"" + wifiSSID + "\",";
    jsonResponse += "\"mqtt_url\": \"" + mqttURL + "\",";
    jsonResponse += "\"mqtt_connection_status\": \"" + String(mqttConnected) + "\",";
    jsonResponse += "\"mqtt_port\": \"" + mqttPort + "\",";
    jsonResponse += "\"mqtt_status_topic\": \"" + mqttStatusTopic + "\",";
    jsonResponse += "\"moisture\": \"" + String(moisture) + "\",";
    jsonResponse += "\"relay\": \"" + String(relay) + "\"";
    jsonResponse += "}";
    return jsonResponse;
}

String IrrigatorController::getSensorsStatus() {
        // Moisture
    int moisture = moistureSensor->getMoistureLevel(10);
    // Relay
    bool relay = relaySensor->getCurrentState();
      String jsonResponse = "{";
    jsonResponse += "\"moisture\": \"" + String(moisture) + "\"";
    jsonResponse += "\"relay\": \"" + String(relay) + "\"";
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
            "\", \"is_connected\":\"" + String(mqttManager->isConnected()) +
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

int IrrigatorController::getMoistureLevel(int delay) {
    return moistureSensor->getMoistureLevel(delay);
}

bool IrrigatorController::getRelayState() {
    return relaySensor->getCurrentState();
}

void IrrigatorController::toggleRelay() {
    relaySensor->toggle();
}