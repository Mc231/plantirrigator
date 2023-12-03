// StateNotifier.cpp
#include "StateNotifier.h"
#include <Arduino.h>

StateNotifier::StateNotifier(MqttManager* mqttManager, MqttConfigManager& mqttConfigManager, IrrigatorController& irrigatorController)
    : mqttManager(mqttManager), mqttConfigManager(mqttConfigManager), irrigatorController(irrigatorController) {}

void StateNotifier::notifyStatus() {
  // Get the topic and state as Arduino Strings
    String topicStr = mqttConfigManager.readConfig().statusTopic;
    String stateStr = irrigatorController.getStatus();

    // Convert to C strings
    const char* topic = topicStr.c_str();
    const char* state = stateStr.c_str();
    mqttManager->publish(topic, state);
}


void StateNotifier::notifyByTimeout(int timeout) {
    if (mqttManager->isConnected())
    {
        timer.attach(timeout, [this] () { this->notifyStatus(); });
    }
}