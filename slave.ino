// Slave Arduino code for light and moisture control system
// this code is for a slave Arduino that controls a water pump and light based on moisture and light levels. 
// It uses analog sensors to read the values and actuates the devices accordingly.
// The code also includes serial communication for debugging purposes.
#include <Wire.h>
const int lightPin = A1;      // Light sensor connected to 
const int waterPumpPin = 10;  // Pin for water pump
const int moisturePin = A0;  // Soil moisture sensor pin
int lightLevel = 0;

const int moistureThreshold = 40;  // 40% threshold
int moisture = 0; // Moisture level variable
int moistureDry = 1023;   // Value read when sensor is in air (dry)
int moistureWet = 1; // Value read when sensor is in water (wet)
int moisturePercent = 0; // Moisture percentage variable

void setup() {
    Serial.begin(9600);
    Wire.begin(8); // Join I2C bus with address #8
    Wire.onRequest(requestEvent); // Register event handler for requests
    pinMode(lightPin, INPUT);
    pinMode(waterPumpPin, OUTPUT);

}

void loop() {
    // Read light level and moisture level
    lightLevel = analogRead(lightPin);
    Serial.println(lightLevel);
    moisture = analogRead(moisturePin);
    // Convert to percentage (inverted because higher analog value means drier soil)
    moisturePercent = map(moisture, moistureDry, moistureWet, 0, 100);
    // Constrain to 0-100% range
    moisturePercent = constrain(moisturePercent, 0, 100);

    // Actuate water pump based on moisture level
    if (moisture < moistureThreshold) {
        digitalWrite(waterPumpPin, HIGH);  // Turn on water pump
        Serial.println("Water pump ON");
    } else {
        digitalWrite(waterPumpPin, LOW);   // Turn off water pump
        Serial.println("Water pump OFF");
    }
    // Print moisture level to serial monitor
    Serial.print("Moisture: ");
    Serial.print(moisturePercent);
    Serial.println("%");

    // Actuate light based on light level
    if (lightLevel < 500) { // Adjust threshold as needed
        digitalWrite(lightPin, HIGH);  // Turn on light
        Serial.println("Light ON");
    } else {
        digitalWrite(lightPin, LOW);   // Turn off light
        Serial.println("Light OFF");
    }
    // Print light level to serial monitor
    Serial.print("Light Level: ");
    Serial.print(lightLevel);  
}

// Function to handle I2C requests
// This function is called when the master requests data from the slave
void requestEvent() {
    Wire.write(moisturePercent); // Send moisture percentage to master
    Serial.print("Sent moisture percentage: ");
    Serial.println(moisturePercent);
    
}