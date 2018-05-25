#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "secrets.h"

#ifndef SECRETS_H
 #define SECRETS_H
 const char* WIFI_SSID     = "ssid";
 const char* WIFI_PASSWORD = "password";
#endif

void setup(){

   Serial.begin(115200);
   while (!Serial) { delay(1); }

   Serial.println("Congratulations with your new ESP Board!!");

   Serial.print("MAC: ");
   Serial.println(WiFi.macAddress());

   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.println("Connecting to WiFi..");
   }

   Serial.print("WiFi IP: ");
   Serial.println(WiFi.localIP());

}

void loop()
{

}
