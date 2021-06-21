#if !defined(ESP8266)
#error This file is for ESP8266 only
#endif

// **************************************** compiler ****************************************
#define APPNAME "New ESP Board - ESP8266"
#include <version.h> // include BUILD_NUMBER, VERSION, VERSION_SHORT
//#define INCLUDE_OTA

// ********************  INCLUDE  ********************

#include <Arduino.h>
#include <wifimessaging.h>

// OTA
// ESPmDNS + WiFiUdp + ArduinoOTA
#if defined(INCLUDE_OTA)
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#endif

// Webserver
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer
#include "webpage.h"

// ********************  CONSTANTS  ********************

#if defined(LED_BUILTIN)
const int ledPin = LED_BUILTIN;
#else
const int ledPin = 2;
#endif

// ********************  GLOBALS  ********************

// Led
unsigned long previousMillis = 0;
const int intervalMillis = 100;
bool _ledOn = true;

// WiFi, MQTT and NTP
void callback(char *topic, byte *payload, unsigned int length);
WifiMessaging myWM(WifiMessaging::connectionService::ServiceWifi |
                       WifiMessaging::connectionService::ServiceMQTT |
                       WifiMessaging::connectionService::ServiceNTP,
                   callback);

// message
bool sentMessage = false;
const char *MQTT_TOPIC = "devices/newboard";
const char *MQTT_PAYLOAD = "SIGNAL";

// Webserver
AsyncWebServer server(80);
String webpage = "";
char _hostname[12];

// ********************  FUNCTIONS  ********************

String Hex4s(uint32_t value)
{
  char buffer[9];
  snprintf(buffer, sizeof(buffer), "%08X", value);
  return String(buffer);
}

String Hex3s(uint32_t value)
{
  char buffer[7];
  snprintf(buffer, sizeof(buffer), "%06X", value & 0x00FFFFFF);
  return String(buffer);
}

char *Hostname(char hostname[])
{
  sprintf(hostname, "esp-%06x", ESP.getChipId());
  return hostname;
}

String macId()
{
  uint8_t mac[6];
  char macStr[13] = {0};
  wifi_get_macaddr(STATION_IF, mac);
  sprintf(macStr, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(macStr);
}

String mac()
{
  uint8_t mac[6];
  char macStr[18] = {0};
  wifi_get_macaddr(STATION_IF, mac);
  sprintf(macStr, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(macStr);
}

String FlashMode(void)
{
  FlashMode_t ideMode = ESP.getFlashChipMode();
  return String(ideMode == FM_QIO    ? "QIO"
                : ideMode == FM_QOUT ? "QOUT"
                : ideMode == FM_DIO  ? "DIO"
                : ideMode == FM_DOUT ? "DOUT"
                                     : "UNKNOWN");
}

void static setLed(bool newLed)
{
  _ledOn = newLed;
  digitalWrite(ledPin, _ledOn ? LOW : HIGH);
}

// ********************  MQTT  ********************

void callback(char *topic, byte *payload, unsigned int length)
{
  // DEBUG: Show payload
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// ********************  NEW ESP BOARD  ********************

// Replace %xx% strings in html templates for specific value
String processor(const String &var)
{
  if (var == "ARDUINO_BOARD")
  {
    return String(ARDUINO_BOARD);
  }
  else if (var == "ARDUINO")
  {
    return String(ARDUINO);
  }
  else if (var == "ESP_")
  {
    return String("ESP8266");
  }
  else if (var == "ESP8266")
  {
    return String(ESP8266);
  }
  else if (var == "CHIPID")
  {
    return Hex3s(ESP.getChipId());
  }
  else if (var == "ARDUINO_ARCH_ESP8266")
  {
    return String(ARDUINO_ARCH_ESP8266);
  }
  else if (var == "ARDUINO_ARCH_")
  {
    return String("ARDUINO_ARCH_ESP8266");
  }
#if defined(ESP8266_WEMOS_D1MINI)
  else if (var == "ESP8266_WEMOS_D1MINI")
  {
    return String(ESP8266_WEMOS_D1MINI);
  }
#endif
  else if (var == "CPUFREQ")
  {
    return String(ESP.getCpuFreqMHz());
  }
  else if (var == "F_CPU")
  {
    return String(F_CPU);
  }
  else if (var == "LED_BUILTIN")
  {
#if defined(LED_BUILTIN)
    return String(LED_BUILTIN);
#else
    return String("N/A");
#endif
  }
  else if (var == "LED")
  {
    return String(ledPin);
  }
  else if (var == "FLASHID")
  {
    return Hex4s(ESP.getFlashChipId());
  }
  else if (var == "FLASHSIZE")
  {
    return String(ESP.getFlashChipSize());
  }
  else if (var == "FLASHREALSIZE")
  {
    return String(ESP.getFlashChipRealSize());
  }
  else if (var == "FLASHSPEED")
  {
    return String(ESP.getFlashChipSpeed());
  }
  else if (var == "FLASHMODE")
  {
    return FlashMode();
  }

  else if (var == "SDK")
  {
    return String(ESP.getSdkVersion());
  }
  else if (var == "CORE")
  {
    return ESP.getCoreVersion();
  }
  else if (var == "BOOTVER")
  {
    return String(system_get_boot_version());
  }
  else if (var == "SKETCHSPACE")
  {
    return String(ESP.getFreeSketchSpace());
  }
  else if (var == "SKETCHSIZE")
  {
    return String(ESP.getSketchSize());
  }
  else if (var == "SKETCHMD5")
  {
    return ESP.getSketchMD5();
  }

  else if (var == "RESETREASON")
  {
    return ESP.getResetReason();
  }
  else if (var == "FREEHEAP")
  {
    return String(system_get_free_heap_size());
  }

  // else if(var == "OTASIZE")       { return String(system_partition_get_ota_partition_size()); } v3.0

  else if (var == "MAC")
  {
    return WiFi.macAddress();
  }
  else if (var == "IP")
  {
    return WiFi.localIP().toString();
  }
  else if (var == "HOSTNAME")
  {
    return String(_hostname);
  }
  else if (var == "AUTOCONNECT")
  {
    return String(WiFi.getAutoConnect() ? "True" : "False");
  }
  else if (var == "AUTORECONNECT")
  {
    return String(WiFi.getAutoReconnect() ? "True" : "False");
  }
  else if (var == "APID")
  {
    return String(wifi_station_get_current_ap_id());
  }
  else if (var == "BSSID")
  {
    return WiFi.BSSIDstr();
  }
  else if (var == "PHY")
  {
    const char *phymodes[] = {"", "B", "G", "N"};
    return String(phymodes[(int)wifi_get_phy_mode()]);
  }
  else if (var == "CHANNEL")
  {
    return String(wifi_get_channel());
  }
  else if (var == "RSSI")
  {
    return String(WiFi.RSSI());
  }

  else if (var == "COMPDATE")
  {
    return String(VERSION);
  }
  return String();
}

// Webserver root page

void root(AsyncWebServerRequest *request)
{
  setLed(true);
  Serial.println("server /");
  AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", espboardinfo, processor);
  response->addHeader("Server", "ESP Async Web Server");
  request->send(response);
  setLed(false);
}

// Webserver not Found page
void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

#if defined(INCLUDE_OTA)
// ********************  OTA  ********************

void OTA_onStart(void)
{
  String type;
  if (ArduinoOTA.getCommand() == U_FLASH)
  {
    type = "sketch";
  }
  else
  { // U_SPIFFS
    type = "filesystem";
  }
  // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
  Serial.print("OTA Start: ");
  Serial.println(type);
}

void OTA_onEnd(void)
{
  Serial.println("OTA End\n");
}

void OTA_onProgress(unsigned int progress, unsigned int total)
{
  Serial.printf("OTA Progress: %u%%\n", (progress / (total / 100)));
}

void OTA_onError(ota_error_t error)
{
  Serial.printf("Error[%u]: ", error);
  if (error == OTA_AUTH_ERROR)
  {
    Serial.println("Auth Failed");
  }
  else if (error == OTA_BEGIN_ERROR)
  {
    Serial.println("Begin Failed");
  }
  else if (error == OTA_CONNECT_ERROR)
  {
    Serial.println("Connect Failed");
  }
  else if (error == OTA_RECEIVE_ERROR)
  {
    Serial.println("Receive Failed");
  }
  else if (error == OTA_END_ERROR)
  {
    Serial.println("End Failed");
  }
}

#endif

String MQTT_payload()
{
  String payload = "{";
  payload += "\"mac\":\"" + mac() + "\",";
  payload += "\"name\":\"" + String("ESP_") + Hex3s(ESP.getChipId()) + "\",";
  payload += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
  payload += "\"f_cpu\":" + String(F_CPU) + String(",");
  payload += "\"flash\":{";
  payload += "\"chipid\":\"" + Hex4s(ESP.getFlashChipId()) + "\",";
  payload += "\"realsize\":" + String(ESP.getFlashChipRealSize()) + ",";
  payload += "\"size\":" + String(ESP.getFlashChipSize()) + ",";
  payload += "\"speed\":" + String(ESP.getFlashChipSpeed()) + ",";
  payload += "\"mode\":" + String(ESP.getFlashChipMode()) + "}";
  payload += "}";
  return payload;
}
// ********************  SETUP  ********************

void setup()
{
  if (!Serial)
    Serial.begin(115200);
  Serial.println();
  Serial.println(APPNAME);
  Serial.println(VERSION);
  Serial.println();

  // LED
  pinMode(ledPin, OUTPUT);   // Onboard LED
  digitalWrite(ledPin, LOW); // Switch on LED

  myWM.connectToWiFi();

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
}

// ********************  LOOP  ********************

void loop()
{
  myWM.loop();

  // SEND MESSAGE
  if (myWM.StatusMQTT == myWM.ConnectionActive)
  {
    if (sentMessage)
    {
#if defined(INCLUDE_OTA)
      ArduinoOTA.handle();
#endif
    }
    else
    {
      String devicetopic = "devices/" + macId();
      myWM.mqttClient.publish(MQTT_TOPIC, MQTT_payload().c_str());
      myWM.mqttClient.publish(devicetopic.c_str(), MQTT_payload().c_str(), true);
      Serial.println("Message sent. Be proud! (End of messages)");

      sentMessage = true;
      setLed(false);

      server.begin();
#if defined(INCLUDE_OTA)
      ArduinoOTA.begin();
#endif
    }
  }
  else // NOT (myWM.StatusMQTT == myWM.ConnectionActive)
  {
    if ((unsigned long)(millis() - previousMillis) >= intervalMillis)
    {
      setLed(!_ledOn);
      previousMillis = millis();
    }
  }
}
