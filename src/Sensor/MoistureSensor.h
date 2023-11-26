// MoistureSensor.h
#ifndef MoistureSensor_h
#define MoistureSensor_h

class MoistureSensor {
public:
    virtual void begin() = 0;
    virtual int getMoistureLevel(int delayBeforeRead) = 0;
};

#endif
