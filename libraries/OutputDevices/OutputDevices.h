/**
 * OutputDevices.h
 * Controls all output devices for the Smart Environment Management System
 */

 #ifndef OUTPUT_DEVICES_H
 #define OUTPUT_DEVICES_H
 
 #include <Arduino.h>
 #include <Servo.h>
 
 class OutputDevices {
   public:
     // Constructor
     OutputDevices(int fanPin, int heaterPin, int pumpPin, int lightPin);
     
     // Initialization
     void begin();
     
     // Device control methods
     void activateFan();
     void deactivateFan();
     
     void activateHeater();
     void deactivateHeater();
     
     void activateWaterPump();
     void deactivateWaterPump();
     
     void activateLights();
     void deactivateLights();
     
     // Emergency functions
     void deactivateAllDevices();
     
   private:
     // Pins
     int _fanPin;
     int _heaterPin;
     int _pumpPin;
     int _lightPin;
     
     // Device state tracking
     bool _fanActive;
     bool _heaterActive;
     bool _pumpActive;
     bool _lightActive;
     
     // Servo object for fan
     Servo _fanServo;
     
     // Servo positions
     const int FAN_OFF_POSITION = 0;
     const int FAN_ON_POSITION = 180;
 };
 
 #endif