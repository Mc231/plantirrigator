// PostSetupAvailabilityBroadcaster.h
#ifndef PostSetupAvailabilityBroadcaster_h
#define PostSetupAvailabilityBroadcaster_h

class PostSetupAvailabilityBroadcaster {
public:
    virtual ~PostSetupAvailabilityBroadcaster() {}
    virtual void begin() = 0;
    virtual void loop() = 0;
};

#endif
