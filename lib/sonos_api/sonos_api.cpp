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

String SonosAPI::adjust_zone_volume(String zone, int adjustment_amt) {
  Serial.print("Adjusting zone: ");
  Serial.println(zone);

  String add_pos_sign = "";
  if (adjustment_amt > 0)
    add_pos_sign = "+";

  return make_request("/" + url_encode(zone) + "/volume/" + add_pos_sign + adjustment_amt);
}

String SonosAPI::mute_zone(String zone) {
  Serial.print("Toggling mute in zone: ");
  Serial.println(zone);

  return make_request("/" + url_encode(zone) + "/togglemute");
}

/*
 * Source: http://www.icosaedro.it/apache/urlencode.c
 * Via: http://hardwarefun.com/tutorials/url-encoding-in-arduino
 */
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
  String response;
  WiFiClient client;
  HTTPClient http;

  String http_uri = F("http://");
  http_uri += _hostname;
  http_uri += F(":");
  http_uri += _port;
  http_uri += path;

  Serial.print(F("GET "));
  Serial.println(http_uri);
  if (http.begin(client, http_uri)) {
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.printf_P(PSTR("[HTTP] GET response code: %d\n"), httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        Serial.println(payload);
        return payload;
      }
    } else {
      Serial.printf_P(PSTR("[HTTP] GET failed, error: %s\n"), http.errorToString(httpCode).c_str());
      return F("[HTTP] GET failed");
    }
  } else {
    Serial.println(F("[HTTP] Unable to connect!"));
    return F("[HTTP] Unable to connect!");
  }

  return F("NOT GONNA GET HERE!");
}

