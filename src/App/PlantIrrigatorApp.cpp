// PlantIrrigatorApp.cpp
#include "PlantIrrigatorApp.h"
#include "PostSetupBroadcaster/MDNSBroadcaster.h"
#include "Mqtt/MqttManagerImpl.h"

PlantIrrigatorApp::PlantIrrigatorApp() 
    : wifiConfigManager(fileSystem), 
    mqttConfigManager(fileSystem),
    mqttManager(new MqttManagerImpl(mqttConfigManager)),
    irrigatorController(wifiConfigManager, mqttConfigManager),
    setupManager(new WiFiSetupManager(wifiConfigManager)), 
    irrigatorWebServer(irrigatorController),
    postSetupBroadcaster(new MDNSBroadcaster()),
    isSetupComplete(false),
    hasMqttConfig(false)
     {}

void PlantIrrigatorApp::setup() {
    Serial.begin(115200);
    initializeWiFi();
}

void PlantIrrigatorApp::loop() {
    if (!isSetupComplete) {
        setupManager->handleClient();
    } else {
        irrigatorWebServer.handleClient();
        postSetupBroadcaster->loop();
        if (hasMqttConfig) {
            mqttManager->loop();
        }
    }
}

void PlantIrrigatorApp::initializeWiFi() {
    setupManager->initialize(std::bind(&PlantIrrigatorApp::setupCompleted, this));
}

void PlantIrrigatorApp::setupCompleted() {
    setupManager->stopServices();
    Serial.println("Wi-Fi connected. Setup completed!");
    isSetupComplete = true;
    postSetupBroadcaster->begin();
    irrigatorWebServer.begin();
    hasMqttConfig = mqttManager->hasConfig();
    if (hasMqttConfig) {
        mqttManager->connect();
    }
}
