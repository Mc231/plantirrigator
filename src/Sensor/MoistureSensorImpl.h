// MoistureSensorImpl.h
#ifndef MoistureSensorImpl_h
#define MoistureSensorImpl_h

#include "MoistureSensor.h"

class MoistureSensorImpl : public MoistureSensor {
public:
    MoistureSensorImpl(int sensorPower, int sensorPin);
    void begin() override;
    int getMoistureLevel(int delayBeforeRead) override;

private:
    int sensorPower;
    int sensorPin;
};

#endif
