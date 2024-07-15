//
//
//  espDDNS - PorkBun Dynamic DNS Updater v1.0.4
//  This version was not deployed [2024.01.22]
//
//  ESP8266/32 Based
//    Checks for current IP from https://domains.google.com/checkip
//    Makes an HTTPS API Request https://porkbun.com/api/json/v3/documentation#DNS%20Edit%20Record%20by%20Domain%20and%20ID
//    Based on https://github.com/ayushsharma82/EasyDDNS
//    Added support for HTTPS https://arduino-esp8266.readthedocs.io/en/2.4.0/esp8266wifi/client-secure-examples.html
//    Also thanks to https://randomnerdtutorials.com/esp8266-nodemcu-http-get-post-arduino/
//
//  Changes From Previous Version
//    Comments, cleanup
//    update -> updateRoutine
//
//  To Do
//    Add other DDNS services
//    HTTPS Server Verification (Oops kinda skipped that... Do I need to check the server identity for it to work? No. Should I? I guessssss)
//
//


/*----------  Libraries  ----------*/

#include <espDDNS.h>

/*----------  User Variables  ----------*/

// WiFi Credentials
#ifndef STASSID
  #define STASSID "Your_WiFi_SSID"
  #define STAPSK  "Your_WiFi_Pass"
#endif
#define WiFiHostname "espDDNS"

/*----------  DDNS Variables  ----------*/

espDDNS ddnsHost("subdomain", "domain.com", "apikey", "secretapikey", 15);  // 15 minute intervals

/*----------  Main Functions  ----------*/

void setup() {
  Serial.begin(115200);
  Serial.println("");
  connectWiFi();

  // Update DDNS, run once during setup and then updateRoutine manages interval in loop
  ddnsHost.performUpdate();
}

void loop() {
  // DDNS Update routine, run every loop
  ddnsHost.updateRoutine();
  delay(1000);
}

/*---------- WiFi Code ----------*/

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.hostname(WiFiHostname);
  WiFi.begin(STASSID, STAPSK);

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - startTime) <= 10000) // Timeout WiFi connection attempt after 10 sec
  {
    delay(500);
  }
}
