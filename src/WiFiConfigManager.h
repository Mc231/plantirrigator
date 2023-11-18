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
        bool forceSetup;
    };

    WiFiConfigManager(AbstractFS& fs);

    Config readConfig();
    void saveConfig(const String& ssid, const String& password, const String& forceSetup);

private:
    AbstractFS& fileSystem;
    String trimString(const String& str);
};

#endif
