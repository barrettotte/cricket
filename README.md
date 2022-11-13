# cricket

Temperature/Humidity sensor metrics in Prometheus with Discord alerting and Grafana.

## Summary

TODO:

## Hardware

- 1x ESP8266
- 1x DHT11 Temperature Humidity Sensor Module
- 1x Red LED
- 1x 470Ω resistor

## Infra

- `nginx` - Reverse proxy to local IP of ESP8266
- `prometheus` - Metric store for temperature/humidity data 
- `grafana` - Humidity/temperature dashboards over Prometheus
- `alertmanager` - Manages alert notification for alerts firing in Prometheus
- `discord-alerts` - Send alerts to Discord Web Hook
- `cadvisor` - Provide metrics about cgroups

## References

- ESP8266
  - https://randomnerdtutorials.com/esp8266-web-server-spiffs-nodemcu/
  - https://randomnerdtutorials.com/esp8266-nodemcu-vs-code-platformio-littlefs/
- Prometheus/Grafana
  - https://dzlab.github.io/monitoring/2021/12/30/monitoring-stack-docker/
  - https://github.com/grafana/prometheus-arduino ... note: does not support ESP8266
  - https://github.com/prometheus/docs/blob/main/content/docs/instrumenting/exposition_formats.md
  - https://www.amazon.com/Prometheus-Infrastructure-Application-Performance-Monitoring
- https://github.com/benjojo/alertmanager-discord
- https://docs.platformio.org/en/latest/core/installation/
- https://diarmuid.ie/blog/using-nginx-as-a-reverse-proxy-for-developing-microservice-with-docker-compose
