// MqttManager.h
#ifndef MqttManager_h
#define MqttManager_h

#include <Arduino.h>
#include <FS.h>

class MqttManager {
public:
    virtual void connect() = 0;
    virtual void loop() = 0;
    virtual void publish(const char* topic, const char* payload) = 0;
    virtual bool hasConfig() = 0;
    virtual bool isConnected() = 0;
};

#endif
