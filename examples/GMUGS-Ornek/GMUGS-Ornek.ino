#include <WiFi.h>
#include <WebServer.h>
#include "UGS.h"  // Yeni kütüphane adı

// WiFi bilgilerini buraya yaz
const char* ssid = "wifi-ad";
const char* password = "wifi-sifre";

WebServer server(80);
UGS ugs(server);  // 'gm' yerine artık 'ugs'

void setup() {
    Serial.begin(115200);

    // WiFi bağlanma
    WiFi.begin(ssid, password);
    Serial.print("WiFi'ye bağlanıyor");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Bağlandı! IP: ");
    Serial.println(WiFi.localIP());

    ugs.begin(); // OTA sunucusunu başlat
}

void loop() {
    ugs.handleClient(); // HTTP isteklerini işle
}
