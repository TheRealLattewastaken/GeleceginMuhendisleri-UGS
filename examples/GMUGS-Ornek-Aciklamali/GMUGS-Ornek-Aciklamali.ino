#include <WiFi.h>
#include <WebServer.h>
#include "ESP32_OTA_Server.h"
// Bunun altına başka "#include" veya "#define" kullanma!

WebServer server(80);
ESP32_OTA_Server gm(server);
// Bundan sonra hemen setup gelmeli!!!!!!!
void setup() {
    Serial.begin(115200);
    WiFi.begin("wifi-ad", "wifi-sifre");
    while (WiFi.status() != WL_CONNECTED) delay(500);
    gm.begin(); // OTA Sunucusunu Başlat

    // Setup kodunu bunun altına koy!
}

void loop() {
    gm.handleClient();
    
    // Loop kodunu bunun altına koy!!!
}
