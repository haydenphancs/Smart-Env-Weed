# Smart Environment Management System

An automated environment management system designed to optimize the growth of marijuana plants by monitoring and controlling key environmental factors.

## Team Members
- Sunil Gupta
- Hai Phan
- Khasar Munkh-Erdene

## Project Description
This system monitors and controls key environmental factors such as water supply, temperature, and lighting to create ideal growing conditions for marijuana plants.

### Features
- Temperature control (60°F-80°F)
- Moisture monitoring and control (40%-70%)
- Automated lighting schedule (4 AM to midnight)
- Manual override capability
- Real-time monitoring via LCD display

## Hardware Requirements
- Arduino Uno
- DHT22 temperature and humidity sensor
- Soil moisture sensor
- Light sensor
- DS3231 RTC module
- Servo motor (for fan)
- LED (for heating indicator)
- Relays for water pump and grow lights
- LCD display
- Buttons for user input

## Software Architecture
The code is organized into three main libraries:
- SensorControl: Handles all sensor inputs
- OutputDevices: Controls all actuators
- SystemState: Implements the state machine logic