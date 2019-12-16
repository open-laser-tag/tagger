// build from https://github.com/espressif/arduino-esp32/blob/master/libraries/ArduinoOTA/examples/BasicOTA/BasicOTA.ino released under LPGL-2.1

#define TIME_WAITING_FOR_CONNECTION_IN_MS 10000

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <esp_log.h>

int init_ota(const char* ssid, const char* pw);
void handle_ota();
