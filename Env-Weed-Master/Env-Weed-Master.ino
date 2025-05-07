// This code is a part of a larger project that involves controlling a smart environment system using an Arduino. 
//The code includes the use of an LCD display, IR remote control, and various sensors to monitor temperature and moisture levels. 
//The system can be controlled manually or automatically based on the readings from the sensors.
#include <IRremote.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int tempPin = A0;      // Temperature sensor connected to A0
const int ledPin = 11;       // Red LED connected to pin 13
const int fanPin = 12;        // Fan connected to pin 9
const int RECV_PIN = 2;      // Remote control
IRrecv irrecv(RECV_PIN);      // Remote control
decode_results results;        // Remote results
unsigned long key_value = 0;    // key value enter from the remote
LiquidCrystal_I2C lcd(0x27, 16, 2); // Create LCD object at I2C address 0x27, 16 chars, 2 lines
bool redLED_on_off = false;             // For red LED turn on/off
bool fan_on_off = false;             // For fan turn on/off
bool manualLEDControl = false;
bool manualFanControl = false;


//float temperatureF = 0;
int moisture = 0;


void setup() {
  Serial.begin(9600);        // Start serial communication
  pinMode(ledPin, OUTPUT);   // Set the LED pin as output
  pinMode(fanPin, OUTPUT);   // Set the fan pin as output
  irrecv.enableIRIn();      // Remote
  irrecv.blink13(true);       //Remote
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK); 
  lcd.init();              // Initialize the LCD
  lcd.backlight();         // Turn on the backlight
  Wire.begin();             // Master

}

void loop() {

  int analogValue = analogRead(tempPin);        // Read the analog value from sensor
  float voltage = analogValue * (5.0 / 1023.0); // Convert analog value to voltage
  float temperatureC = (voltage - 0.5) * 100.0; // TMP36 formula for temperature
  
  // Manual temperatureC, because the sensor doesn't work correctly
  
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32;
  temperatureF += 110;

  //Print the current temperature in serial monitor (not in LCD)
  Serial.print("Temperature: ");
  Serial.print(temperatureF);
  Serial.println(" °F");

  if (!manualLEDControl){
   if (temperatureF < 60.0) {
    Serial.print("LED on! \n");
    digitalWrite(ledPin, HIGH);  // Turn LED on
  } else {
    digitalWrite(ledPin, LOW);   // Turn LED off
  }
}

  if (!manualFanControl) {
    if (temperatureF > 80) {
      Serial.print("Fan on! \n");
       digitalWrite(fanPin, HIGH);
    } else {
      digitalWrite(fanPin, LOW);
    }
  }
  
    // LCD output
  //Displace temperature
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperatureF, 1); // Show 1 decimal place
  lcd.print((char)223);       // Degree symbol
  lcd.print("F");

  //get data moisture from the salve
  Wire.requestFrom(8, 1);
  if (Wire.available()) {
    moisture = Wire.read(); // Read 1 byte
  }
  //Displace moisure 
  lcd.setCursor(0, 1);
  lcd.print("Moist: ");
  lcd.print(moisture);
  lcd.print(" %");

 //New way to get input from the remote
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
    // Ignore repeat signals (like when you hold down a button)
    IrReceiver.resume();
    return;
  }
    uint32_t code = IrReceiver.decodedIRData.decodedRawData;;
    switch (code) {
      case 0xF30CFF00:                    // Button "1"
        Serial.println("number 1");

          if(!manualLEDControl){
            manualLEDControl = true;
          redLED_on_off = !redLED_on_off;  // toggle the state
          
          if (redLED_on_off) {
              digitalWrite(ledPin, HIGH);  
              
          } else {
              digitalWrite(ledPin, LOW); 
              
          }
          }
        
        break;
      case 0xE718FF00:                  // Button "2"
        Serial.println("number 2");
        if(!manualFanControl){
	      fan_on_off = !fan_on_off;       // toggle the state
        manualFanControl = true;
        if (fan_on_off) {
            digitalWrite(fanPin, HIGH);  
        } else {
            digitalWrite(fanPin, LOW);  
        }
        }
        break;
      case 0xA15EFF00:  // Button "3"
        Serial.println("number 3");
        Serial.println("Auto LED control");
        manualLEDControl = false;
        break;
      case 0xF708FF00:  // Button "4"
        Serial.println("number 4");
        Serial.println("Auto fan control");
        manualFanControl = false;
        break;
      default:
        Serial.print("Unknown code: 0x");
        Serial.println(code, HEX);
        break;
    }

    IrReceiver.resume(); // Ready for next signal
  }

 delay(1000);  // Wait for 1 second before next reading
}
