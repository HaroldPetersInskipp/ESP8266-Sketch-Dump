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

const char* ssid = "wifi_ssid";
const char* password = "your_password";
const char* mqttServer = "10.0.0.111";
const int mqttPort = 1883;
const char* mqttUser = "MQTT_USERNAME";
const char* mqttPassword = "your_password";
const char* esp8266unitNumber = "ESP8266Client_3";

// Set the MQTT topic for the controller buttons
const char* buttonTopic = "ESP8266/Controller";

// Set the GPIO pins for the 3 buttons
const int buttonPin1 = D2; // button 1 input pin
const int buttonPin2 = D3; // button 2 input pin
const int buttonPin3 = D4; // button 3 input pin


// Declare variables for the built in LED's
const int ESP8266_BUILTIN_LED = 2;  // Onboard LED (GPIO2)
const int NodeMCU_BUILTIN_LED = 16; // External LED (GPIO16)
// Both BUILTIN_LED's operate in “inverted” mode, with regard to the pin levels
// When the pin is 1/HIGH, the LED is off
// When the pin is 0/LOW, the LED is on

WiFiClient espClient;
PubSubClient client(espClient);
ESP8266WebServer server(80);

const int bufferSize = 10; // Maximum number of messages to store in the buffer
String buffer[bufferSize]; // Buffer to store MQTT messages
int nextIndex = 0; // Index to add the next message to the buffer

// NEW CODE BELOW

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
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
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
// NEW CODE ABOVE


void setup() {
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


// Set the button pins as inputs with pull-up resistors
Serial.println("Setting the following pins as inputs with pull-up resistors for controller buttons");
pinMode(buttonPin1, INPUT_PULLUP);
Serial.println("Pin: 'D2' as button 1");
pinMode(buttonPin2, INPUT_PULLUP);
Serial.println("Pin: 'D3' as button 2");
pinMode(buttonPin3, INPUT_PULLUP);
Serial.println("Pin: 'D4' as button 3");

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
Serial.println("Connecting to WiFi...");
connectToWifi();
Serial.println("Connected to WiFi!");


client.setServer(mqttServer, mqttPort);
client.setCallback(callback);
while (!client.connected()) {
if (client.connect(esp8266unitNumber, mqttUser, mqttPassword)) {
client.subscribe("ESP8266/#");
Serial.println("Connected to MQTT server");
} else {
Serial.println("Could not connect to MQTT server, trying again in 5 seconds");
delay(5000);
}
}
// Print STATION mode specific details about the device's WiFi capabilities
Serial.print("WiFi SSID: ");
Serial.println(WiFi.SSID());
Serial.print("WiFi local IP address: ");
Serial.println(WiFi.localIP());
Serial.print("WiFi subnet mask: ");
Serial.println(WiFi.subnetMask());
Serial.print("WiFi gateway IP address: ");
Serial.println(WiFi.gatewayIP());
Serial.print("WiFi DNS server IP address: ");
Serial.println(WiFi.dnsIP());

// Print the remaining details about the device's WiFi capabilities
Serial.print("WiFi mode: ");
Serial.println(WiFi.getMode());
Serial.print("WiFi channel: ");
Serial.println(WiFi.channel());
Serial.print("WiFi MAC address: ");
Serial.println(WiFi.macAddress());

// Print WiFi connection status
Serial.print("WiFi status: ");
Serial.println(WiFi.status());

// Print WiFi signal strength
Serial.print("WiFi RSSI: ");
Serial.println(WiFi.RSSI());

server.on("/", handleRoot);
server.begin();
Serial.println("Web server started");
}

void loop() {

  client.loop();
  server.handleClient();

  // NEW CODE BELOW

  // If the button is pressed, send an MQTT message
  if (digitalRead(buttonPin1) == LOW) {
    digitalWrite(buttonPin1, HIGH);
    String placeholder = "Button 1 has been pressed";
    // Publish an MQTT message on topic
    uint16_t packetIdPub1 = mqttClient.publish(buttonTopic, 2, false, String(placeholder).c_str());
    Serial.println();
    Serial.printf("Publishing a MQTT message on topic '%s' at QoS 2, packetId: %i\n", MQTT_PUB_TOPIC, packetIdPub1);;
    Serial.print("payload: ");
    Serial.println(String(placeholder));
    Serial.println();
    delay(100);
  }
  
  // If the button is pressed, send an MQTT message
  if (digitalRead(buttonPin2) == LOW) {
    digitalWrite(buttonPin2, HIGH);
    String placeholder = "Button 2 has been pressed";
    // Publish an MQTT message on topic
    uint16_t packetIdPub1 = mqttClient.publish(buttonTopic, 2, false, String(placeholder).c_str());
    Serial.println();
    Serial.printf("Publishing a MQTT message on topic '%s' at QoS 2, packetId: %i\n", MQTT_PUB_TOPIC, packetIdPub1);;
    Serial.print("payload: ");
    Serial.println(String(placeholder));
    Serial.println();
    delay(100);
  }

  // If the button is pressed, send an MQTT message
  if (digitalRead(buttonPin3) == LOW) {
    digitalWrite(buttonPin3, HIGH);
    String placeholder = "Button 3 has been pressed";
    // Publish an MQTT message on topic
    uint16_t packetIdPub1 = mqttClient.publish(buttonTopic, 2, false, String(placeholder).c_str());
    Serial.println();
    Serial.printf("Publishing a MQTT message on topic '%s' at QoS 2, packetId: %i\n", MQTT_PUB_TOPIC, packetIdPub1);;
    Serial.print("payload: ");
    Serial.println(String(placeholder));
    Serial.println();
    delay(100);
  }

  // NEW CODE ABOVE

}

void callback(char* topic, byte* payload, unsigned int length) {
String mqttPayload;
String mqttTopic = topic;
for (int i = 0; i < length; i++) {
mqttPayload += (char)payload[i];
}
buffer[nextIndex] = mqttPayload;
nextIndex = (nextIndex + 1) % bufferSize; // Increment index and wrap around if necessary
Serial.println();
Serial.println("Received MQTT message");
Serial.println("topic: " + mqttTopic);
Serial.println("payload: " + mqttPayload);
}

void handleRoot() {
String response;
response += "ESP8266 MQTT payload display webserver\n";
for (int i = 0; i < bufferSize; i++) {
response += buffer[(nextIndex + i) % bufferSize] + "\n"; // Add messages in the correct order
}
server.send(200, "text/plain", response);
}

