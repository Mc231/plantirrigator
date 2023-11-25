// MqttManagerImpl.h
#ifndef MqttManagerImpl_h
#define MqttManagerImpl_h

#include "MqttManager.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "MqttConfigManager.h"

class MqttManagerImpl : public MqttManager  {
public:
    MqttManagerImpl(MqttConfigManager& configManager);
    void connect() override;
    void loop() override;
    void publish(const char* topic, const char* payload) override;
    bool hasConfig() override;
    void setCallback(MQTT_CALLBACK_SIGNATURE);

private:
    MqttConfigManager mqttConfigManager;
    WiFiClient wifiClient;
    PubSubClient mqttClient;

    void reconnect();
};

#endif
