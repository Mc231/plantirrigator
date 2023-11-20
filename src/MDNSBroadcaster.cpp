// MDNSBroadcaster.cpp
#include "MDNSBroadcaster.h"

void MDNSBroadcaster::begin() {
    if (!MDNS.begin("irrigator")) {
        Serial.println("Error setting up MDNS responder!");
    } else {
        Serial.println("mDNS responder started");
        MDNS.addService("http", "_irrigator._tcp.", 80);
  