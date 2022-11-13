#pragma once

#include "secrets.h"

#define DHT_TYPE DHT11
#define DHT_PIN 5 // D1; output from DHT11 sensor
#define RDY_PIN 4 // D2; LED indicating startup finished

#define READ_INTERVAL_MS 10000
#define READ_RETRY 3

#define HTTP_PORT 80
#define PROMETHEUS_ENDPOINT "/metrics"

char const* PROMETHEUS_TEMPLATE = 
    "# HELP cricket_temperature_fahrenheit Temperature\n"
    "# TYPE cricket_temperature_fahrenheit gauge\n"
    "# UNIT cricket_temperature_fahrenheit \u00B0F\n"
    "cricket_temperature_fahrenheit{app=\"cricket\"} %f\n\n"
    "# HELP cricket_humidity_percent Humidity\n"
    "# TYPE cricket_humidity_percent gauge\n"
    "# UNIT cricket_humidity_percent %%\n"
    "cricket_humidity_percent{app=\"cricket\"} %f\n";
