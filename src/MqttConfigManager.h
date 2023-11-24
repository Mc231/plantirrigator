// MqttConfigManager.h
#ifndef MqttConfigManager_h
#define MqttConfigManager_h

#include "AbstractFS.h"

class MqttConfigManager {
public:
    struct Config {
        String server;
        String username;
        String password;
    };

    MqttConfigManager(AbstractFS& fs);

    Config readConfig();
    void clearConfig();
    void saveConfig(const String& server, const String& username, const String& password);

private:
    AbstractFS& fileSystem;
    String trimString(const String& str);
};

#endif
