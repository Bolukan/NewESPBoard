![Travis CI status](https://api.travis-ci.com/Bolukan/NewESPBoard.svg?branch=master)
![License](https://img.shields.io/github/license/Bolukan/NewESPBoard)
![Release stable](https://badgen.net/github/release/Bolukan/NewESPBoard/stable)

# New ESP board

Upload this script to a new ESP8266 to gather the information on the hardware and WiFi connection.
The script starts a webserver. Open the webpage with your browser.
Also a MQTT message is sent to devices/newboard and devices/<mac> 

The script also runs the OTA (Over-The-Air) service, which enables you to replace the code with your own code over-the-air.

To use: Add your SSID and password in a secrets.h file (example available) or change the main.cpp
