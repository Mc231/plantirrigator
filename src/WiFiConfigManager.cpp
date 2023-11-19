// WiFiConfigManager.cpp
#include "WiFiConfigManager.h"

WiFiConfigManager::WiFiConfigManager(AbstractFS& fs) : fileSystem(fs) {
    fileSystem.begin();
}

WiFiConfigManager::Config WiFiConfigManager::readConfig() {
    Config config;
    if (fileSystem.exists("/config.cfg")) {
        File configFile = fileSystem.open("/config.cfg", "r");
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
    File configFile = fileSystem.open("/config.cfg", "w");
    if (configFile) {
        configFile.println(ssid);
        configFile.println(password);
        configFile.close();
    }
}

void WiFiConfigManager::clearConfig() {
    if (fileSystem.exists("/config.cfg")) {
        fileSystem.remove("/config.cfg");
        Serial.println("Wi-Fi configuration cleared.");
    } else {
        Serial.println("No Wi-Fi configuration to clear.");
    }
}

String WiFiConfigManager::trimString(const String& str) {
    String trimmed = str;
    trimmed.trim();
    return trimmed;
}
