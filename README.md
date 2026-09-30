# ESP32 Weather Clock

An IoT-based weather clock built using an ESP32, 20x4 I2C LCD, NTP time synchronization, and the OpenWeather API.

The project displays accurate Indian Standard Time, date, day, and real-time outdoor weather information retrieved from the internet.

---

## Version

**V1.1**

---

## Features

- Real-time clock using NTP
- Indian Standard Time (IST)
- Automatic date display
- Automatic day display
- Wi-Fi connectivity
- OpenWeather API integration
- Outdoor temperature
- Feels-like temperature
- Humidity
- Current weather condition
- JSON parsing using ArduinoJson
- Automatic weather update every 10 minutes
- 20x4 I2C LCD display
- Secure credential separation using `secrets.h`
- GitHub-safe `.gitignore` configuration

---

## Hardware

- ESP32 DevKit V1
- JHD204A 20x4 LCD
- I2C LCD backpack
- USB cable

---

## Software

- Arduino IDE
- ESP32 Arduino Core
- C++ / Arduino Programming
- LiquidCrystal_I2C
- ArduinoJson
- HTTPClient
- WiFi library
- OpenWeather API
- NTP

---

## Communication Interfaces

### I2C

The LCD communicates with the ESP32 using the I2C interface.

| LCD | ESP32 |
|---|---|
| GND | GND |
| VCC | 5V / VIN |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

### I2C Address

```text
0x27


## System Architecture
```
                    ┌─────────────────┐
                    │      ESP32      │
                    │                 │
                    │ Wi-Fi + Logic   │
                    └────────┬────────┘
                             │
                ┌────────────┴────────────┐
                │                         │
                ▼                         ▼
        ┌───────────────┐        ┌─────────────────┐
        │   NTP Server  │        │ OpenWeather API │
        │               │        │                 │
        │ Time          │        │ Temperature     │
        │ Date          │        │ Feels-like      │
        │ Day           │        │ Humidity        │
        └───────┬───────┘        │ Condition       │
                │                └────────┬────────┘
                │                         │
                │                         ▼
                │                ┌─────────────────┐
                │                │   JSON Data     │
                │                └────────┬────────┘
                │                         │
                │                ┌────────▼────────┐
                │                │   ArduinoJson   │
                │                │     Parser      │
                │                └────────┬────────┘
                │                         │
                └────────────┬────────────┘
                             ▼
                    ┌─────────────────┐
                    │  I2C Interface  │
                    └────────┬────────┘
                             ▼
                    ┌─────────────────┐
                    │ 20x4 LCD        │
                    │ JHD204A         │
                    └─────────────────┘
```

### LCD Display

The current display format is:
```
TIME 03:42:15
DATE 30/09/2026
DAY  Wednesday
TEMP 33C FL38C H53%
```
### Display Information

| Display | Meaning                |
| ------- | ---------------------- |
| `TIME`  | Current time           |
| `DATE`  | Current date           |
| `DAY`   | Current day            |
| `TEMP`  | Outdoor temperature    |
| `FL`    | Feels-like temperature |
| `H`     | Relative humidity      |

Example:
```
TEMP 33C FL38C H53%
```
means:
```
Temperature      = 33°C
Feels Like       = 38°C
Humidity         = 53%
```

## System
```
ESP32
 ├── NTP → Time / Date / Day
 └── OpenWeather API → JSON → Weather Data
                         ↓
                      I2C LCD
```

### Security

API and Wi-Fi credentials are stored in ```secrets.h``` and excluded from GitHub using ```.gitignore.```



### Status

V1.1 — Successfully Tested

- Wi-Fi: Working
- NTP: Working
- LCD: Working
- Weather API: Working
- JSON parsing: Working
- Temperature: Working
- Feels-like: Working
- Humidity: Working

### Future Upgrades

- Weather icons
- Wi-Fi reconnection
- Better error handling
- Weather forecast
- Custom PCB
- OTA updates

## Author

Rupam Ghosh

B.Tech — Electronics & Communication Engineering
