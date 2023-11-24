// WiFiConfigManager.cpp
#include "WiFiConfigManager.h"

WiFiConfigManager::WiFiConfigManager(AbstractFS& fs) : fileSystem(fs) {
    fileSystem.begin();
}

WiFiConfigManager::Config WiFiConfigManager::readConfig() {
    Config config;
    if (fileSystem.exists("/wi_fi_config.cfg")) {
        File configFile = fileSystem.open("/wi_fi_config.cfg", "r");
        if (configFile) {
            while(configFile.available()) {
                String line = configFile.readStringUntil('\n');
                line = trimString(line);

                if (config.ssid.length() == 0) {
                    config.ssid = line;
                } else if (config.password.length() == 0) {
                    config.password = line;
                } 
            }
            configFile.close();
        }
    }
    return config;
}

void WiFiConfigManager::saveConfig(const String& ssid, const String& password) {
    File configFile = fileSystem.open("/wi_fi_config.cfg", "w");
    if (configFile) {
        configFile.println(ssid);
        configFile.println(password);
        configFile.close();
    }
}

void WiFiConfigManager::clearConfig() {
    if (fileSystem.begin()) {
        if (fileSystem.exists("/wi_fi_config.cfg")) {
            fileSystem.remove("/wi_fi_config.cfg");
            Serial.println("WI-FI config configuration cleared.");
        } else {
            Serial.println("No WI-FI config configuration to clear.");
        }
        fileSystem.end();
    } else {
        Serial.println("Failed to initialize file system for clearing WI-FI config.");
    }
}

String WiFiConfigManager::trimString(const String& str) {
    String trimmed = str;
    trimmed.trim();
    return trimmed;
}
