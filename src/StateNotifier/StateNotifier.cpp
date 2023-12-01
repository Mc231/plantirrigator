// StateNotifier.cpp
#include "StateNotifier.h"
#include <Arduino.h>

StateNotifier::StateNotifier(MqttManager* mqttManager, MqttConfigManager& mqttConfigManager, IrrigatorController& irrigatorController)
    : mqttManager(mqttManager), mqttConfigManager(mqttConfigManager), irrigatorController(irrigatorController) {}

void StateNotifier::notifyStatus() {
	if (mqttManager->isConnected()) 
    {
        auto topic = mqttConfigManager.readConfig().statusTopic.c_str();
        auto state = irrigatorController.getStatus().c_str();
        mqttManager->publish(topic, state);
    }
}
