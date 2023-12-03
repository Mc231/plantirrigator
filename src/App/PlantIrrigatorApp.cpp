// PlantIrrigatorApp.cpp
#include "PlantIrrigatorApp.h"
#include "PostSetupBroadcaster/MDNSBroadcaster.h"
#include "Mqtt/MqttManagerImpl.h"
#include "Sensor/MoistureSensorImpl.h"
#include "Sensor/RelaySensorImpl.h"


PlantIrrigatorApp::PlantIrrigatorApp() 
    : wifiConfigManager(fileSystem), 
    mqttConfigManager(fileSystem),
    mqttManager(new MqttManagerImpl(mqttConfigManager)),
    moistureSensor(new MoistureSensorImpl(12,A0)),
    relaySensor(new RelaySensorImpl(13)),
    irrigatorController(wifiConfigManager, mqttConfigManager, moistureSensor, relaySensor, mqttManager),
    stateNotifier(mqttManager, mqttConfigManager, irrigatorController),
    setupManager(new WiFiSetupManager(wifiConfigManager)), 
    irrigatorWebServer(irrigatorController),
    postSetupBroadcaster(new MDNSBroadcaster()),
    isSetupComplete(false),
    hasMqttConfig(false)
     {}

void PlantIrrigatorApp::setup() {
    Serial.begin(115200);
    moistureSensor->begin();
    initializeWiFi();
}

void PlantIrrigatorApp::loop() {
    if (!isSetupComplete) {
        setupManager->handleClient();
    } else {
        irrigatorWebServer.handleClient();
        postSetupBroadcaster->loop();
        if (hasMqttConfig && mqttManager->isConnected()) {
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
        stateNotifier.notifyByTimeout(STATE_PUBLISH_DELAY);
    }
}
