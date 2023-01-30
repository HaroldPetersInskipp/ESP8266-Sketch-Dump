/*
This sketch file does the following:
1. Includes the necessary libraries for WiFi and MQTT communication (`WiFi.h` and `PubSubClient.h`) as well as the `time.h` library for getting the current time.
2. Defines constants for the WiFi and MQTT server settings, including the SSID and password for the WiFi network, the MQTT server address, port number, and username and password for the MQTT server.
3. Creates WiFi and MQTT client objects.
4. In the `setup` function, initializes serial communication, connects to the WiFi network, and sets up the MQTT client.
5. In the `loop` function, connects to the MQTT server if not already connected, gets the current time, creates a JSON object with the current time, and publishes the time as a MQTT message with the topic "ESP8266/time". The loop then waits for one minute before sending the next message.
6. Defines a callback function for handling incoming MQTT messages, which simply prints the received message to the serial console.
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>

// WiFi and MQTT server settings
const char* WIFI_SSID = "wifi_ssid";
const char* WIFI_PASSWORD = "your_password";
const char* MQTT_SERVER = "10.0.0.111";
const int MQTT_PORT = 1883;
const char* MQTT_USERNAME = "MQTT_USERNAME";
const char* MQTT_PASSWORD = "your_password";

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

  // Get the current time
  time_t now = time(nullptr);
  Serial.println(now);
  struct tm* tm = localtime(&now);

  // Create a JSON object with the current time
  timeJson = "{\"hour\":" + String(tm->tm_hour) + ",\"minute\":" + String(tm->tm_min) + ",\"second\":" + String(tm->tm_sec) + "}";

  // Publish the time as a MQTT message
  mqttClient.publish("ESP8266/time", timeJson.c_str());

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