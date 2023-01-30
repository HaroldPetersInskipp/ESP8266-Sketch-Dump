#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <time.h>

// WiFi and MQTT server settings
const char* WIFI_SSID = "wifi_ssid";
const char* WIFI_PASSWORD = "your_password";
const char* MQTT_SERVER = "10.0.0.111";
const int MQTT_PORT = 1883;
const char* MQTT_USERNAME = "MQTT_USERNAME";
const char* MQTT_PASSWORD = "your_password";

// Time server settings
const char* TIME_SERVER = "http://worldtimeapi.org/api/timezone/America/Denver.txt";

// WiFi and MQTT client objects
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// JSON object to store the current time
String timeJson;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi!");

  // Set up the MQTT client
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
}

void loop() {
  // Connect to the MQTT server
  if (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT server...");
    if (mqttClient.connect("ESP6288", MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("Connected to MQTT server!");
    } else {
      Serial.print("Failed to connect to MQTT server, rc=");
      Serial.println(mqttClient.state());
      delay(1000);
      return;
    }
  }
  mqttClient.loop();

  // Make a HTTP request to the time server to get the current timestamp
  HTTPClient http;
  http.begin(wifiClient, TIME_SERVER);
  int httpCode = http.GET();
  if (httpCode != HTTP_CODE_OK) {
    Serial.println("Error making HTTP request to time server");
    http.end();
    return;
  }
  String response = http.getString();

  // Parse the timestamp from the HTTP response
  int timestamp = response.substring(response.indexOf("unixtime") + 10, response.indexOf("utc_offset") - 3).toInt();
  Serial.print(timestamp);

  
  // Close the HTTP connection and wait for one minute before sending the next message
  http.end();

  // Publish the time as a MQTT message
  mqttClient.publish("ESP8266/time", "placeholder");
  
  // Wait for one minute before sending the next message
  delay(60000);
  }

  // Callback function for MQTT messages
  void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Print the received MQTT message to the serial console
  Serial.print("Received MQTT message: ");
  Serial.print(topic);
  Serial.print(" - ");
  for (int i = 0; i < length; i++) {
  Serial.print((char)payload[i]);
  }
  Serial.println();
}