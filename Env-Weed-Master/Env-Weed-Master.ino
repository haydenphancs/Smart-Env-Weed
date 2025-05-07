#include <IRremote.h>
//#include <IRremote.hpp>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#include <SoftwareWire.h>
//#include <NewPing.h>
//TimerFreeTone.h>




const int tempPin = A0; // Temperature sensor connected to A0
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

// Test Echo
#define buzzerPin 10
#define trigPin 8
#define echoPin 9

long duration;

int threshold = 20; // cm - adjust as needed
int alertTone = 1000; // frequency in Hz
#define maxDistance 200
//NewPing sonar(trigPin, echoPin, maxDistance);
//int distance = sonar.ping_cm();
int changeTempF = 30;
float temperatureF = 0;
int moisture = 0;


void setup() {
  Wire.begin();  // Master

  
  Serial.begin(9600);        // Start serial communication
  pinMode(ledPin, OUTPUT);   // Set the LED pin as output
  pinMode(fanPin, OUTPUT);   // Set the fan pin as output
  //irrecv.enableIRIn();      // Remote
  //irrecv.blink13(true);       //Remote
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK); 
  lcd.init();              // Initialize the LCD
  lcd.backlight();         // Turn on the backlight
             


//Echo
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

}

void loop() {
 int distance = 0;

  int reading = analogRead(tempPin);
  float voltage = reading * (5.0 / 1023.0);     // Convert ADC to voltage
  float temperatureC = voltage * 100.0;         // LM35 gives 10mV per °C
  temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  //temperatureF = getSmoothedTemperatureF();
  temperatureF += changeTempF;

  //Print the current temperature in serial monitor (not in LCD)
  Serial.print("Temperature: ");
  Serial.print(temperatureF);
  Serial.println(" °F");

  if (!manualLEDControl){
   if (temperatureF < 60) {
    Serial.print("LED on! \n");
    digitalWrite(ledPin, HIGH);  // Turn LED on
  } else {
    digitalWrite(ledPin, LOW);   // Turn LED off
  }
}

  if (!manualFanControl) {
    if (temperatureF > 75) {
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
  lcd.print(temperatureF, 1);            // Show 1 decimal place
  lcd.print((char)223);       // Degree symbol
  lcd.print("F");

  //get data moisture from the salve
  Wire.requestFrom(8, 1);
  if (Wire.available()) {
    moisture = Wire.read(); // Read 1 byte
      lcd.setCursor(0, 1);
  lcd.print("Moist: ");
  lcd.print(moisture);
  lcd.print(" %");
  }
  //Displace moisure 



//  //New way to get input from the remote
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
            manualLEDControl = !manualLEDControl;
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


      case  0xB946FF00:  // Button "Vol+" but increase temp
        Serial.println("Vol+");
        Serial.println("Increase temp (+2)");
        changeTempF = changeTempF + 3;
        break;

      case 0xEA15FF00: // Button Vol- but decrease temp
        Serial.println("Vol-");
        Serial.println("Decrease temp (-2)");
         changeTempF = changeTempF - 3;
        break;

        
      default:
        Serial.print("Unknown code: 0x");
        Serial.println(code, HEX);
        break;
    }

    IrReceiver.resume(); // Ready for next signal
  }


  ///Test Echo
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure echo time
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate distance in cm
  distance = duration * 0.034 / 2;
 
  //Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 10) {
  tone(buzzerPin, 1000);  // 1000 Hz tone
} else {
  noTone(buzzerPin);      // Stop tone
}
// if (distance < 10) {
//   digitalWrite(buzzerPin, HIGH);
//   delay(500);  
//   //tone(buzzerPin, 1000);  // 1000 Hz tone
//   //TimerFreeTone(buzzerPin, 1000, 500);

// } else {
//   digitalWrite(buzzerPin, LOW);
//   //noTone(buzzerPin);
// }

 delay(1000);  // Wait for 1 second before next reading
}

// float getSmoothedTemperatureF() {
//   int total = 0;
//   const int samples = 10;

//   for (int i = 0; i < samples; i++) {
//     total += analogRead(A0);
//     delay(10);  // Short delay between samples
//   }

//   float avgReading = total / (float)samples;
//   float voltage = avgReading * (5.0 / 1023.0);
//   float tempC = voltage * 100.0;
//   return (tempC * 9.0 / 5.0) + 32.0;
// }
