#include <Arduino.h>

const char espboardinfo[] PROGMEM = R"==EOF==(<!DOCTYPE html>
<html lang="en"><head>
<meta http-equiv="content-type" content="text/html; charset=UTF-8">
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
<!-- Bootstrap CSS -->
<link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css" integrity="sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO" crossorigin="anonymous">
<title>New ESP Board</title>
</head>
<body>
<div class="container-fluid bg-white">
<div class="container">
<div class="row mb-5">
<table class="table table-sm table-striped bg-light">
<caption class="text-dark" style="caption-side: top;"><h3>New ESP Board @ %IP%</h3></caption>
<thead><tr class="bg-secondary text-white">
<th scope="col">item</th>
<th scope="col">value</th>
</tr></thead>
<tbody>
<tr><td><b>ESP chip</b></td><td></td></tr>
<tr><td>ESP Chip Id</td><td title="ESP.getChipId() / ESP.getEfuseMac()">%CHIPID%</td></tr>
<tr><td>Cpu Freq</td><td title="ESP.getCpuFreqMHz()">%CPUFREQ% MHz</td></tr>
<tr><td>F_CPU</td><td title="F_CPU">%F_CPU%</td></tr>

<tr><td><b>Flash chip</b></td><td></td></tr>
<tr><td>Chip ID</td><td>%FLASHID%</td></tr>
<tr><td>Chip Size</td><td>%FLASHSIZE% bytes</td></tr>
<tr><td>Chip Real Size</td><td>%FLASHREALSIZE% bytes</td></tr>
<tr><td>Chip Speed</td><td>%FLASHSPEED% Hz</td></tr>
<tr><td>Chip Mode</td><td>%FLASHMODE%</td></tr>

<tr><td><b>Compiler</b></td><td></td></tr>
<tr><td>ARDUINO_BOARD</td><td title="ARDUINO_BOARD">%ARDUINO_BOARD%</td></tr>
<tr><td>ARDUINO</td><td title="ARDUINO">%ARDUINO%</td></tr>
<tr><td>%ESP_%</td><td title="%ESP_%">%ESP8266%%ESP32%</td></tr>
<tr><td>%ARDUINO_ARCH_%</td><td title="%ARDUINO_ARCH_%">%ARDUINO_ARCH_ESP8266%%ARDUINO_ARCH_ESP32%</td></tr>
<tr><td>ESP8266_WEMOS_D1MINI</td><td title="ESP8266_WEMOS_D1MINI">%ESP8266_WEMOS_D1MINI%</td></tr>
<tr><td>LED_BUILTIN</td><td title="LED_BUILTIN">%LED_BUILTIN%</td></tr>
<tr><td><i>LED</i></td><td title="const int ledPin">%LED%</td></tr>

<tr><td><b>Software</b></td><td></td></tr>
<tr><td>SDK version</td><td title="ESP.getSdkVersion()">%SDK%</td></tr>
<tr><td>Core version</td><td title="ESP.getCoreVersion()">%CORE%</td></tr>
<tr><td>ARDUINO</td><td>%ARDUINO%</td></tr>
<tr><td>Boot version</td><td>%BOOTVER%</td></tr>
<tr><td>Free sketch space</td><td>%SKETCHSPACE% bytes</td></tr>
<tr><td>Sketch size</td><td>%SKETCHSIZE% bytes</td></tr>
<tr><td>Sketch MD5</td><td>%SKETCHMD5%</td></tr>

<tr><td><b>Running code</b></td><td></td></tr>
<tr><td>Reset reason</td><td>%RESETREASON%</td></tr>
<tr><td>Free heap</td><td>%FREEHEAP% bytes</td></tr>

<tr><td><b>WiFi</b></td><td></td></tr>
<tr><td>MAC</td><td>%MAC%</td></tr>
<tr><td>IP</td><td>%IP%</td></tr>
<tr><td>Hostname</td><td>%HOSTNAME%</td></tr>
<tr><td>AutoConnect</td><td>%AUTOCONNECT%</td></tr>
<tr><td>AutoReconnect</td><td>%AUTORECONNECT%</td></tr>
<tr><td>AP ID</td><td>%APID%</td></tr>
<tr><td>BSSID</td><td>%BSSID%</td></tr>
<tr><td>PHY</td><td>%PHY%</td></tr>
<tr><td>Channel</td><td>%CHANNEL%</td></tr>
<tr><td>RSSI</td><td>%RSSI%</td></tr>
<tr><td>CC</td><td></td></tr>

</tbody>
</table>
<p><b>&copy; 2019 Bolukan</b> - Compiled: %COMPDATE%</p>
</div>
</div>
</div>
</body>
</html>
)==EOF==";
