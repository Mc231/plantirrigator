// WiFiConfigManager.h
#ifndef WiFiConfigManager_h
#define WiFiConfigManager_h
#include <FS.h>
#include "AbstractFS.h"

class WiFiConfigManager {
public:
    struct Config {
        String ssid;
        String password;
    };

    WiFiConfigManager(AbstractFS& fs);

    Config readConfig();
    void clearConfig();
    void saveConfig(const String& ssid, const String& password);

private:
    AbstractFS& fileSystem;
    String trimString(const String& str);
};

#endif
