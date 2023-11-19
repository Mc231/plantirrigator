// PlantIrrigatorApp.cpp
#include "PlantIrrigatorApp.h"

PlantIrrigatorApp::PlantIrrigatorApp() 
    : configManager(fileSystem), 
    setupManager(new WiFiSetupManager(configManager)), 
    irrigatorManager(configManager),
    isSetupComplete(false) {}

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
    Serial.println("Wi-Fi connected. Setup completed!");
    isSetupComplete = true;
    irrigatorManager.begin();
}
