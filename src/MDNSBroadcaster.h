// MDNSBroadcaster.h
#ifndef MDNSBroadcaster_h
#define MDNSBroadcaster_h

#include "PostSetupAvailabilityBroadcaster.h"
#include <ESP8266mDNS.h>

class MDNSBroadcaster : public PostSetupAvailabilityBroadcaster {
public:
    void begin() override;
    void loop() override;
};

#endif
