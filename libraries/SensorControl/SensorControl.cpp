/**
 * SensorControl.cpp
 * Implementation of the SensorControl class
 */

#include "SensorControl.h"

 SensorControl::SensorControl(int tempPin, int moisturePin, int lightPin) :
   _tempPin(tempPin),
   _moisturePin(moisturePin),
   _lightPin(lightPin),
   _dhtSensor(tempPin, DHT22) {
   
   // Default calibration values - can be adjusted during calibration
   _moistureAirValue = 700;   // Typical value in air
   _moistureWaterValue = 300; // Typical value in water
 }
 
 void SensorControl::begin() {
   _dhtSensor.begin();
   pinMode(_moisturePin, INPUT);
   pinMode(_lightPin, INPUT);
   
   // Perform initial readings
   delay(2000); // DHT22 needs 2 seconds to stabilize
   readTemperature();
   readMoisture();
   readLightLevel();
 }
 
 float SensorControl::readTemperature() {
   float temp = _dhtSensor.readTemperature();
   
   // Check if reading failed
   if (isnan(temp)) {
     Serial.println("Failed to read temperature from DHT sensor!");
     return -999; // Error value
   }
   
   return temp;
 }
 
 float SensorControl::readHumidity() {
   float humidity = _dhtSensor.readHumidity();
   
   // Check if reading failed
   if (isnan(humidity)) {
     Serial.println("Failed to read humidity from DHT sensor!");
     return -999; // Error value
   }
   
   return humidity;
 }
 
 int SensorControl::readMoisture() {
   int rawValue = analogRead(_moisturePin);
   
   // Convert to percentage (inverted because higher analog value means drier soil)
   int moisturePercent = map(rawValue, _moistureAirValue, _moistureWaterValue, 0, 100);
   
   // Constrain to 0-100% range
   moisturePercent = constrain(moisturePercent, 0, 100);
   
   return moisturePercent;
 }
 
 int SensorControl::readLightLevel() {
   int lightLevel = analogRead(_lightPin);
   return lightLevel;
 }
 
 bool SensorControl::isTemperatureHigh() {
   float temp = readTemperature();
   return (temp != -999) && (temp > TEMP_UPPER_THRESHOLD);
 }
 
 bool SensorControl::isTemperatureLow() {
   float temp = readTemperature();
   return (temp != -999) && (temp < TEMP_LOWER_THRESHOLD);
 }
 
 bool SensorControl::isMoistureLow() {
   int moisture = readMoisture();
   return moisture < MOISTURE_THRESHOLD;
 }