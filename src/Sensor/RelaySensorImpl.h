// RelaySensorImpl.h
#ifndef RelaySensorImpl_h
#define RelaySensorImpl_h

#include "RelaySensor.h"

class RelaySensorImpl : public RelaySensor {
public:
    RelaySensorImpl(int pin);

    bool getCurrentState() override;
    void toggle() override;

private:
    int relayPin;
    bool state;
};

#endif
