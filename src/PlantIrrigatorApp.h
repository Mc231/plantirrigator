// PlantIrrigatorApp.h
#ifndef PlantIrrigatorApp_h
#define PlantIrrigatorApp_h

#include "WiFiSetupManager.h"
#include "WiFiConfigManager.h"
#include "LittleFSImpl.h"
#include "AbstractWiFiSetupManager.h"
#include "IrrigatorManager.h"

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
    bool isSetupComplete;

    void setupCompleted();
    void initializeWiFi();
    // Add other methods and members relevant to your application
};

#endif
