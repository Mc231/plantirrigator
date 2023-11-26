// IrrigatorController.h
#ifndef IrrigatorController_h
#define IrrigatorController_h

#include "WiFi/WiFiConfigManager.h"
#include "Mqtt/MqttConfigManager.h"
#include "Sensor/MoistureSensor.h"

class IrrigatorController {
public:
    IrrigatorController(WiFiConfigManager& configManager, MqttConfigManager& mqttConfigManager, MoistureSensor* moistureSensor);
    String getStatus();
    String getWiFiConfig();
    String getMqttConfig();
    void setMqttConfig(const String& server, const String& port, const String& username, const String& password, String& statusTopic);
    void forgotWiFi();
    void reboot();
    void restoreConfig();
    int getMoistureLevel(int delay);

private:
    WiFiConfigManager& wiFiConfigManager;
    MqttConfigManager& mqttConfigManager;
    MoistureSensor* moistureSensor;
};

#endif
