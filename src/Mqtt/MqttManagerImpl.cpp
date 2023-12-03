// MqttManagerImpl.cpp
#include "MqttManagerImpl.h"

MqttManagerImpl::MqttManagerImpl(MqttConfigManager &mqttConfigManager)
    : mqttConfigManager(mqttConfigManager), mqttClient(wifiClient), connectAttempts(0)
{ }

void MqttManagerImpl::connect()
{
    while (!mqttClient.connected() && connectAttempts <= MAX_RETRY_ATTEMPTS)
    {
        Serial.println("Attempt to connect to mqtt: ");
        Serial.print(connectAttempts);
        Serial.println("");
        auto config = mqttConfigManager.readConfig();
        int port = config.port.toInt();
        mqttClient.setServer(config.server.c_str(), port);
        String clientId = "IrrigatorClient-";
        clientId += String(random(0xffff), HEX);
        if (mqttClient.connect(clientId.c_str(), config.username.c_str(), config.password.c_str()))
        {
            Serial.println("Connected to MQTT Broker!");
        }
        else
        {
            connectAttempts++;
            Serial.print("Failed to connect, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            delay(RETRY_DELAY);
        }
    }
}

bool MqttManagerImpl::hasConfig()
{
    auto config = mqttConfigManager.readConfig();
    return !config.server.isEmpty() && !config.port.isEmpty();
}

bool MqttManagerImpl::isConnected()
{
    return mqttClient.connected();
}

void MqttManagerImpl::loop()
{
    mqttClient.loop();
}

void MqttManagerImpl::publish(const char *topic, const char *payload)
{
    mqttClient.publish(topic, payload);
}

void MqttManagerImpl::setCallback(MQTT_CALLBACK_SIGNATURE)
{
    mqttClient.setCallback(callback);
}
