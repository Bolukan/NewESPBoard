#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "secrets.h"
#include <locale.h>

#ifndef SECRETS_H
 #define SECRETS_H
 const char* WIFI_SSID     = "ssid";
 const char* WIFI_PASSWORD = "password";
#endif

void setup(){

   Serial.begin(115200);
   while (!Serial) { delay(1); }

   setlocale(LC_ALL, "nl_NL.UTF8");

   Serial.println();
   Serial.println("Congratulations with your new ESP Board!!");

   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.println("Connecting to WiFi..");
   }

   Serial.println("\nWIFI");

   Serial.print("MAC: ");
   Serial.println(WiFi.macAddress());

   Serial.print("WiFi IP: ");
   Serial.println(WiFi.localIP());

   Serial.print("ESP Chip Id: ");
   Serial.printf("0x%x\n", ESP.getChipId());

   Serial.print("SDK version: ");
   Serial.println(ESP.getSdkVersion());

   Serial.println("\nFLASH");
   Serial.printf("Chip ID: 0x%x\n", ESP.getFlashChipId());
   Serial.printf("Chip Real Size: %u bytes\n", ESP.getFlashChipRealSize());
   Serial.printf("Chip Size: %u bytes\n", ESP.getFlashChipSize());
   Serial.printf("Chip Speed: %u Hz\n", ESP.getFlashChipSpeed());

   Serial.print("Chip Mode (0=QIO;1=QOUT;2=DIO;3=DOUT;255=unknown): ");
   Serial.println(ESP.getFlashChipMode());

}

void loop()
{
    delay(1);
}
