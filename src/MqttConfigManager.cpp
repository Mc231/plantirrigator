// MqttConfigManager.cpp
#include "MqttConfigManager.h"

MqttConfigManager::MqttConfigManager(AbstractFS& fs) : fileSystem(fs) {}

MqttConfigManager::Config MqttConfigManager::readConfig() {
    Config config;
    if (fileSystem.begin()) {
        if (fileSystem.exists("/mqtt_config.cfg")) {
            File configFile = fileSystem.open("/mqtt_config.cfg", "r");
            if (configFile) {
                config.server = trimString(configFile.readStringUntil('\n'));
                config.username = trimString(configFile.readStringUntil('\n'));
                config.password = trimString(configFile.readStringUntil('\n'));
                configFile.close();
            }
        }
        fileSystem.end();
    }
    return config;
}

void MqttConfigManager::clearConfig() {
   if (fileSystem.begin()) {
        if (fileSystem.exists("/mqtt_config.cfg")) {
            fileSystem.remove("/mqtt_config.cfg");
            Serial.println("Mqtt config configuration cleared.");
        } else {
            Serial.println("No Mqtt config configuration to clear.");
        }
        fileSystem.end();
    } else {
        Serial.println("Failed to initialize file system for clearing Mqtt config.");
    }
}

void MqttConfigManager::saveConfig(const String& server, const String& username, const String& password) {
    if (fileSystem.begin()) {
        File configFile = fileSystem.open("/mqtt_config.cfg", "w");
        if (configFile) {
            configFile.println(server);
            configFile.println(username);
            configFile.println(password);
            configFile.close();
        }
        fileSystem.end();
    }
}

String MqttConfigManager::trimString(const String& str) {
    String trimmed = str;
    trimmed.trim();
    return trimmed;
}
