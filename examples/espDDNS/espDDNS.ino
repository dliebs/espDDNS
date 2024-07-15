//
// espDDNS v1.0.0
// 2023.12.13
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

/*----------  Program Variables  ----------*/

espDDNS ddnsHost("subdomain", "domain.com", "apikey", "secretapikey", 15);  // 15 minute intervals

/*----------  Main Functions  ----------*/

void setup() {
  Serial.begin(115200);
  Serial.println("");
  connectWiFi();
  ddnsHost.performUpdate();
}

void loop() {
  ddnsHost.update();
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
