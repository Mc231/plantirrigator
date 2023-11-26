// IrrigatorController.h
#ifndef IrrigatorController_h
#define IrrigatorController_h

#include "WiFi/WiFiConfigManager.h"
#include "Mqtt/MqttConfigManager.h"

class IrrigatorController {
public:
    IrrigatorController(WiFiConfigManager& configManager, MqttConfigManager& mqttConfigManager);
    String getStatus();
    String getWiFiConfig();
    String getMqttConfig();
    void setMqttConfig(const String& server, const String& port, const String& username, const String& password, String& statusTopic);
    void forgotWiFi();
    void reboot();
    void restoreConfig();

private:
    WiFiConfigManager& wiFiConfigManager;
    MqttConfigManager& mqttConfigManager;
};

#endif
