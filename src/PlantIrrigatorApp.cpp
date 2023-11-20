// PlantIrrigatorApp.cpp
#include "PlantIrrigatorApp.h"
#include "MDNSBroadcaster.h"

PlantIrrigatorApp::PlantIrrigatorApp() 
    : configManager(fileSystem), 
    setupManager(new WiFiSetupManager(configManager)), 
    irrigatorManager(configManager),
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
    }
}

void PlantIrrigatorApp::initializeWiFi() {
    setupManager->initialize(std::bind(&PlantIrrigatorApp::setupCompleted, this));
}

void PlantIrrigatorApp::setupCompleted() {
    setupManager->stopServices();
    Serial.println("Wi-Fi connecte