#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>
#include <LittleFS.h>

#include "secrets.h"

#define DHT_PIN  5
#define DHT_TYPE DHT11

/*** globals ***/
DHT dht(DHT_PIN, DHT_TYPE);

float temperature = 0.0;
float humidity = 0.0;

unsigned long prevMs = 0;
const long interval = 10000;  // 10s

void initWifi() {
    WiFi.begin(_WIFI_SSID, _WIFI_PASS);
    Serial.println("Connecting to WiFi");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println(".");
    }
    Serial.println(WiFi.localIP());
}

void readDHT() {
    float newTemp = dht.readTemperature(true); // true => as fahrenheit
    if (isnan(newTemp)) {
        Serial.println("Failed to read temperature.");
    } else {
        temperature = newTemp;
        Serial.printf("Temperature => %f\n", temperature);
    }

    float newHumid = dht.readHumidity();
    if (isnan(newHumid)) {
        Serial.println("Failed to read humidity.");
    } else {
        humidity = newHumid;
        Serial.printf("Humidity => %f\n", humidity);
    }
}

void setup() {
    Serial.begin(115200);
    // dht.begin();
    // initWifi();

    if (!LittleFS.begin()) {
        Serial.println("Error occurred while mounting LittleFS.");
        return;
    }
    File file = LittleFS.open("/test.txt", "r");
    if (!file) {
        Serial.println("Failed to open file.");
        return;
    }
    Serial.println("File content:");
    while (file.available()) {
        Serial.write(file.read());
    }
    file.close();
}

void loop() {
    unsigned long currentMs = millis();

    if (currentMs - prevMs >= interval) {
        prevMs = currentMs;
        // readDHT();
    }
}
