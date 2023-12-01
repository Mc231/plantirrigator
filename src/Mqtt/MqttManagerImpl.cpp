// MqttManagerImpl.cpp
#include "MqttManagerImpl.h"

MqttManagerImpl::MqttManagerImpl(MqttConfigManager &mqttConfigManager)
    : mqttConfigManager(mqttConfigManager), mqttClient(wifiClient), serverSetup(false)
{ }

void MqttManagerImpl::connect()
{
    if (!serverSetup && hasConfig())
    {
        auto config = mqttConfigManager.readConfig();
        int port = config.port.toInt();
        mqttClient.setServer(config.server.c_str(), port);
        serverSetup = true;
    }

    if (!mqttClient.connected())
    {
        String clientId = "IrrigatorClient-";
        clientId += String(random(0xffff), HEX);
        auto config = mqttConfigManager.readConfig();
        if (mqttClient.connect(clientId.c_str(), config.username.c_str(), config.password.c_str()))
        {
            Serial.println("Connected to MQTT Broker!");
        }
        else
        {
            Serial.print("Failed to connect, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
            connect();
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
    if (mqttClient.connected())
    {
        mqttClient.publish(topic, payload);
    }
}

void MqttManagerImpl::setCallback(MQTT_CALLBACK_SIGNATURE)
{
    mqttClient.setCallback(callback);
}