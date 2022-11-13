# cricket

Humidity/temperature sensor metrics in Grafana and Prometheus with Discord alerting.

## Summary

TODO:

## Infra

- `nginx` - Reverse proxy to local IP of ESP8266
- `prometheus` - Metric store for temperature/humidity data 
- `grafana` - Humidity/temperature dashboards over Prometheus
- `alertmanager` - Manages alert firing
- `discord-alerts` - Send alerts to Discord Web Hook

## Hardware

- 1x ESP8266
- 1x DHT11 Temperature Humidity Sensor Module
- 1x Red LED
- 1x 470Ω resistor

## References

- ESP8266
  - https://randomnerdtutorials.com/esp8266-web-server-spiffs-nodemcu/
  - https://randomnerdtutorials.com/esp8266-nodemcu-vs-code-platformio-littlefs/
- Prometheus/Grafana
  - https://dzlab.github.io/monitoring/2021/12/30/monitoring-stack-docker/
  - https://github.com/grafana/prometheus-arduino ... note: does not support ESP8266
  - https://github.com/prometheus/docs/blob/main/content/docs/instrumenting/exposition_formats.md
- https://github.com/benjojo/alertmanager-discord
- https://docs.platformio.org/en/latest/core/installation/
- https://diarmuid.ie/blog/using-nginx-as-a-reverse-proxy-for-developing-microservice-with-docker-compose
