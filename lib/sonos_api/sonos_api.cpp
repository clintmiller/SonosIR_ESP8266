#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "sonos_api.h"

void SonosAPI::begin(String hostname, int port) {
  _hostname = hostname;
  _port = port;
  
  Serial.print("Sonos API endpoint: ");
  Serial.print(_hostname);
  Serial.print(":");
  Serial.println(_port);
}

void SonosAPI::adjust_zone_volume(String zone, int adjustment_amt) {
  Serial.print("Adjusting zone: ");
  Serial.println(zone);

  String add_pos_sign = "";
  if (adjustment_amt > 0)
    add_pos_sign = "+";

  make_request("/" + url_encode(zone) + "/volume/" + add_pos_sign + adjustment_amt);
}

void SonosAPI::mute_zone(String zone) {
  Serial.print("Toggling mute in zone: ");
  Serial.println(zone);

  make_request("/" + url_encode(zone) + "/togglemute");
}

String SonosAPI::url_encode(String unencoded)
{
    const char *msg = unencoded.c_str();
    const char *hex = "0123456789abcdef";
    String encodedMsg = "";

    while (*msg!='\0'){
        if( ('a' <= *msg && *msg <= 'z')
                || ('A' <= *msg && *msg <= 'Z')
                || ('0' <= *msg && *msg <= '9') ) {
            encodedMsg += *msg;
        } else {
            encodedMsg += '%';
            encodedMsg += hex[*msg >> 4];
            encodedMsg += hex[*msg & 15];
        }
        msg++;
    }
    return encodedMsg;
}

String SonosAPI::make_request(String path) {
  // We don't care about the response
  String response;
  
  if (!_client.connect(_hostname.c_str(), _port)) {
    Serial.println("connection failed");
    return "connection failed!";
  }

  Serial.print("Requesting URL: ");
  Serial.println(path);

  _client.print(String("GET ") + path + " HTTP/1.1\r\n" +
               "Host: " + _hostname + "\r\n\r\n");

  return response;
}

