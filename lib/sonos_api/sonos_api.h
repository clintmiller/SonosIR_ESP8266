#include "WString.h"
#include <ESP8266WiFi.h>

/*
 * This "client" of the non-existent Sonos API actually hits a running instance of
 * https://github.com/jishi/node-sonos-http-api, which is a friendly wrapper
 * around whatever "protocol" Sonos exposes over HTTP.
 */

class SonosAPI {
  public:
    SonosAPI() : _client() { };
    void begin(String hostname, int port);
    void adjust_zone_volume(String zone, int adjustment_amt); 
    void mute_zone(String zone);
  private:
    String make_request(String path);
    String url_encode(String unencoded);
    String _hostname;
    int _port;
    WiFiClient _client;
};
