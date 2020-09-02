/*
 * SonosIR_ESP8266: Translate IR remote codes for volume control into Sonos volume adjustments
 * An IR photo sensor must be connected to the input RECV_PIN.
 * Version 0.1 Mar 4, 2017
 * 
 * 
 */

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <ESP8266WiFi.h>
#include <ota_updater.h>
#include <sonos_api.h>

const char* WIFI_SSID     = WIFISSID;
const char* WIFI_PASSWORD = WIFIPASS;

const String OTA_HOSTNAME = "sonos-ir";
const String OTA_PASSWORD = OTA_UPDATE_PASSWORD;
const int    OTA_PORT     = 8266;

const String SONOS_API_HOST = SONOSAPIHOST;
const int    SONOS_API_PORT = 5005;
const String ZONE_NAME      = SONOSZONENAME;
const int    ADJAMT         = 5;

//an IR photo sensor is connected to GPIO pin 2
int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
OTAUpdater OTA;
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

  // Setup OTA Updates
  OTA.begin(OTA_HOSTNAME, OTA_PASSWORD, OTA_PORT);

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
  OTA.handle();

  if (irrecv.decode(&results)) {
    if (results.decode_type == JVC) {
      
      String res;
      // Figure out what to do based on the remote code
      if (results.value == 0xC578) {
        Serial.println("!!volume up");
        res = Sonos.adjust_zone_volume(ZONE_NAME, ADJAMT);
      } else if (results.value == 0xC5F8) {
        Serial.println("!!volume down");
        res = Sonos.adjust_zone_volume(ZONE_NAME, -ADJAMT);
      } else if (results.value == 0xC538) {
        Serial.println("!!toggle mute");
        res = Sonos.mute_zone(ZONE_NAME);
      }

      if (res == "connection failed") {
        delay(5000);
        return;
      }
    } else {
      // Serial.println("Not implemented");
    }

    
    irrecv.resume(); // Receive the next value
  }
}
