// WiFiSetupManager.h
#ifndef WiFiSetupManager_h
#define WiFiSetupManager_h

#include "AbstractWiFiSetupManager.h"
#include "WiFiConfigManager.h"
#include <DNSServer.h>
#include <ESP8266WebServer.h>

class WiFiSetupManager : public AbstractWiFiSetupManager {
public:
    WiFiSetupManager(WiFiConfigManager& configManager);

    void initialize(SetupCompleteCallback callback) override;
    void handleClient() override;
    void stopServices() override;

private:
    WiFiConfigManager& configManager;
    DNSServer dnsServer;
    ESP8266WebServer webServer;
    SetupCompleteCallback setupCompleteCallback;
    void startAPMode();
    void setupCaptivePortal();
    void handleRoot();
    void handleScanNetworks();
    void handleConnectToNetwork();
    void handleNotFound();
};

#endif
