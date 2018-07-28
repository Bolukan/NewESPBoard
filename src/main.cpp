#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif

#include "secrets.h"
#ifndef SECRETS_H
 #define SECRETS_H
 char WIFI_SSID[]     = "ssid";
 char WIFI_PASSWORD[] = "password";
#endif

void setup(){

   Serial.begin(115200);

   Serial.println();
   Serial.println("Congratulations with your new ESP Board!!");

   Serial.print("\nCHIP\n");
   Serial.printf("ESP Chip Id:    0x%06x\n", ESP.getChipId());
   Serial.printf("Cpu Freq:       %u MHz\n", ESP.getCpuFreqMHz());

   Serial.print("\nFLASH\n");
   Serial.printf("Chip ID :       0x%06x\n", ESP.getFlashChipId());
   Serial.printf("Chip Size:      %u bytes\n", ESP.getFlashChipSize());
   Serial.printf("Chip Real Size: %u bytes\n", ESP.getFlashChipRealSize());
   Serial.printf("Chip Speed:     %u Hz\n", ESP.getFlashChipSpeed());
   FlashMode_t ideMode = ESP.getFlashChipMode();
   Serial.printf("Chip Mode:      0x%02x %s\n", ideMode, (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));

   Serial.print("\nSOFTWARE\n");
   Serial.printf("SDK version:    %s\n", ESP.getSdkVersion());
   Serial.printf("Core version:   %s\n", ESP.getCoreVersion().c_str());

   Serial.print("\nWIFI\n");
   Serial.print("Connecting to WiFi .");
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   while (WiFi.status() != WL_CONNECTED) {
     delay(250);
     Serial.print(".");
   }
   Serial.print("\n");
   Serial.printf("MAC:            %s\n", WiFi.macAddress().c_str());
   Serial.printf("WiFi IP:        %s\n", WiFi.localIP().toString().c_str());

}

void loop()
{
    delay(1);
}
