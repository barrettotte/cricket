#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

#include "config.h"

unsigned long prevReadMs = 0;
float temperature = 0.0;
float humidity = 0.0;

AsyncWebServer server(HTTP_PORT);
DHT dht(DHT_PIN, DHT_TYPE);
// NOTE: DHT can't be read in async server methods.
//   DHT uses yield() which panics when in async handler.

bool getSensorData(float (*readFn)(), float* val) {
    for (uint8_t i = 0; i < READ_RETRY; i++) {
        *val = readFn();
        if (!isnan(*val)) {
            return true;
        }
    }
    return false;
}

float readTemperature() {
    bool ok = getSensorData([]{return dht.readTemperature(true);}, &temperature);
    if (!ok) {
        Serial.println("Failed to read temperature.");
    }
    return temperature;
}

float readHumidity() {
    bool ok = getSensorData([]{return dht.readHumidity();}, &humidity);
    if (!ok) {
        Serial.println("Failed to read humidity.");
    }
    return humidity;
}

String getSensorJson() {
    char json[128];
    snprintf(json, 128, "{\"temperature\":%f,\"humidity\":%f}", temperature, humidity);
    return String(json);
}

void readSensors() {
    unsigned long currentMs = millis();
    if (currentMs - prevReadMs >= READ_INTERVAL_MS) {
        prevReadMs = currentMs;
        readTemperature();
        readHumidity();
        Serial.println(getSensorJson());
    }
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

void handlePrometheusRequest(AsyncWebServerRequest* req) {
    if (isnan(temperature) || isnan(humidity)) {
        req->send_P(500, "text/plain; charset=utf-8", "Sensor data error.");
    } else {
        char resp[1024];
        snprintf(resp, 1024, PROMETHEUS_TEMPLATE, temperature, humidity);
        req->send(200, "text/plain; charset=utf-8", resp);
    }
}

void initServer() {
    server.onNotFound([](AsyncWebServerRequest* req) {
        req->send_P(404, "text/plain; charset=utf-8", "Page not found.");
    });
    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest* req) {
        req->send_P(200, "text/plain; charset=utf-8", String(temperature).c_str());
    });
    server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest* req) {
        req->send_P(200, "text/plain; charset=utf-8", String(humidity).c_str());
    });
    server.on("/sensors", HTTP_GET, [](AsyncWebServerRequest* req) {
        req->send_P(200, "application/json; charset=utf-8", getSensorJson().c_str());
    });
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* req) {
        req->send(LittleFS, "/index.html", String(), false, valueInjector);
    });
    server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest* req) {
        req->send(LittleFS, "/styles.css", "text/css");
    });
    server.on(PROMETHEUS_ENDPOINT, HTTP_GET, handlePrometheusRequest);
    server.begin();
    Serial.printf("Server listening on port %d...\n", HTTP_PORT);
}

void initWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(_WIFI_SSID, _WIFI_PASS);
    Serial.printf("Connecting to WiFi [%s]", _WIFI_SSID);

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
        while (1) {}
    }
    initServer();
    digitalWrite(RDY_PIN, HIGH);
}

void loop() {
    readSensors();
}
