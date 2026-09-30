#include <WiFi.h>
#include "time.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "secrets.h"

// ========================================
// Wi-Fi
// ========================================

//* const char* ssid = "YOUR_WIFI_NAME";             #add yours wifi name
//* const char* password = "YOUR_WIFI_PASSWORD";     #add yours wifi pasword

// ========================================
// OpenWeather API
// ========================================

//* const char* apiKey = "YOUR_OPENWEATHER_API_KEY";   #add yours api key

// Kolkata coordinates
const float latitude = 22.5726;
const float longitude = 88.3639;

// ========================================
// Weather variables
// ========================================

float weatherTemperature = 0.0;
float weatherFeelsLike = 0.0;
int weatherHumidity = 0;
String weatherCondition = "WAITING";
// ========================================
// LCD
// ========================================

LiquidCrystal_I2C lcd(0x27, 20, 4);

// ========================================
// Indian Standard Time
// UTC + 5:30
// ========================================

const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 0;

// ========================================
// Day names
// ========================================

const char* days[] =
{
  "Sunday",
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday"
};

// ========================================
// Clock timing
// ========================================

unsigned long previousClockMillis = 0;

const unsigned long clockInterval = 1000;

// ========================================
// Weather timing
// ========================================

unsigned long previousWeatherMillis = 0;

// Update weather every 10 minutes
const unsigned long weatherInterval = 600000;

// ========================================
// Remember previously displayed date/day
// ========================================

int lastDay = -1;
int lastMonth = -1;
int lastYear = -1;
int lastWeekday = -1;


// ========================================
// SETUP
// ========================================

void setup()
{
  Serial.begin(115200);

  // ======================================
  // Start LCD
  // ======================================

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("ESP32 WEATHER CLOCK");

  lcd.setCursor(0, 1);
  lcd.print("Connecting WiFi...");

  // ======================================
  // Connect to Wi-Fi
  // ======================================

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected!");

  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  // ======================================
  // Configure NTP
  // ======================================

  configTime(
    gmtOffset_sec,
    daylightOffset_sec,
    "pool.ntp.org"
  );

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("ESP32 WEATHER CLOCK");

  lcd.setCursor(0, 1);
  lcd.print("Synchronizing...");

  // Give NTP time to synchronize
  delay(2000);

  lcd.clear();

  // ======================================
  // Force first date/day update
  // ======================================

  lastDay = -1;
  lastMonth = -1;
  lastYear = -1;
  lastWeekday = -1;

  // ======================================
  // Get weather immediately
  // ======================================

  getWeather();

  // Force the first weather update timer
  previousWeatherMillis = millis();
}


// ========================================
// LOOP
// ========================================

void loop()
{
  unsigned long currentMillis = millis();

  // ======================================
  // Update clock every second
  // ======================================

  if (currentMillis - previousClockMillis >= clockInterval)
  {
    previousClockMillis = currentMillis;

    updateClock();
  }

  // ======================================
  // Update weather every 10 minutes
  // ======================================

  if (currentMillis - previousWeatherMillis >= weatherInterval)
  {
    previousWeatherMillis = currentMillis;

    getWeather();
  }
}


// ========================================
// UPDATE CLOCK
// ========================================

void updateClock()
{
  struct tm timeinfo;

  // ======================================
  // Get current NTP time
  // ======================================

  if (!getLocalTime(&timeinfo))
  {
    lcd.setCursor(0, 0);
    lcd.print("TIME ERROR          ");

    Serial.println("Failed to obtain time");

    return;
  }


  // ======================================
  // LINE 1 - TIME
  // ======================================

  char timeBuffer[21];

  snprintf(
    timeBuffer,
    sizeof(timeBuffer),
    "TIME %02d:%02d:%02d",
    timeinfo.tm_hour,
    timeinfo.tm_min,
    timeinfo.tm_sec
  );

  lcd.setCursor(0, 0);
  lcd.print(timeBuffer);


  // ======================================
  // LINE 2 - DATE
  // Only update when date changes
  // ======================================

  if (
    timeinfo.tm_mday != lastDay ||
    timeinfo.tm_mon != lastMonth ||
    timeinfo.tm_year != lastYear
  )
  {
    char dateBuffer[21];

    snprintf(
      dateBuffer,
      sizeof(dateBuffer),
      "DATE %02d/%02d/%04d",
      timeinfo.tm_mday,
      timeinfo.tm_mon + 1,
      timeinfo.tm_year + 1900
    );

    lcd.setCursor(0, 1);
    lcd.print(dateBuffer);

    lastDay = timeinfo.tm_mday;
    lastMonth = timeinfo.tm_mon;
    lastYear = timeinfo.tm_year;
  }


  // ======================================
  // LINE 3 - DAY
  // Only update when weekday changes
  // ======================================

  if (timeinfo.tm_wday != lastWeekday)
  {
    char dayBuffer[21];

    snprintf(
      dayBuffer,
      sizeof(dayBuffer),
      "DAY  %s",
      days[timeinfo.tm_wday]
    );

    lcd.setCursor(0, 2);
    lcd.print(dayBuffer);

    lastWeekday = timeinfo.tm_wday;
  }


  // ======================================
  // LINE 4 - WEATHER
  // ======================================

  char weatherBuffer[21];

  snprintf(
  weatherBuffer,
  sizeof(weatherBuffer),
  "TEMP %.0fC FL%.0fC H%d%%",
  weatherTemperature,
  weatherFeelsLike,
  weatherHumidity
);

  lcd.setCursor(0, 3);
  lcd.print("                    ");

  lcd.setCursor(0, 3);
  lcd.print(weatherBuffer);
}


// ========================================
// GET WEATHER
// ========================================

void getWeather()
{
  Serial.println();
  Serial.println("Getting weather...");

  // ======================================
  // Check Wi-Fi
  // ======================================

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi disconnected!");

    weatherCondition = "NO WIFI";

    return;
  }


  // ======================================
  // Create API URL
  // ======================================

  String url =
    "https://api.openweathermap.org/data/2.5/weather?lat=";

  url += String(latitude, 4);

  url += "&lon=";

  url += String(longitude, 4);

  url += "&appid=";

  url += apiKey;

  url += "&units=metric";


  Serial.print("Request URL: ");
  Serial.println("https://api.openweathermap.org/data/2.5/weather");


  // ======================================
  // Create HTTP client
  // ======================================

  HTTPClient http;

  http.begin(url);


  // ======================================
  // Send GET request
  // ======================================

  int httpCode = http.GET();

  Serial.print("Weather HTTP Code: ");
  Serial.println(httpCode);


  // ======================================
  // Successful response
  // ======================================

  if (httpCode == 200)
  {
    String payload = http.getString();

    Serial.println("Weather API response received.");


    // ====================================
    // Parse JSON
    // ====================================

    JsonDocument doc;

    DeserializationError error =
      deserializeJson(doc, payload);


    if (error)
    {
      Serial.print("JSON parsing failed: ");
      Serial.println(error.c_str());

      http.end();

      return;
    }


    // ====================================
    // Extract temperature
    // ====================================

    weatherTemperature =
  doc["main"]["temp"].as<float>();

    weatherFeelsLike =
  doc["main"]["feels_like"].as<float>();


    // ====================================
    // Extract weather condition
    // ====================================

    weatherCondition =
  doc["weather"][0]["main"].as<String>();

    weatherHumidity =
  doc["main"]["humidity"].as<int>();


    // ====================================
    // Serial output
    // ====================================

    Serial.print("Temperature: ");
Serial.print(weatherTemperature);
Serial.println(" C");

Serial.print("Feels Like: ");
Serial.print(weatherFeelsLike);
Serial.println(" C");

Serial.print("Humidity: ");
Serial.print(weatherHumidity);
Serial.println(" %");

Serial.print("Condition: ");
Serial.println(weatherCondition);
  }


  // ======================================
  // HTTP request failed
  // ======================================

  else
  {
    Serial.print("Weather request failed: ");
    Serial.println(http.errorToString(httpCode));

    weatherCondition = "ERROR";
  }


  // ======================================
  // Close HTTP connection
  // ======================================

  http.end();
}