// StateNotifier.h
#ifndef StateNotifier_h
#define StateNotifier_h

#include "Mqtt/MqttConfigManager.h"
#include "Mqtt/MqttManager.h"
#include "Controller/IrrigatorController.h"


class StateNotifier {
public:
    StateNotifier(MqttManager* mqttManager, MqttConfigManager& mqttConfigManager, IrrigatorController& irrigatorController);
    void notifyStatus();

private:
    MqttManager* mqttManager;
    MqttConfigManager& mqttConfigManager;
    IrrigatorController irrigatorController;
};

#endif
