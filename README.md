# ESP32 Weather Clock

An IoT-based weather clock built using ESP32, a 20x4 I2C LCD, NTP time synchronization, and the OpenWeather API.

## Version

V1.0

## Features

- Real-time clock using NTP
- Indian Standard Time (IST)
- Automatic date display
- Automatic day display
- Wi-Fi connectivity
- OpenWeather API integration
- Outdoor temperature
- Current weather condition
- JSON parsing using ArduinoJson
- Automatic weather update every 10 minutes
- 20x4 I2C LCD display

## Hardware

- ESP32 DevKit V1
- JHD204A 20x4 LCD
- I2C LCD backpack

## Software

- Arduino IDE
- ESP32 Arduino Core
- LiquidCrystal_I2C
- ArduinoJson
- OpenWeather API
- NTP

## System Architecture

ESP32
↓
Wi-Fi
├── NTP Server → Time / Date / Day
│
└── OpenWeather API → JSON → ArduinoJson → Weather Data
↓
I2C
↓
JHD204A 20x4 LCD

## Display

TIME HH:MM:SS
DATE DD/MM/YYYY
DAY Wednesday
TEMP XX C Clouds

## Current Status

V1.0 successfully tested.

- Wi-Fi connection: Working
- NTP synchronization: Working
- LCD display: Working
- OpenWeather API: Working
- JSON parsing: Working
- Automatic weather update: Working

## Future Upgrades

- Feels-like temperature
- Humidity
- Better weather-condition formatting
- Wi-Fi reconnection
- API error handling
- Secure credential management
- Custom LCD weather icons
- Modular code structure
- PCB design