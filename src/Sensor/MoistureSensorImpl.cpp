// MoistureSensorImpl.cpp
#include "MoistureSensorImpl.h"
#include <Arduino.h>

MoistureSensorImpl::MoistureSensorImpl(int sensorPower, int sensorPin)
    : sensorPower(sensorPower), sensorPin(sensorPin) {}

void MoistureSensorImpl::begin() {
	pinMode(sensorPower, OUTPUT);
	// Initially keep the sensor OFF
	digitalWrite(sensorPower, LOW);
}

int MoistureSensorImpl::getMoistureLevel(int delayBeforeRead) {
	digitalWrite(sensorPower, HIGH);	
	delay(delayBeforeRead);						
	int val = analogRead(sensorPin);
	digitalWrite(sensorPower, LOW);	
	return val;	
}