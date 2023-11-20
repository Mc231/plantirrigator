// PlantIrrigatorApp.h
#ifndef PlantIrrigatorApp_h
#define PlantIrrigatorApp_h

#include "WiFiSetupManager.h"
#include "WiFiConfigManager.h"
#include "LittleFSImpl.h"
#include "AbstractWiFiSetupManager.h"
#include "IrrigatorManager.h"
#include "PostSetupAvailabilityBroadcaster.h"

class PlantIrrigatorApp {
public:
    PlantIrrigatorApp();
    void setup();
    void loop();

private:
    LittleFSImpl fileSystem;
    WiFiConfigManager configManager;
    AbstractWiFiSetupManager* setupManager;
    IrrigatorManager irrigatorManager;
    PostSetupAvailabilityBroadcaster* postSetupBroadcaster;
    bool isSetupComplete;

    void setupCompleted()