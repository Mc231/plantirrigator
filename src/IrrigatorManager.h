// IrrigatorManager.h
#ifndef IrrigatorManager_h
#define IrrigatorManager_h

#include <ESP8266WebServer.h>
#include "WiFiConfigManager.h"

class IrrigatorManager {
public:
    IrrigatorManager(WiFiConfigManager& configManager);
    void begin();
    void handleClient();

private:
    WiFiConfigManager& configManager;
    ESP8266WebServer webServer;

    void handleStatus();
    void handleWiFi();
    void handleWiFiForget();
    void handleRoot();
};

#endif
