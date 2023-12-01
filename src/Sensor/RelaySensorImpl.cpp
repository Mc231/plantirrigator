// RelaySensorImpl.cpp
#include "RelaySensorImpl.h"
#include <Arduino.h>

RelaySensorImpl::RelaySensorImpl(int pin) : relayPin(pin), state(false) {
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW);
}

bool RelaySensorImpl::getCurrentState() {
    return state;
}

void RelaySensorImpl::toggle() {
    state = !state;
    digitalWrite(relayPin, state ? HIGH : LOW);
}
