#ifndef ESP32_OTA_SERVER_H
#define ESP32_OTA_SERVER_H

#include <WiFi.h>
#include <WebServer.h>
#include <Update.h>

class ESP32_OTA_Server {
  public:
    ESP32_OTA_Server(WebServer& srv);
    void begin();
    void handleClient();

  private:
    WebServer* _server;
    const char* _ota_html;

    void handleRoot();
    void handleUpdate();
};

#endif