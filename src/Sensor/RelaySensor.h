// RelaySensor.h
#ifndef RelaySensor_h
#define RelaySensor_h

class RelaySensor {
public:
    virtual ~RelaySensor() {}

    virtual bool getCurrentState() = 0;
    virtual void toggle() = 0;
};

#endif
