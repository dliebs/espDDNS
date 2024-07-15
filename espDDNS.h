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

#ifndef espDDNS_h
#define espDDNS_h

/*----------  Libraries  ----------*/

#include "Arduino.h"

#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClientSecure.h>
#endif

#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
  #include <WiFiClient.h>
#endif

class espDDNS {
  public:
    // Instanciate DDNS updater, ddns_updateInterval in minutes
    espDDNS(String, String, String, String, int);
    // DDNS Update routine, run every loop
    void updateRoutine();
    // Update DDNS, run once during setup and then updateRoutine manages interval in loop
    void performUpdate();
  private:
    // Porkbun DDNS API Variables
    String _ddns_subdomain;
    String _ddns_domain;
    String _ddns_apikey;
    String _ddns_secretapikey;
    int _ddns_updateInterval;

    String _ddns_server = "porkbun.com";
    String _ddns_ip = ""; // Placeholder for IP address

    String _ddns_update_url;
    String _ddns_fullDomain;

    // Timekeeping
    unsigned long _ddns_lastUpdateTime = 0;
    int _ddns_interval_millis;

    bool _ddns_needsUpdate = false;  // Don't want to assume an update is needed

    // Private Functions
    void readDDNSIP();
    void getNewIP();
    void ddnsUpdate();
};

#endif
