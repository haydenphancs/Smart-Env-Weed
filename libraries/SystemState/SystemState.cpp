/**
 * SystemState.cpp
 * Implementation of the SystemState class
 */

#include "SystemState.h"

 SystemState::SystemState() :
    _currentState(INITIAL),
    _fanActive(false),
    _heaterActive(false),
    _pumpActive(false),
    _lightActive(false),
    _manualFanOverride(false),
    _manualHeaterOverride(false),
    _manualPumpOverride(false),
    _manualLightOverride(false),
    _fanLastToggleTime(0),
    _heaterLastToggleTime(0),
    _pumpLastToggleTime(0) {
 }
 
 void SystemState::begin() {
    // Initialize to INITIAL state
    _currentState = INITIAL;

    // After brief initialization, transition to ACTIVE state
    _currentState = ACTIVE;
    Serial.println("System entered ACTIVE state");
 }
 
 void SystemState::update(float temperature, int moisture, int lightLevel, DateTime currentTime) {
   if (_currentState == INITIAL) {
     // In INITIAL state, just collect data without controlling devices
     return;
   }
   
   // In any other state, update device states based on readings
   updateDeviceStates(temperature, moisture, lightLevel, currentTime);
 }
 
 void SystemState::processSchedule(DateTime currentTime) {
   // This method is called from the timer interrupt
   // Update time-based operations here
   
   // Light scheduling is handled in updateDeviceStates, 
   // but additional scheduled tasks could be added here
 }
 
 void SystemState::toggleManualOverride() {
   if (_currentState == MANUAL_OVERRIDE) {
     // Exit manual override
     _currentState = ACTIVE;
     _manualFanOverride = false;
     _manualHeaterOverride = false;
     _manualPumpOverride = false;
     _manualLightOverride = false;
     Serial.println("Exiting MANUAL_OVERRIDE state");
   } else {
     // Enter manual override
     _currentState = MANUAL_OVERRIDE;
     Serial.println("Entering MANUAL_OVERRIDE state");
   }
 }
 
 void SystemState::updateDeviceStates(float temperature, int moisture, int lightLevel, DateTime time) {
   unsigned long currentMillis = millis();
   
   // Update fan state based on temperature (if not manually overridden)
   if (!_manualFanOverride) {
     if (temperature > TEMP_UPPER_THRESHOLD) {
       // Check delay before turning on
       if (!_fanActive && (currentMillis - _fanLastToggleTime > TOGGLE_DELAY)) {
         _fanActive = true;
         _fanLastToggleTime = currentMillis;
         Serial.println("Fan activated by temperature control");
       }
     } else if (_fanActive && (currentMillis - _fanLastToggleTime > OPERATION_TIME)) {
       // Turn off after operation time
       _fanActive = false;
       _fanLastToggleTime = currentMillis;
       Serial.println("Fan deactivated by temperature control");
     }
   }
   
   // Update heater state based on temperature (if not manually overridden)
   if (!_manualHeaterOverride) {
     if (temperature < TEMP_LOWER_THRESHOLD) {
       // Check delay before turning on
       if (!_heaterActive && (currentMillis - _heaterLastToggleTime > TOGGLE_DELAY)) {
         _heaterActive = true;
         _heaterLastToggleTime = currentMillis;
         Serial.println("Heater activated by temperature control");
       }
     } else if (_heaterActive && (currentMillis - _heaterLastToggleTime > OPERATION_TIME)) {
       // Turn off after operation time
       _heaterActive = false;
       _heaterLastToggleTime = currentMillis;
       Serial.println("Heater deactivated by temperature control");
     }
   }
   
   // Update pump state based on moisture (if not manually overridden)
   if (!_manualPumpOverride) {
     if (moisture < MOISTURE_THRESHOLD) {
       // Check delay before turning on
       if (!_pumpActive && (currentMillis - _pumpLastToggleTime > TOGGLE_DELAY)) {
         _pumpActive = true;
         _pumpLastToggleTime = currentMillis;
         Serial.println("Water pump activated by moisture control");
       }
     } else if (_pumpActive && (currentMillis - _pumpLastToggleTime > OPERATION_TIME)) {
       // Turn off after operation time
       _pumpActive = false;
       _pumpLastToggleTime = currentMillis;
       Serial.println("Water pump deactivated by moisture control");
     }
   }
   
   // Update light state based on time schedule (if not manually overridden)
   if (!_manualLightOverride) {
     _lightActive = isLightingScheduleActive(time);
   }
 }
 
 bool SystemState::isLightingScheduleActive(DateTime time) {
   int currentHour = time.hour();
   
   // Handle case where lighting period spans midnight
   if (LIGHTS_ON_HOUR < LIGHTS_OFF_HOUR) {
     // Simple case: both times are in the same day
     return (currentHour >= LIGHTS_ON_HOUR && currentHour < LIGHTS_OFF_HOUR);
   } else {
     // Complex case: lighting period spans midnight
     return (currentHour >= LIGHTS_ON_HOUR || currentHour < LIGHTS_OFF_HOUR);
   }
 }
 
 State SystemState::getCurrentState() {
   return _currentState;
 }
 
 bool SystemState::getFanState() {
   return _fanActive;
 }
 
 bool SystemState::getHeaterState() {
   return _heaterActive;
 }
 
 bool SystemState::getPumpState() {
   return _pumpActive;
 }
 
 bool SystemState::getLightState() {
   return _lightActive;
 }
 
 void SystemState::setManualFan(bool state) {
   if (_currentState == MANUAL_OVERRIDE) {
     _manualFanOverride = true;
     _fanActive = state;
   }
 }
 
 void SystemState::setManualHeater(bool state) {
   if (_currentState == MANUAL_OVERRIDE) {
     _manualHeaterOverride = true;
     _heaterActive = state;
   }
 }
 
 void SystemState::setManualPump(bool state) {
   if (_currentState == MANUAL_OVERRIDE) {
     _manualPumpOverride = true;
     _pumpActive = state;
   }
 }
 
 void SystemState::setManualLight(bool state) {
   if (_currentState == MANUAL_OVERRIDE) {
     _manualLightOverride = true;
     _lightActive = state;
   }
 }