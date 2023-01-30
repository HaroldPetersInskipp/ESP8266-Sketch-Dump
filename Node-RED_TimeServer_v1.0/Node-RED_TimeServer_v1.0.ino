#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

// Replace with your network credentials
const char* ssid = "wifi_ssid";
const char* password = "your_password";

// Replace with the IP address of your Node-RED API server
const char* server = "10.0.0.111";
const int port = 1880;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if (client.connect(server, port)) {
    // Make a GET request to the API endpoint for current time
    client.println("GET /time HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("Connection: close");
    client.println();
  }
  while (client.connected()) {
    while (client.available()) {
      String line = client.readStringUntil('\r');
      if (line.startsWith("{\"time\":\"")) {
        // Extract the current time from the response
        int startIndex = line.indexOf("\":\"") + 3;
        int endIndex = line.indexOf("\"}");
        String currentTime = line.substring(startIndex, endIndex);
        Serial.println("Current time: " + currentTime);
      }
    }
  }
  client.stop();
  delay(10000); // interval of 10s between each update
}