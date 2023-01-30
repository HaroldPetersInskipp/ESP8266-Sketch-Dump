#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>

// Raspberry Pi Mosquitto MQTT Broker examples
// #define MQTT_HOST IPAddress(10, 0, 0, 111)
// #define MQTT_HOST "10.0.0.111"
// #define MQTT_PORT 1883
// #define MQTT_PUB_TOPIC "ESP8266/time"

// WiFi and MQTT server settings
const char* WIFI_SSID = "wifi_ssid";
const char* WIFI_PASSWORD = "your_password";
const char* MQTT_SERVER = "10.0.0.111";
const int MQTT_PORT = 1883;
const char* MQTT_USERNAME = "MQTT_USERNAME";
const char* MQTT_PASSWORD = "your_password";
const char* MQTT_PUB_TOPIC = "ESP8266/time";
unsigned long previousMillis = 0;
const long interval = 60000;

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

void connectToWifi() {
  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println();
  Serial.println("Connected to WiFi!");
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}


void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi.");
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach();
  wifiReconnectTimer.once(2, connectToWifi);
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("MQTT server: ");
  Serial.println(MQTT_SERVER);
  Serial.println();
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");

  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void onMqttPublish(uint16_t packetId) {
  Serial.println("Publish acknowledged.");
  Serial.print("PacketId: ");
  Serial.println(packetId);
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println();
    
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect); 
  
  // Set up the MQTT client
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  //mqttClient.onSubscribe(onMqttSubscribe);
  //mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCredentials(MQTT_USERNAME, MQTT_PASSWORD);
  connectToWifi();
}

void loop() {
  unsigned long currentMillis = millis();
  // Every X number of seconds (interval = 1 minute) 
  // it publishes a new MQTT message
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
    previousMillis = currentMillis;

    // TODO: place real timestamp code here
    String timestamp = "1672734836259";
    int placeholder = timestamp.toInt();

    // Publish an MQTT message on topic
    uint16_t packetIdPub1 = mqttClient.publish("ESP8266/time", 2, false, String(placeholder).c_str());
    Serial.printf("Publishing on topic '%s' at QoS 2, packetId: %i\n", MQTT_PUB_TOPIC, packetIdPub1);;
    Serial.print("payload: ");
    Serial.println(String(placeholder));
    Serial.println();
  }
}