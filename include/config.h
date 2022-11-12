#pragma once

#include "secrets.h"

#define DHT_TYPE DHT11
#define DHT_PIN 5 // D1; output from DHT11 sensor
#define RDY_PIN 4 // D2; LED indicating startup finished

#define READ_INTERVAL_MS 10000
#define READ_RETRY 3

#define HTTP_PORT 80
#define PROMETHEUS_ENDPOINT "/metrics"
#define PROMETHEUS_NAMESPACE "cricket"

char const* PROMETHEUS_TEMPLATE = 
    "# HELP " PROMETHEUS_NAMESPACE "_temperature_fahrenheit Temperature.\n"
    "# TYPE " PROMETHEUS_NAMESPACE "_temperature_fahrenheit gauge\n"
    "# UNIT " PROMETHEUS_NAMESPACE "_temperature_fahrenheit \u00B0F\n"
    PROMETHEUS_NAMESPACE "_temperature_fahrenheit %f\n"
    "# HELP " PROMETHEUS_NAMESPACE "_humidity_percent Humidity.\n"
    "# TYPE " PROMETHEUS_NAMESPACE "_humidity_percent gauge\n"
    "# UNIT " PROMETHEUS_NAMESPACE "_humidity_percent %%\n"
    PROMETHEUS_NAMESPACE "_humidity_percent %f\n";
