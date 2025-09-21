#include <WiFi.h>
#include <WebServer.h>
#include "ESP32_OTA_Server.h"

WebServer server(80);
ESP32_OTA_Server gm(server);

void setup() {
    Serial.begin(115200);
    WiFi.begin("wifi-ad", "wifi-sifre");
    while (WiFi.status() != WL_CONNECTED) delay(500);

    gm.begin(); // Start OTA server
}

void loop() {
    gm.handleClient();
}
