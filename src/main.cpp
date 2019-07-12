#if !defined(ESP8266) && !defined(ESP32)
#error This file is for ESP8266 and ESP32 only
#endif

#define COMPDATE __DATE__ " " __TIME__
#define INCLUDE_OTA

#include <Arduino.h>
/*
Dependency Graph
|-- <ESP Async WebServer> 1.2.0
|   |-- <ESPAsyncTCP> 1.2.0
|   |   |-- <ESP8266WiFi> 1.0
|   |-- <ESP8266WiFi> 1.0
|   |-- <Hash> 1.0
|-- <ESP8266WiFi> 1.0
|-- <ESP8266mDNS>
|   |-- <ESP8266WiFi> 1.0
|-- <ArduinoOTA> 1.0
|   |-- <ESP8266WiFi> 1.0
|   |-- <ESP8266mDNS>
|   |   |-- <ESP8266WiFi> 1.0
*/
// WiFi
#if defined(ESP8266)
  #include <ESP8266WiFi.h>           // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
  #include <ESPAsyncTCP.h>           // https://github.com/me-no-dev/ESPAsyncTCP
#elif defined(ESP32)
  #include <WiFi.h>                  // https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/src/WiFi.h
  #include <AsyncTCP.h>              // https://github.com/me-no-dev/AsyncTCP
#endif

// OTA
#if defined(INCLUDE_OTA)
  #if defined(ESP8266)
    #include <ESP8266mDNS.h>         // PlatformIO platform-espressif8266
  #elif defined(ESP32)
    #include <ESPmDNS.h>             // PlatformIO platform-espressif32
  #endif
  // For OTA: ESPmDNS + WiFiUdp + ArduinoOTA
  #include <WiFiUdp.h>               // PlatformIO platform-espressif8266 or platform-espressif32
  #include <ArduinoOTA.h>            // PlatformIO platform-espressif8266 or platform-espressif32
#endif

// Webserver
#include <ESPAsyncWebServer.h>       // https://github.com/me-no-dev/ESPAsyncWebServer
#include "webpage.h"

// Accesspoint Credentials
#include "secrets.h"
#ifndef SECRETS_H
 #define SECRETS_H
 const char WIFI_SSID[]     = "ssid";
 const char WIFI_PASSWORD[] = "password";
#endif

// Webserver
AsyncWebServer server(80);
String webpage = "";
char _hostname[12];
// WiFi
bool _wifiGotIP = false;
bool _wifiAvailable = false;
// Led
unsigned long previousMillis = 0;
const int intervalMillis = 100;
bool _ledOn = true;
#if defined(ARDUINO_ESP8266_ESP12)
const int ledPin = 2;
#else
const int ledPin = LED_BUILTIN;
#endif

// ********************  FUNCTIONS  ********************

String Hex4s(uint32_t value) {
  char buffer[9];
  snprintf(buffer, sizeof(buffer), "%08X", value);
  return String(buffer);
}

String Hex3s(uint32_t value) {
  char buffer[7];
  snprintf(buffer, sizeof(buffer), "%06X", value & 0x00FFFFFF);
  return String(buffer);
}

String FlashMode(void) {
  FlashMode_t ideMode = ESP.getFlashChipMode();
  return String(ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN");
}

void static setLed(bool newLed)
{
  _ledOn = newLed;
  digitalWrite(ledPin, _ledOn ? LOW : HIGH);
}

// Replace %xx% strings in html templates for specific value
String processor(const String& var)
{
  if(var == "ARDUINO_BOARD")      { return String(ARDUINO_BOARD); }
  else if(var == "ARDUINO")       { return String(ARDUINO); }
#if defined(ESP8266)
  else if(var == "ESP_")          { return String("ESP8266"); }
  else if(var == "ESP8266")       { return String(ESP8266); }
  else if(var == "CHIPID")        { return Hex3s(ESP.getChipId()); }
#elif defined(ESP32)
  else if(var == "ESP_")          { return String("ESP32"); }
  else if(var == "ESP32")         { return String(ESP32); }
  else if(var == "CHIPID")        { return Hex4s(ESP.getEfuseMac()); }
#endif
#if defined(ARDUINO_ARCH_ESP8266)
  else if(var == "ARDUINO_ARCH_ESP8266")  { return String(ARDUINO_ARCH_ESP8266); }
  else if(var == "ARDUINO_ARCH_")         { return String("ARDUINO_ARCH_ESP8266"); }
#elif defined(ARDUINO_ARCH_ESP32)
  else if(var == "ARDUINO_ARCH_ESP32")    { return String(ARDUINO_ARCH_ESP32); }
  else if(var == "ARDUINO_ARCH_")         { return String("ARDUINO_ARCH_ESP32"); }
#endif
#if defined(ESP8266_WEMOS_D1MINI)
  else if(var == "ESP8266_WEMOS_D1MINI")  { return String(ESP8266_WEMOS_D1MINI); }
#endif

  else if(var == "CPUFREQ")       { return String(ESP.getCpuFreqMHz()); }
  else if(var == "F_CPU")         { return String(F_CPU); }
  else if(var == "LED_BUILTIN")   { return String(LED_BUILTIN); }
  else if(var == "LED")           { return String(ledPin); }

  else if(var == "FLASHID")       { return Hex4s(ESP.getFlashChipId()); }
  else if(var == "FLASHSIZE")     { return String(ESP.getFlashChipSize()); }
  else if(var == "FLASHREALSIZE") { return String(ESP.getFlashChipRealSize()); }
  else if(var == "FLASHSPEED")    { return String(ESP.getFlashChipSpeed()); }
  else if(var == "FLASHMODE")     { return FlashMode(); }

  else if(var == "SDK")           { return String(ESP.getSdkVersion()); }
  else if(var == "CORE")          { return ESP.getCoreVersion(); }
  else if(var == "BOOTVER")       { return String(system_get_boot_version()); }
  else if(var == "SKETCHSPACE")   { return String(ESP.getFreeSketchSpace()); }
  else if(var == "SKETCHSIZE")    { return String(ESP.getSketchSize()); }
  else if(var == "SKETCHMD5")     { return ESP.getSketchMD5(); }

  else if(var == "RESETREASON")   { return ESP.getResetReason(); }
  else if(var == "FREEHEAP")      { return String(system_get_free_heap_size()); }

  // else if(var == "OTASIZE")       { return String(system_partition_get_ota_partition_size()); } v3.0

  else if(var == "MAC")           { return WiFi.macAddress(); }
  else if(var == "IP")            { return WiFi.localIP().toString(); }
  else if(var == "HOSTNAME")      { return String(_hostname); }
  else if(var == "AUTOCONNECT")   { return String(WiFi.getAutoConnect() ? "True" : "False"); }
  else if(var == "AUTORECONNECT") { return String(WiFi.getAutoReconnect() ? "True" : "False"); }
  else if(var == "APID")          { return String(wifi_station_get_current_ap_id()); }
  else if(var == "BSSID")         { return WiFi.BSSIDstr(); }
  else if(var == "PHY")           { const char* phymodes[] = { "", "B", "G", "N" }; return String(phymodes[(int) wifi_get_phy_mode()]); }
  else if(var == "CHANNEL")       { return String(wifi_get_channel()); }
  else if(var == "RSSI")          { return String(WiFi.RSSI()); }

  else if(var == "COMPDATE")      { return String(COMPDATE); }
  return String();
}

// Webserver root page
void root(AsyncWebServerRequest *request) {
  setLed(true);
  Serial.println("server /");
  AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", espboardinfo, processor);
  response->addHeader("Server","ESP Async Web Server");
  request->send(response);
  setLed(false);
}

// Webserver not Found page
void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

#if defined(INCLUDE_OTA)
// ********************  OTA  ********************

void OTA_onStart(void) {
  String type;
  if (ArduinoOTA.getCommand() == U_FLASH) {
    type = "sketch";
  } else { // U_SPIFFS
    type = "filesystem";
  }
  // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
  Serial.print("OTA Start: ");
  Serial.println(type);
}

void OTA_onEnd(void) {
  Serial.println("OTA End\n");
}

void OTA_onProgress(unsigned int progress, unsigned int total) {
  Serial.printf("OTA Progress: %u%%\n", (progress / (total / 100)));
}

void OTA_onError(ota_error_t error) {
  Serial.printf("Error[%u]: ", error);
  if (error == OTA_AUTH_ERROR) {
    Serial.println("Auth Failed");
  } else if (error == OTA_BEGIN_ERROR) {
    Serial.println("Begin Failed");
  } else if (error == OTA_CONNECT_ERROR) {
    Serial.println("Connect Failed");
  } else if (error == OTA_RECEIVE_ERROR) {
    Serial.println("Receive Failed");
  } else if (error == OTA_END_ERROR) {
    Serial.println("End Failed");
  }
}

#endif

char* Hostname(char hostname[]) {
#ifdef ESP32
  sprintf(hostname, "esp-%06x", ESP.getEfuseMac());
#elif defined(ESP8266)
  sprintf(hostname, "esp-%06x", ESP.getChipId());
#endif
return hostname;
}

// ********************  WIFI  ********************

void onSTAConnected(WiFiEventStationModeConnected e /*String ssid, uint8 bssid[6], uint8 channel*/) {
  Serial.printf("Connected to SSID %s @ bssid %.2X:%.2X:%.2X:%.2X:%.2X:%.2X channel %d\n",
    e.ssid.c_str(), e.bssid[0], e.bssid[1], e.bssid[2], e.bssid[3], e.bssid[4], e.bssid[5], e.channel);
 }

void onSTADisconnected(WiFiEventStationModeDisconnected e /*String ssid, uint8 bssid[6], WiFiDisconnectReason reason*/) {
  Serial.printf("Disconnected from SSID %s @ bssid %.2X:%.2X:%.2X:%.2X:%.2X:%.2X reason %d\n",
    e.ssid.c_str(), e.bssid[0], e.bssid[1], e.bssid[2], e.bssid[3], e.bssid[4], e.bssid[5], e.reason);

  setLed(true);
  delay(5000);
  ESP.restart();
}

void onSTAGotIP(WiFiEventStationModeGotIP e /*IPAddress ip, IPAddress mask, IPAddress gw*/) {
  Serial.printf("Got IP: %s mask %s gateway %s\n",
    e.ip.toString().c_str(), e.mask.toString().c_str(), e.gw.toString().c_str());

  _wifiGotIP = true;
}

// ********************  SETUP  ********************

void setup(){
  // WiFi
  static WiFiEventHandler e1, e2, e4;

  // LED
  pinMode(ledPin, OUTPUT);          // Onboard LED
  digitalWrite(ledPin, LOW);        // Switch on LED

  // Serial
  Serial.begin(115200);
  Serial.print(F("\nCongratulations with your new ESP Board!!\n"));

  #if defined(INCLUDE_OTA)
  // OTA
  ArduinoOTA.setHostname(Hostname(_hostname));
  ArduinoOTA.onStart(OTA_onStart);
  ArduinoOTA.onEnd(OTA_onEnd);
  ArduinoOTA.onProgress(OTA_onProgress);
  ArduinoOTA.onError(OTA_onError);
  #endif

  // WebServer
  server.onNotFound(notFound);
  server.on("/", HTTP_ANY, root);

  // WiFi events
  e1 = WiFi.onStationModeConnected(onSTAConnected);
  e2 = WiFi.onStationModeDisconnected(onSTADisconnected);
  e4 = WiFi.onStationModeGotIP(onSTAGotIP);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(false);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(false);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

}

// ********************  LOOP  ********************

void loop()
{
  if (_wifiGotIP) {
    _wifiGotIP = false;
    _wifiAvailable = true;
    setLed(false);
    #if defined(INCLUDE_OTA)
    ArduinoOTA.begin();
    Serial.println("ArduinoOTA begin");
    #endif
    server.begin();
    Serial.println("server begin");
  }
  if (_wifiAvailable) {
    #if defined(INCLUDE_OTA)
    ArduinoOTA.handle();
    #endif
  } else {
    if ((unsigned long)(millis() - previousMillis) >= intervalMillis) {
      setLed(!_ledOn);
      previousMillis = millis();
    }
  }
}
