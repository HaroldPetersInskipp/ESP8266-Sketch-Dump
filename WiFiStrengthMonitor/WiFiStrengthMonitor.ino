#include <ESP8266WiFi.h>

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Connect to the WiFi network (replace "your_ssid" and "your_password" with your own WiFi network details)
  WiFi.begin("wifi_ssid", "your_password");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  
  // Print the IP address of the ESP8266 NodeMCU ESP-12E
  Serial.println("Connected to WiFi. IP address: " + WiFi.localIP().toString());
}

void loop() {
  // Scan for nearby WiFi networks
  int numNetworks = WiFi.scanNetworks();
  
  // Print the number of networks found
  //Serial.println("Number of networks found: " + String(numNetworks));
  
  // Print the details of each network
  for (int i = 0; i < numNetworks; i++) {
    String ssid = WiFi.SSID(i);
    int rssi = WiFi.RSSI(i);
    Serial.println(rssi); // Print the RSSI value as an int
  }
  
  // Delay for a small amount of time
  delay(2000);
}
