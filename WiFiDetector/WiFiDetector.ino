#include <ESP8266WiFi.h>

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Connect to the WiFi network
  WiFi.begin("wifi_ssid", "your_password");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi");
}

void loop() {
  // Scan for nearby WiFi networks
  int numNetworks = WiFi.scanNetworks();
  
  // Print the number of networks found
  Serial.println("Number of networks found: " + String(numNetworks));
  
  // Print the details of each network found
  for (int i = 0; i < numNetworks; i++) {
    // Filter out networks that are not 2.4 GHz
    if (WiFi.channel(i) > 13) {
      continue;
    }
    
    // Print the network details
    Serial.print("Network " + String(i + 1) + ": ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" (" + String(WiFi.RSSI(i)) + " dBm)");
    Serial.print(" on channel " + String(WiFi.channel(i)));
    Serial.println();
  }
  
  // Delay for a small amount of time
  delay(5000);
}
