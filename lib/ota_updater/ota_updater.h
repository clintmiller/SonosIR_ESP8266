#include "WString.h"
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

class OTAUpdater {
  public:
    void begin(String hostname, String password, int port);
    void handle();
};
