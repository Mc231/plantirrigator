// PlantIrrigatorApp.h
#ifndef PlantIrrigatorApp_h
#define PlantIrrigatorApp_h

#include "WiFi/WiFiSetupManager.h"
#include "WiFi/WiFiConfigManager.h"
#include "Mqtt/MqttConfigManager.h"
#include "Fs/LittleFSImpl.h"
#include "WiFi/AbstractWiFiSetupManager.h"
#include "WebServer/IrrigatorWebServer.h"
#include "PostSetupBroadcaster/PostSetupAvailabilityBroadcaster.h"
#include "Mqtt/MqttManager.h"
#include "Controller/IrrigatorController.h"

class PlantIrrigatorApp {
public:
    PlantIrrigatorApp();
    void setup();
    void loop();

private:
    LittleFSImpl fileSystem;
    WiFiConfigManager wifiConfigManager;
    MqttConfigManager mqttConfigManager;
    IrrigatorController irrigatorController;
    MqttManager* mqttManager;
    AbstractWiFiSetupManager* setupManager;
    IrrigatorWebServer irrigatorWebServer;
    PostSetupAvailabilityBroadcaster* postSetupBroadcaster;
    bool isSetupComplete;
    bool hasMqttConfig;

    void setupCompleted();
    void initializeWiFi();
    // Add other methods and members relevant to your application
};

#endif
