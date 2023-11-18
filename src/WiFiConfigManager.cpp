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
                } else {
                    config.forceSetup = line.equalsIgnoreCase("true");
                    break;
                }
            }
            configFile.close();
        }
    }
    return config;
}

void WiFiConfigManager::saveConfig(const String& ssid, const String& password, const String& forceSetup) {
    File configFile = fileSystem.open("/config.cfg", "w");
    if (configFile) {
        configFile.println(ssid);
        configFile.println(password);
        configFile.println(forceSetup);
        configFile.close();
    }
}

String WiFiConfigManager::trimString(const String& str) {
    String trimmed = str;
    trimmed.trim();
    return trimmed;
}
