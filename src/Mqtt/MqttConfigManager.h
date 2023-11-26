// MqttConfigManager.h
#ifndef MqttConfigManager_h
#define MqttConfigManager_h

#include "Fs/AbstractFS.h"

class MqttConfigManager {
public:
    struct Config {
        String server;
        String port;
        String username;
        String password;
        String statusTopic;
    };

    MqttConfigManager(AbstractFS& fs);

    Config readConfig();
    void clearConfig();
    void saveConfig(const String& server, const String& port, const String& username, const String& password, String& statusTopic);

private:
    static constexpr const char* MQTT_CONFIG_FILE = "/mqtt_config.cfg";
    AbstractFS& fileSystem;
    String trimString(const String& str);
};

#endif
