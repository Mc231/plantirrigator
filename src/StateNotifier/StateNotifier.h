// StateNotifier.h
#ifndef StateNotifier_h
#define StateNotifier_h

#include "Mqtt/MqttConfigManager.h"
#include "Mqtt/MqttManager.h"
#include "Controller/IrrigatorController.h"
#include "Ticker.h"


class StateNotifier {
public:
    StateNotifier(MqttManager* mqttManager, MqttConfigManager& mqttConfigManager, IrrigatorController& irrigatorController);
    void notifyStatus();
    void notifyByTimeout(int timeout);
    void loop();

private:
    Ticker timer;
    MqttManager* mqttManager;
    MqttConfigManager& mqttConfigManager;
    IrrigatorController irrigatorController;
};

#endif
