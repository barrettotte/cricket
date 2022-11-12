#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

#include "secrets.h"

#define DHT_TYPE DHT11
#define DHT_PIN 5 // D1
#define RDY_PIN 4 // D2
#define SERVER_PORT 80

unsigned long prevMs = 0;
const long interval = 15000; // 15s

float temperature = 0.0;
float humidity = 0.0;

AsyncWebServer server(SERVER_PORT);
DHT dht(DHT_PIN, DHT_TYPE);
// NOTE: DHT can't be read in async server methods.
//   DHT uses yield() which panics when in server method.

void updateTemperature() {
    float t = dht.readTemperature(true); // true => as fahrenheit
    if (isnan(t)) {
        Serial.println("Failed to read temperature.");
        t = 0.0;
    }
    temperature = t;
}

void updateHumidity() {
    float h = dht.readHumidity();
    if (isnan(h)) {
        Serial.println("Failed to read humidity.");
        h = 0.0;
    }
    humidity = h;
}

String valueInjector(const String& target) {
    if (target == "TEMPERATURE") {
        return String(temperature);
    } else if (target == "HUMIDITY") {
        return String(humidity);
    } else {
        Serial.printf("Invalid target. %s\n", target.c_str());
        return String("???");
    }
}

void initServer() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *req) {
        req->send(LittleFS, "/index.html", String(), false, valueInjector);
    });
    server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *req) {
        req->send(LittleFS, "/styles.css", "text/css");
    });
    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *req) {
        req->send_P(200, "text/plain", String(temperature).c_str());
    });
    server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *req) {
        req->send_P(200, "text/plain", String(humidity).c_str());
    });
    // TODO: /log
    server.begin();
    printf("Server listening on port %d...\n", SERVER_PORT);
}

void initWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(_WIFI_SSID, _WIFI_PASS);
    Serial.printf("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.printf(".");
    }
    Serial.println(WiFi.localIP());
}

void initSerial() {
    Serial.begin(9600);
    Serial.println();

    for(uint8_t t = 3; t > 0; t--){
        Serial.printf("WAIT %d...\n", t);
        Serial.flush();
        delay(500);
    }
    Serial.println("\n* * * START * * *");
}

void setup() {
    pinMode(RDY_PIN, OUTPUT);
    digitalWrite(RDY_PIN, LOW);

    initSerial();
    initWifi();
    dht.begin();

    if (!LittleFS.begin()) {
        Serial.println("Error occurred while mounting LittleFS.");
        return;
    }
    initServer();
    digitalWrite(RDY_PIN, HIGH);
}

void loop() {
    unsigned long currentMs = millis();

    if (currentMs - prevMs >= interval) {
        prevMs = currentMs;
        updateTemperature();
        updateHumidity();
        Serial.printf("Temperature=%f,Humidity=%f\n", temperature, humidity);
    }
}
