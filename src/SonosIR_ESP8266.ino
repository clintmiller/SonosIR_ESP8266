/*
 * SonosIR_ESP8266: Translate IR remote codes for volume control into Sonos volume adjustments
 * An IR photo sensor must be connected to the input RECV_PIN.
 * Version 0.1 Mar 4, 2017
 * 
 * 
 */

#include <IRremoteESP8266.h>
#include <ESP8266WiFi.h>
#include "sonos_api.h"
 
const char* WIFI_SSID     = "YOUR_SSID";
const char* WIFI_PASSWORD = "YOUR_PASSWORD";

const String SONOS_API_HOST = "YOUR_SONOS_API_HOSTNAME";
const int    SONOS_API_PORT = 5005;
const String ZONE_NAME      = "YOUR_SONOS_ZONE_NAME";
const int    ADJAMT         = 5;

int RECV_PIN = 2; //an IR photo sensor is connected to GPIO pin 2
IRrecv irrecv(RECV_PIN);
SonosAPI Sonos;

decode_results results;

void setup()
{
  Serial.begin(9600);
  delay(100);
  
  // Start the receiver
  irrecv.enableIRIn();

  // Setup WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup Sonos
  Sonos.begin(SONOS_API_HOST, SONOS_API_PORT);
}

/*
 * JVC codes:
 *   C578 -> VOLUME UP
 *   C5F8 -> VOLUME DOWN
 *   C538 -> VOLUME MUTE (toggle)
 */

void loop() {
  if (irrecv.decode(&results)) {
    if (results.decode_type == JVC) {
      
      // Figure out what to do based on the remote code
      if (results.value == 0xC578) {
        Serial.println("!!volume up");
        Sonos.adjust_zone_volume(ZONE_NAME, ADJAMT);
      } else if (results.value == 0xC5F8) {
        Serial.println("!!volume down");
        Sonos.adjust_zone_volume(ZONE_NAME, -ADJAMT);
      } else if (results.value == 0xC538) {
        Serial.println("!!toggle mute");
        Sonos.mute_zone(ZONE_NAME);
      }
    } else {
      // Serial.println("Not implemented");
    }
    
    irrecv.resume(); // Receive the next value
  }
}
