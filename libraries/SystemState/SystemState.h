/**
 * SystemState.h
 * Manages the state machine for the Smart Environment Management System
 */

#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#include <Arduino.h>
#include <RTClib.h>

// Define system states
enum State
{
    INITIAL,
    ACTIVE,
    MANUAL_OVERRIDE
};

class SystemState
{
public:
    // Constructor
    SystemState();

    // Core methods
    void begin();
    void update(float temperature, int moisture, int lightLevel, DateTime currentTime);
    void processSchedule(DateTime currentTime);

    // State control methods
    void toggleManualOverride();
    State getCurrentState();

    // Device state getters
    bool getFanState();
    bool getHeaterState();
    bool getPumpState();
    bool getLightState();

    // Manual override methods
    void setManualFan(bool state);
    void setManualHeater(bool state);
    void setManualPump(bool state);
    void setManualLight(bool state);

private:
    // Current system state
    State _currentState;

    // Device states
    bool _fanActive;
    bool _heaterActive;
    bool _pumpActive;
    bool _lightActive;

    // Manual override flags
    bool _manualFanOverride;
    bool _manualHeaterOverride;
    bool _manualPumpOverride;
    bool _manualLightOverride;

    // Environmental thresholds
    const float TEMP_UPPER_THRESHOLD = 27.0; // 80°F
    const float TEMP_LOWER_THRESHOLD = 15.0; // 60°F
    const int MOISTURE_THRESHOLD = 40;       // 40%

    // Light schedule times (in hours, 24-hour format)
    const int LIGHTS_ON_HOUR = 4;  // 4 AM
    const int LIGHTS_OFF_HOUR = 0; // Midnight

    // Timing variables for device delays
    unsigned long _fanLastToggleTime;
    unsigned long _heaterLastToggleTime;
    unsigned long _pumpLastToggleTime;
    const unsigned long TOGGLE_DELAY = 10000;  // 10 seconds
    const unsigned long OPERATION_TIME = 5000; // 5 seconds

    // Helper methods
    void updateDeviceStates(float temperature, int moisture, int lightLevel, DateTime time);
    bool isLightingScheduleActive(DateTime time);
};

#endif