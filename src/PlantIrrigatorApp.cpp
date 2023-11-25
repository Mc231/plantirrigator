// PlantIrrigatorApp.cpp
#include "PlantIrrigatorApp.h"
#include "MDNSBroadcaster.h"
#include "MqttManagerImpl.h"

PlantIrrigatorApp::PlantIrrigatorApp() 
    : wifiConfigManager(fileSystem), 
    mqttConfigManager(fileSystem),
    mqttManager(new MqttManagerImpl(mqttConfigManager)),
    setupManager(new WiFiSetupManager(wifiConfigManager)), 
    irrigatorManager(wifiConfigManager, mqttConfigManager),
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
        irrigatorManager.handleClient();
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
    irrigatorManager.begin();
    hasMqttConfig = mqttManager->hasConfig();
    if (hasMqttConfig) {
        mqttManager->connect();
    }
}
