#include <WiFi.h>
#include <WebServer.h>
#include "ESP32_OTA_Server.h"

WebServer server(80);
ESP32_OTA_Server UGS(server);  // OTA sunucusu örneği

void setup() {
    Serial.begin(115200);

    // WiFi bağlantısı
    WiFi.begin("wifi-ad", "wifi-sifre");
    Serial.print("WiFi'ye bağlanıyor");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Bağlandı! IP: ");
    Serial.println(WiFi.localIP());

    UGS.begin(); // OTA sunucusunu başlat
}

void loop() {
    UGS.handleClient(); // HTTP isteklerini işleme
}
