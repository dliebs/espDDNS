//
// espDDNS v1.0.1
// 2023.01.16
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
    espDDNS(String, String, String, String, int);
    void update();
    void performUpdate();
  private:
    // Porkbun API Variables
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

    void readDDNSIP();
    void getNewIP();
    void ddnsUpdate();
};

#endif
