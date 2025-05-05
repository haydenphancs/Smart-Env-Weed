/**
 * SensorControl.h
 * Handles all environmental sensing for the Smart Environment Management System
 */

 #ifndef SENSOR_CONTROL_H
 #define SENSOR_CONTROL_H
 
 #include <Arduino.h>
 #include <DHT.h>
 
 class SensorControl {
   public:
     // Constructor
     SensorControl(int tempPin, int moisturePin, int lightPin);
     
     // Methods
     void begin();
     float readTemperature();
     float readHumidity();
     int readMoisture();      // Returns moisture percentage (0-100%)
     int readLightLevel();    // Returns light level (0-1023)
     
     // Helper methods
     bool isTemperatureHigh();
     bool isTemperatureLow();
     bool isMoistureLow();
     
   private:
     // Pins
     int _tempPin;
     int _moisturePin;
     int _lightPin;
     
     // Sensor objects
     DHT _dhtSensor;
     
     // Conversion and calibration values
     int _moistureAirValue = 1023;   // Value read when sensor is in air (dry)
     int _moistureWaterValue = 1; // Value read when sensor is in water (wet)
     
     // Threshold values
     const float TEMP_UPPER_THRESHOLD = 27.0; // 80°F
     const float TEMP_LOWER_THRESHOLD = 15.0; // 60°F
     const int MOISTURE_THRESHOLD = 40;       // 40%
 };
 
 #endif