// IrrigatorManager.h
#ifndef IrrigatorManager_h
#define IrrigatorManager_h

#include <ESP8266WebServer.h>
#include "WiFiConfigManager.h"
#include "MqttConfigManager.h"

class IrrigatorManager {
public:
    IrrigatorManager(WiFiConfigManager& configManager, MqttConfigManager& mqttConfigManager);
    void begin();
    void handleClient();

private:
    WiFiConfigManager& wiFiConfigManager;
    MqttConfigManager& mqttConfigManager;
    ESP8266WebServer webServer;
    void handleReboot();
    void handleStatus();
    void handleWiFi();
    void handleWiFiForget();
    void handleSetMQTT();
    void handleGetMQTT();
    void handleRoot();
    void handleRestore();
};

#endif
