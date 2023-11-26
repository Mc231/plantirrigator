// IrrigatorWebServer.h
#ifndef IrrigatorWebServer_h
#define IrrigatorWebServer_h

#include <ESP8266WebServer.h>
#include "Controller/IrrigatorController.h"

class IrrigatorWebServer {
public:
    IrrigatorWebServer(IrrigatorController& irrigatorController);
    void begin();
    void handleClient();

private:
    IrrigatorController& irrigatorController;
    ESP8266WebServer webServer;
    void handleReboot();
    void handleGetStatus();
    void handleGetWiFi();
    void handleWiFiForget();
    void handleSetMQTT();
    void handleGetMQTT();
    void handleRoot();
    void handleRestore();
};

#endif
