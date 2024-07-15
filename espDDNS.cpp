//
// espDDNS - PorkBun Dynamic DNS Updater v1.0.2
// 2024.01.22
//
// Checks for current IP from https://domains.google.com/checkip
// Makes an HTTPS API Request https://porkbun.com/api/json/v3/documentation#DNS%20Edit%20Record%20by%20Domain%20and%20ID
// Based on https://github.com/ayushsharma82/EasyDDNS
// Added support for HTTPS https://arduino-esp8266.readthedocs.io/en/2.4.0/esp8266wifi/client-secure-examples.html
// Also thanks to https://randomnerdtutorials.com/esp8266-nodemcu-http-get-post-arduino/
//
// Future Plans:
//   - Add other DDNS services
//   - HTTPS Server Verification (Oops kinda skipped that... Do I need to check the server identity for it to work? No. Should I? I guessssss)
//
// Changes from 1.0.1
//  update -> updateRoutine
//

#include "espDDNS.h"

espDDNS::espDDNS(String ddns_subdomain, String ddns_domain, String ddns_apikey, String ddns_secretapikey, int ddns_updateInterval) {
  _ddns_subdomain = ddns_subdomain;
  _ddns_domain = ddns_domain;
  _ddns_apikey = ddns_apikey;
  _ddns_secretapikey = ddns_secretapikey;

  _ddns_update_url = "https://" + _ddns_server + "/api/json/v3/dns/editByNameType/" + _ddns_domain + "/A/" + _ddns_subdomain;
  _ddns_fullDomain = _ddns_subdomain + "." + _ddns_domain;

  _ddns_updateInterval = ddns_updateInterval;
  _ddns_interval_millis = 1000 * 60 * _ddns_updateInterval;
}

void espDDNS::updateRoutine() {
  if ( _ddns_lastUpdateTime + _ddns_interval_millis < millis() ) {
    performUpdate();
    // Update timer here
    _ddns_lastUpdateTime = millis();
    Serial.println("DDNS Updater for " + _ddns_fullDomain + " will run again in " + (String)_ddns_updateInterval + " minutes.");
  }
}

void espDDNS::performUpdate() {
  // Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    readDDNSIP();
    getNewIP();
    ddnsUpdate();
  }
  else {
    Serial.println("! WiFi disconnected, can't perform DDNS update !");
  }
}

// Reads the IP address of _ddns_fullDomain and writes the value to _ddns_ip
void espDDNS::readDDNSIP() {
  IPAddress result;
  int err = WiFi.hostByName(_ddns_fullDomain.c_str(), result);
  if (err == 1) {
    Serial.print("Current IP address for " + _ddns_fullDomain + " is ");
    Serial.println(result);
    _ddns_ip = result.toString();
  }
  else {
    Serial.print("Error Code: ");
    Serial.println(err);
    Serial.print("Result: ");
    Serial.println(result);
  }
}

// Reads the current IP address from Googles resolver and compares it to _ddns_ip
// If it does not match, writes the new IP to _ddns_ip and sets _ddns_needsUpdate to true
void espDDNS::getNewIP() {
  Serial.print("Retrieving external IP from Google: ");

  WiFiClientSecure client;
  client.setInsecure(); // Need to verify server identities in the future
  HTTPClient http;
  http.begin(client, "https://domains.google.com/checkip");

  int httpCode = http.GET();
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String newIP = http.getString();
      Serial.println(newIP);
      if (newIP == _ddns_ip) {
        Serial.println("External IP matches current IP for " + _ddns_fullDomain + ", no update needed.");
        _ddns_needsUpdate = false;
      }
      else {
        Serial.println("External IP DOES NOT match current IP for " + _ddns_fullDomain + ", UPDATE COMMENCING.");
        _ddns_ip = newIP;
        _ddns_needsUpdate = true;
      }
    }
  }
  else {
    Serial.println("HTTP Error Code: " + httpCode);
  }

  http.end();
}

// The update code
void espDDNS::ddnsUpdate() {
  if ( _ddns_needsUpdate ) {
    Serial.println("Attempting to update " + _ddns_fullDomain + " to " + _ddns_ip);

    WiFiClientSecure client;
    HTTPClient http;

    Serial.print("Connecting to Server... ");
    client.setInsecure();
    client.connect(_ddns_server, 443);

    // Your Domain name with URL path or IP address with path
    Serial.println("Updating IP");
    http.begin(client, _ddns_update_url);

    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST("{\"secretapikey\":\"" + _ddns_secretapikey + "\",\"apikey\":\"" + _ddns_apikey + "\",\"name\":\"" + _ddns_subdomain + "\",\"type\":\"A\",\"content\":\"" + _ddns_ip + "\",\"ttl\":\"3600\"}");

    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        String ddnsOutput = http.getString();
        Serial.println("Success!");
        Serial.print("HTTP Response Code: ");
        Serial.println(httpCode);
        Serial.println("Server Output: " + ddnsOutput);
      }
    }
    else {
      Serial.println("! Failure to update DDNS IP !");
      Serial.println("HTTP Error Code: " + httpCode);
    }

    // Free resources
    http.end();
    Serial.println("Update Completed, will check again in " + (String)_ddns_updateInterval + " minutes.");
  }
  else {
    Serial.println("Will check again in " + (String)_ddns_updateInterval + " minutes.");
  }
}
