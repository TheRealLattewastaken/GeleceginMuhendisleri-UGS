#ifndef UGS_H
#define UGS_H

#include <WiFi.h>
#include <WebServer.h>
#include <Update.h>

class UGS {
  public:
    UGS(WebServer& srv);
    void begin();
    void handleClient();

  private:
    WebServer* _server;
    const char* _ota_html;

    void handleRoot();
    void handleUpdate();
};

#endif
