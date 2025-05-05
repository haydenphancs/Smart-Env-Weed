#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include "SensorControl.h"
#include "OutputDevices.h"
#include "SystemState.h"

// Pin definitions
#define TEMP_HUMIDITY_PIN 2    // DHT22 sensor
#define MOISTURE_SENSOR_PIN A0 // Soil moisture sensor
#define   A1    // LDR or light sensor
#define FAN_SERVO_PIN 9        // Servo for fan
#define HEATING_LED_PIN 10     // Heating indicator LED
#define WATER_PUMP_PIN 11      // Relay for water pump
#define GROW_LIGHT_PIN 12      // Relay for grow lights
#define OVERRIDE_BUTTON_PIN 3  // Button for override mode
#define FAN_BUTTON_PIN 4       // Button to control fan
#define HEAT_BUTTON_PIN 5      // Button to control heater
#define WATER_BUTTON_PIN 6     // Button to control water pump
#define LIGHT_BUTTON_PIN 7     // Button to control lights

// Interrupt flags
volatile bool buttonPressed = false;
volatile byte buttonId = 0;
volatile bool checkSchedule = false;
volatile unsigned long secondCounter = 0;

// Object initialization
SensorControl sensors(TEMP_HUMIDITY_PIN, MOISTURE_SENSOR_PIN, LIGHT_SENSOR_PIN);
OutputDevices outputs(FAN_SERVO_PIN, HEATING_LED_PIN, WATER_PUMP_PIN, GROW_LIGHT_PIN);
SystemState system;
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2); // 0x27 is the I2C address, adjust if needed

void setup()
{
    Serial.begin(9600);

    // Initialize components
    Wire.begin();
    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        while (1)
            ;
    }

    // Set the RTC to the date & time on PC when sketch was compiled
    // Uncomment this line ONLY when first uploading the code
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    lcd.init();
    lcd.backlight();
    lcd.print("Initializing...");

    sensors.begin();
    outputs.begin();
    system.begin();

    // Setup button interrupts
    pinMode(OVERRIDE_BUTTON_PIN, INPUT_PULLUP);
    pinMode(FAN_BUTTON_PIN, INPUT_PULLUP);
    pinMode(HEAT_BUTTON_PIN, INPUT_PULLUP);
    pinMode(WATER_BUTTON_PIN, INPUT_PULLUP);
    pinMode(LIGHT_BUTTON_PIN, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(OVERRIDE_BUTTON_PIN), overrideButtonISR, FALLING);

    // Setup timer interrupt
    setupTimerInterrupt();

    lcd.clear();
    lcd.print("System ready");
    delay(1000);
}

void loop()
{
    // Get current time
    DateTime now = rtc.now();

    // Read all sensors
    float temperature = sensors.readTemperature();
    int moisture = sensors.readMoisture();
    int lightLevel = sensors.readLightLevel();

    // Check if button was pressed (interrupt flag)
    if (buttonPressed)
    {
        handleButtonPress(buttonId);
        buttonPressed = false;
    }

    // Check if it's time to check the schedule (from timer interrupt)
    if (checkSchedule)
    {
        system.processSchedule(now);
        checkSchedule = false;
    }

    // Update system state based on sensor readings and time
    system.update(temperature, moisture, lightLevel, now);

    // Control output devices based on system state
    if (system.getFanState())
        outputs.activateFan();
    else
        outputs.deactivateFan();

    if (system.getHeaterState())
        outputs.activateHeater();
    else
        outputs.deactivateHeater();

    if (system.getPumpState())
        outputs.activateWaterPump();
    else
        outputs.deactivateWaterPump();

    if (system.getLightState())
        outputs.activateLights();
    else
        outputs.deactivateLights();

    // Update display
    updateDisplay(temperature, moisture, lightLevel, now, system);

    // Debug output
    Serial.print("Time: ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(" Temp: ");
    Serial.print(temperature);
    Serial.print("C Moisture: ");
    Serial.print(moisture);
    Serial.print("% Light: ");
    Serial.println(lightLevel);

    delay(1000); // Main loop runs once per second
}

// Interrupt Service Routines
void overrideButtonISR()
{
    buttonPressed = true;
    buttonId = 0; // Override button
}

void setupTimerInterrupt()
{
    cli(); // Disable interrupts

    // Set up Timer1
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    // Set compare match register for 1Hz increments
    OCR1A = 15624; // = 16MHz/1024/1Hz - 1

    // Turn on CTC mode
    TCCR1B |= (1 << WGM12);

    // Set CS10 and CS12 bits for 1024 prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10);

    // Enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);

    sei(); // Enable interrupts
}

// Timer1 interrupt service routine
ISR(TIMER1_COMPA_vect)
{
    secondCounter++;
    checkSchedule = true;
}

void handleButtonPress(byte buttonId)
{
    switch (buttonId)
    {
    case 0: // Override button
        system.toggleManualOverride();
        break;
        // Additional cases for other buttons can be added
    }
}

void updateDisplay(float temp, int moisture, int light, DateTime time, SystemState &state)
{
    lcd.clear();

    // First row: Time and temperature
    lcd.setCursor(0, 0);
    lcd.print(time.hour(), DEC);
    lcd.print(':');
    if (time.minute() < 10)
        lcd.print('0');
    lcd.print(time.minute(), DEC);
    lcd.print(" T:");
    lcd.print(temp, 1);
    lcd.print("C");

    // Second row: System state and active devices
    lcd.setCursor(0, 1);

    // Show current state
    if (state.getCurrentState() == MANUAL_OVERRIDE)
    {
        lcd.print("MANUAL ");
    }
    else
    {
        lcd.print("AUTO ");
    }

    // Show active devices
    if (state.getFanState())
        lcd.print("F");
    if (state.getHeaterState())
        lcd.print("H");
    if (state.getPumpState())
        lcd.print("W");
    if (state.getLightState())
        lcd.print("L");
}