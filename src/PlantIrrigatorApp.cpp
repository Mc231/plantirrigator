// PlantIrrigatorApp.cpp
#include "PlantIrrigatorApp.h"
#include "MDNSBroadcaster.h"

PlantIrrigatorApp::PlantIrrigatorApp() 
    : wifiConfigManager(fileSystem), 
    mqttConfigManager(fileSystem),
    setupManager(new WiFiSetupManager(wifiConfigManager)), 
    irrigatorManager(wifiConfigManager, mqttConfigManager),
    postSetupBroadcaster(new MDNSBroadcaster()),
    isSetupComplete(false)
     {}

void PlantIrrigatorApp::setup() {
    Serial.begin(115200);
    initializeWiFi();
}

void PlantIrrigatorApp::loop() {
    if (!isSetupComplete) {
        setupManager->handleClient();
    } else {
        irrigatorManager.handleClient();
        postSetupBroadcaster->loop();
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
    irrigatorManager.begin();
}
