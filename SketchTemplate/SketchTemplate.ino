/*
EXPECTED OUTPUT:
22:50:16.228 -> Starting boot process...
22:50:16.228 -> Firmware version: 2.2.2-dev(38a443e)
22:50:16.228 -> Boot version: 6
22:50:16.228 -> Boot mode: 1
22:50:16.228 -> CPU frequency: 80
22:50:16.228 -> Flash size: 4194304
22:50:16.228 -> Free heap: 51504
22:50:16.228 -> Heap fragmentation: 0
22:50:16.228 -> Chip ID: 644400
22:50:16.228 -> Flash chip ID: 1458270
22:50:16.228 -> Initializing connection to the WiFi network
22:50:17.226 -> Connecting..
22:50:18.199 -> Connecting..
22:50:19.932 -> Connecting..
22:50:20.918 -> Connecting..
22:50:21.915 -> Connecting..
22:50:21.915 -> Connected to the WiFi network successfully
22:50:21.915 -> WiFi mode: 1
22:50:21.915 -> WiFi channel: 7
22:50:21.915 -> WiFi SSID: wifi_ssid
22:50:21.915 -> WiFi MAC address: C8:C9:A3:09:D5:30
22:50:21.915 -> WiFi local IP address: 10.0.0.228
22:50:21.915 -> WiFi subnet mask: 255.255.255.0
22:50:21.961 -> WiFi gateway IP address: 10.0.0.1
22:50:21.961 -> WiFi DNS server IP address: 75.75.75.75
22:50:21.961 -> WiFi status: 3
22:50:21.961 -> WiFi RSSI: -35
22:50:21.961 -> Current time: Thu Jan  1 00:00:05 1970
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <time.h>

const char* ssid = "wifi_ssid";
const char* password = "your_password";

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Print boot message
  Serial.println("\nStarting boot process...");
  
  // Print firmware version
  Serial.print("Firmware version: ");
  Serial.println(ESP.getSdkVersion());
  
  // Print details about the device's firmware using the ESP8266WiFi library
  Serial.print("Boot version: ");
  Serial.println(ESP.getBootVersion());
  Serial.print("Boot mode: ");
  Serial.println(ESP.getBootMode());
  Serial.print("CPU frequency: ");
  Serial.println(ESP.getCpuFreqMHz());
  Serial.print("Flash size: ");
  Serial.println(ESP.getFlashChipSize());

  // Print memory usage
  Serial.print("Free heap: ");
  Serial.println(ESP.getFreeHeap());
  Serial.print("Heap fragmentation: ");
  Serial.println(ESP.getHeapFragmentation());

  // Print chip information
  Serial.print("Chip ID: ");
  Serial.println(ESP.getChipId());
  Serial.print("Flash chip ID: ");
  Serial.println(ESP.getFlashChipId());

  WiFi.begin(ssid, password);
  Serial.println("Initializing connection to the WiFi network");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }
  Serial.println("Connected to the WiFi network successfully");

  // Print details about the device's WiFi capabilities
  Serial.print("WiFi mode: ");
  Serial.println(WiFi.getMode());
  Serial.print("WiFi channel: ");
  Serial.println(WiFi.channel());
  Serial.print("WiFi SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("WiFi MAC address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("WiFi local IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("WiFi subnet mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("WiFi gateway IP address: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("WiFi DNS server IP address: ");
  Serial.println(WiFi.dnsIP());

  // Print WiFi connection status
  Serial.print("WiFi status: ");
  Serial.println(WiFi.status());

  // Print WiFi signal strength
  Serial.print("WiFi RSSI: ");
  Serial.println(WiFi.RSSI());

  // TODO: Update Print current time from web call
  Serial.print("Current time: ");
  time_t now = time(nullptr);
  Serial.println(ctime(&now));
}

void loop() {
  // Do nothing
}
