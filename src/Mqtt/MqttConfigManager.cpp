// MqttConfigManager.cpp
#include "MqttConfigManager.h"

MqttConfigManager::MqttConfigManager(AbstractFS& fs) : fileSystem(fs), isConfigCached(false) {
    fileSystem.begin();
}

MqttConfigManager::Config MqttConfigManager::readConfig() {
    if (!isConfigCached) { 
        if (fileSystem.exists(MQTT_CONFIG_FILE)) {
            File configFile = fileSystem.open("/mqtt_config.cfg", "r");
            if (configFile) {
                cachedConfig.server = trimString(configFile.readStringUntil('\n'));
                cachedConfig.port = trimString(configFile.readStringUntil('\n'));
                cachedConfig.username = trimString(configFile.readStringUntil('\n'));
                cachedConfig.password = trimString(configFile.readStringUntil('\n'));
                cachedConfig.statusTopic = trimString(configFile.readStringUntil('\n'));
                configFile.close();
            }
        }
        isConfigCached = true;
    }
    return cachedConfig;
}

void MqttConfigManager::clearConfig() {
   if (fileSystem.begin()) {
        if (fileSystem.exists(MQTT_CONFIG_FILE)) {
            fileSystem.remove(MQTT_CONFIG_FILE);
            isConfigCached = false;
            Serial.println("Mqtt config configuration cleared.");
        } else {
            Serial.println("No Mqtt config configuration to clear.");
        }
        fileSystem.end();
    } else {
        Serial.println("Failed to initialize file system for clearing Mqtt config.");
    }
}

void MqttConfigManager::saveConfig(const String& server, const String& port, const String& username, const String& password, String& statusTopic) {
    if (fileSystem.begin()) {
        File configFile = fileSystem.open(MQTT_CONFIG_FILE, "w");
        if (configFile) {
            configFile.println(server);
            configFile.println(port);
            configFile.println(username);
            configFile.println(password);
            configFile.println(statusTopic);
            configFile.close();
            isConfigCached = true;
        }
        fileSystem.end();
    }
}

String MqttConfigManager::trimString(const String& str) {
    String trimmed = str;
    trimmed.trim();
    return trimmed;
}
