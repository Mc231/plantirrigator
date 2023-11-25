// MqttManagerImpl.cpp
#include "MqttManagerImpl.h"

MqttManagerImpl::MqttManagerImpl(MqttConfigManager& mqttConfigManager)
    : mqttConfigManager(mqttConfigManager), mqttClient(wifiClient) {
        auto config = mqttConfigManager.readConfig();
        if (!config.port.isEmpty())
        {
            int port =  config.port.toInt();
            mqttClient.setServer(config.server.c_str(), port);
        }
    }

void MqttManagerImpl::connect() {
    if (!mqttClient.connected()) {
        String clientId = "IrrigatorClient-";
        clientId += String(random(0xffff), HEX); 
        auto config = mqttConfigManager.readConfig();
        if (mqttClient.connect(clientId.c_str(), config.username.c_str(), config.password.c_str())) {
            Serial.println("Connected to MQTT Broker!");
        } else {
            Serial.print("Failed to connect, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
        }
    }
}

bool MqttManagerImpl::hasConfig() {
    return !mqttConfigManager.readConfig().server.isEmpty();
}

void MqttManagerImpl::loop() {
    if (!mqttClient.connected()) {
        reconnect();
    }
    mqttClient.loop();
}

void MqttManagerImpl::publish(const char* topic, const char* payload) {
    if (mqttClient.connected()) {
        mqttClient.publish(topic, payload);
    }
}

void MqttManagerImpl::setCallback(MQTT_CALLBACK_SIGNATURE) {
    mqttClient.setCallback(callback);
}

void MqttManagerImpl::reconnect() {
    while (!mqttClient.connected()) {
        connect();
        delay(5000);
    }
}
