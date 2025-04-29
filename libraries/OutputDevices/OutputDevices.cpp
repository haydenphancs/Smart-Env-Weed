/**
 * OutputDevices.cpp
 * Implementation of the OutputDevices class
 */

#include "OutputDevices.h"

 OutputDevices::OutputDevices(int fanPin, int heaterPin, int pumpPin, int lightPin) :
   _fanPin(fanPin),
   _heaterPin(heaterPin),
   _pumpPin(pumpPin),
   _lightPin(lightPin),
   _fanActive(false),
   _heaterActive(false),
   _pumpActive(false),
   _lightActive(false) {
 }
 
 void OutputDevices::begin() {
   // Initialize servo
   _fanServo.attach(_fanPin);
   _fanServo.write(FAN_OFF_POSITION);
   
   // Initialize other output pins
   pinMode(_heaterPin, OUTPUT);
   pinMode(_pumpPin, OUTPUT);
   pinMode(_lightPin, OUTPUT);
   
   // Set all devices to OFF state
   deactivateAllDevices();
 }
 
 void OutputDevices::activateFan() {
   if (!_fanActive) {
     _fanServo.write(FAN_ON_POSITION);
     _fanActive = true;
     Serial.println("Fan activated");
   }
 }
 
 void OutputDevices::deactivateFan() {
   if (_fanActive) {
     _fanServo.write(FAN_OFF_POSITION);
     _fanActive = false;
     Serial.println("Fan deactivated");
   }
 }
 
 void OutputDevices::activateHeater() {
   if (!_heaterActive) {
     digitalWrite(_heaterPin, HIGH);
     _heaterActive = true;
     Serial.println("Heater activated");
   }
 }
 
 void OutputDevices::deactivateHeater() {
   if (_heaterActive) {
     digitalWrite(_heaterPin, LOW);
     _heaterActive = false;
     Serial.println("Heater deactivated");
   }
 }
 
 void OutputDevices::activateWaterPump() {
   if (!_pumpActive) {
     digitalWrite(_pumpPin, HIGH);
     _pumpActive = true;
     Serial.println("Water pump activated");
   }
 }
 
 void OutputDevices::deactivateWaterPump() {
   if (_pumpActive) {
     digitalWrite(_pumpPin, LOW);
     _pumpActive = false;
     Serial.println("Water pump deactivated");
   }
 }
 
 void OutputDevices::activateLights() {
   if (!_lightActive) {
     digitalWrite(_lightPin, HIGH);
     _lightActive = true;
     Serial.println("Lights activated");
   }
 }
 
 void OutputDevices::deactivateLights() {
   if (_lightActive) {
     digitalWrite(_lightPin, LOW);
     _lightActive = false;
     Serial.println("Lights deactivated");
   }
 }
 
 void OutputDevices::deactivateAllDevices() {
   deactivateFan();
   deactivateHeater();
   deactivateWaterPump();
   deactivateLights();
   Serial.println("All devices deactivated");
 }