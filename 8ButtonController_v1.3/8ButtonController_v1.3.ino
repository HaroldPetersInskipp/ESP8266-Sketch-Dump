// 8 Button wireless controller for ESP8266 NodeMCU using MQTT to communicate with a Node-RED server by Harold Peters Inskipp
// For the Node-RED companion flow.json file visit https://github.com/HaroldPetersInskipp

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

const char* esp8266unitNumber = "ESP8266Client_1";  // Set a esp8266unitNumber to identify each ESP8266 NodeMCU (example "ESP8266Client_1")
const char* WIFI_SSID = "wifi_ssid";                   // Set the WIFI ssid to connect to (example "WiFi SSID")
const char* WIFI_PASSWORD = "your_password";            // Set the WIFI password (example "Wifi Password")
const char* MQTT_SERVER = "10.0.0.111";             // Set the MQTT server ip address (example "10.0.0.111")
const int MQTT_PORT = 1883;                         // Set the MQTT port number (default port is 1883)
const char* MQTT_USERNAME = "MQTT_USERNAME";           // Set the MQTT username to use (example "MQTT Username")
const char* MQTT_PASSWORD = "MQTT_PASSWORD";            // Set the MQTT password to use (example "MQTT Password")
const char* MQTT_PUB_TOPIC = "ESP8266/Controller_1";// Set the MQTT publishing topic to use (example "ESP8266/Controller_1")
const char* MQTT_SUB_TOPIC = "ESP8266/#";           // Set the MQTT topic to subscribe to (example "ESP8266/#")

// Declare variables for the GPIO pins to use as button inputs
// const int buttonPin0 = D0;       // Button 0 input pin (GPIO16) // in use as NodeMCU_BUILTIN_LED
const int analogInputPin0 = A0;     // Button 0 input pin (ESP8266 analog pin ADC0 = A0)
const int buttonPin1 = D1;          // Button 1 input pin (GPIO5)
const int buttonPin2 = D2;          // Button 2 input pin (GPIO4)
const int buttonPin3 = D3;          // Button 3 input pin (GPIO0)
const int buttonPin4 = D4;          // Button 4 input pin (GPIO2)
const int buttonPin5 = D5;          // Button 5 input pin (GPIO14)
const int buttonPin6 = D6;          // Button 6 input pin (GPIO12)
const int buttonPin7 = D7;          // Button 7 input pin (GPIO13)
const int debounceInterval = 200;   // Set the debounce interval for the buttons, in milliseconds (1000 = 1 second)
int currentAnalogValue = 0;         // Initalize the currentAnalogValue to 0

// Declare variables for the built in LED's
const int NodeMCU_BUILTIN_LED = D0;    // External LED (GPIO16) pin D0
// const int ESP8266_BUILTIN_LED = D4; // Onboard LED (GPIO2) pin D4 // in use as buttonPin4
// When the pin is 1/HIGH, the LED is off
// When the pin is 0/LOW, the LED is on

const int bufferSize = 20;  // Maximum number of MQTT message payloads to store in the buffer for the web server to display
String buffer[bufferSize];  // Buffer to store MQTT messages
int nextIndex = 0;          // Index to add the next MQTT message to the buffer

WiFiClient espClient;
PubSubClient client(espClient);
ESP8266WebServer server(80);
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;
void connectToWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi.");
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  wifiReconnectTimer.once(2, connectToWifi);
}

void setup() {

  Serial.begin(115200);

  delay(500);

  // Print boot message
  Serial.println("\n\nStarting the boot process...");

  // Print device identifier
  Serial.print("Device identifier: ");
  Serial.println(esp8266unitNumber);

  // Print firmware version
  Serial.print("Firmware version: ");
  Serial.println(ESP.getSdkVersion());

  // Print details about the device's firmware
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

  // Initilize the following built in LED's as outputs and set their starting state
  Serial.println("Setting the following built in LED's as outputs.");
  pinMode(NodeMCU_BUILTIN_LED, OUTPUT);
  // digitalWrite(NodeMCU_BUILTIN_LED, HIGH); // When the pin is 1/HIGH, the LED is off
  digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
  Serial.println("LED: 'NodeMCU_BUILTIN_LED' (GPIO16)");

  // Set the following button pins as inputs with pull-up resistors
  Serial.println("Setting the following pins as inputs with pull-up resistors for MQTT buttons.");
  pinMode(analogInputPin0, INPUT); // Set the input pin as an input
  Serial.println("Pin: 'A0' as button 0");
  pinMode(buttonPin1, INPUT_PULLUP);
  Serial.println("Pin: 'D1' as button 1");
  pinMode(buttonPin2, INPUT_PULLUP);
  Serial.println("Pin: 'D2' as button 2");
  pinMode(buttonPin3, INPUT_PULLUP);
  Serial.println("Pin: 'D3' as button 3");
  pinMode(buttonPin4, INPUT_PULLUP);
  Serial.println("Pin: 'D4' as button 4");
  pinMode(buttonPin5, INPUT_PULLUP);
  Serial.println("Pin: 'D5' as button 5");
  pinMode(buttonPin6, INPUT_PULLUP);
  Serial.println("Pin: 'D6' as button 6");
  pinMode(buttonPin7, INPUT_PULLUP);
  Serial.println("Pin: 'D7' as button 7");

  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  Serial.println("Connecting to WiFi...");
  connectToWifi();
  Serial.println("Connected to WiFi!");

  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  while (!client.connected()) {
    if (client.connect(esp8266unitNumber, MQTT_USERNAME, MQTT_PASSWORD)) {
      client.subscribe(MQTT_SUB_TOPIC);
      Serial.println("Connected to MQTT server successfully.");
      Serial.print("Subscribed to MQTT messages on the topic: ");
      Serial.println(MQTT_SUB_TOPIC);
      Serial.print("Publishing MQTT messages on the topic: ");
      Serial.println(MQTT_PUB_TOPIC);
    } else {
      Serial.println("Failed to connect to MQTT server, trying again in 3 seconds.");
      delay(3000);
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
  Serial.println(WiFi.getPhyMode());
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

  // Start the ESP8266 MQTT payload display web server and print the IP address
  server.on("/", handleRoot);
  server.begin();
  Serial.println("ESP8266 MQTT payload display web server started successfully.");
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  // End the boot process and print client ready message
  Serial.println("Ending the boot process...\n");
  Serial.print(esp8266unitNumber);
  Serial.println(" should now be operational.\n");

  // fancy graphics
  Serial.println(" ★BUTTON LAYOUT★");
  Serial.println(" ___ ___ ___ ___ ");
  Serial.println("| 0 | 1 | 2 | 3 |");
  Serial.println("|---|---|---|---|");
  Serial.println("| 4 | 5 | 6 | 7 |");
  Serial.println(" --- --- --- --- \n");

}

void loop() {

  client.loop();

  // If the button is pressed, send an MQTT message
  if (digitalRead(buttonPin1) == LOW) {
    digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
    const char* placeholder = "Button 1";
    // Publish an MQTT message on topic
    uint16_t packetIdPub1 = client.publish(MQTT_PUB_TOPIC, placeholder);
    Serial.println();
    Serial.printf("Publishing a MQTT message on topic '%s' at QoS 0, packetId: %i\n", MQTT_PUB_TOPIC, packetIdPub1);;
    Serial.print("payload: ");
    Serial.println(placeholder);
    Serial.println();
    delay(debounceInterval);
    digitalWrite(NodeMCU_BUILTIN_LED, HIGH); // When the pin is 1/HIGH, the LED is off
  }
  
  // If the button is pressed, send an MQTT message
  if (digitalRead(buttonPin2) == LOW) {
    digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
    const char* placeholder = "Button 2";
    // Publish an MQTT message on topic
    uint16_t packetIdPub1 = client.publish(MQTT_PUB_TOPIC, placeholder);
    Serial.println();
    Serial.printf("Publishing a MQTT message on topic '%s' at QoS 0, packetId: %i\n", MQTT_PUB_TOPIC, packetIdPub1);;
    Serial.print("payload: ");
    Serial.println(placeholder);
    Serial.println();
    delay(debounceInterval);
    digitalWrite(NodeMCU_BUILTIN_LED, HIGH); // When the pin is 1/HIGH, the LED is off
  }

  // If the button is pressed, send an MQTT message
  if (digitalRead(buttonPin3) == LOW) {
    digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
    const char* placeholder = "Button 3";
    // Publish an MQTT message on topic
    uint16_t packetIdPub1 = client.publish(MQTT_PUB_TOPIC, placeholder);
    Serial.println();
    Serial.printf("Publishing a MQTT message on topic '%s' at QoS 0, packetId: %i\n", MQTT_PUB_TOPIC, packetIdPub1);;
    Serial.print("payload: ");
    Serial.println(placeholder);
    Serial.println();
    delay(debounceInterval);
    digitalWrite(NodeMCU_BUILTIN_LED, HIGH); // When the pin is 1/HIGH, the LED is off
  }

  // If the button is pressed, send an MQTT message
  if (digitalRead(buttonPin4) == LOW) {
    digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
    const char* placeholder = "Button 4";
    // Publish an MQTT message on topic
    uint16_t packetIdPub1 = client.publish(MQTT_PUB_TOPIC, placeholder);
    Serial.println();
    Serial.printf("Publishing a MQTT message on topic '%s' at QoS 0, packetId: %i\n", MQTT_PUB_TOPIC, packetIdPub1);;
    Serial.print("payload: ");
    Serial.println(placeholder);
    Serial.println();
    delay(debounceInterval);
    digitalWrite(NodeMCU_BUILTIN_LED, HIGH); // When the pin is 1/HIGH, the LED is off
  }

  // If the button is pressed, send an MQTT message
  if (digitalRead(buttonPin5) == LOW) {
    digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
    const char* placeholder = "Button 5";
    // Publish an MQTT message on topic
    uint16_t packetIdPub1 = client.publish(MQTT_PUB_TOPIC, placeholder);
    Serial.println();
    Serial.printf("Publishing a MQTT message on topic '%s' at QoS 0, packetId: %i\n", MQTT_PUB_TOPIC, packetIdPub1);;
    Serial.print("payload: ");
    Serial.println(placeholder);
    Serial.println();
    delay(debounceInterval);
    digitalWrite(NodeMCU_BUILTIN_LED, HIGH); // When the pin is 1/HIGH, the LED is off
  }

  // If the button is pressed, send an MQTT message
  if (digitalRead(buttonPin6) == LOW) {
    digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
    const char* placeholder = "Button 6";
    // Publish an MQTT message on topic
    uint16_t packetIdPub1 = client.publish(MQTT_PUB_TOPIC, placeholder);
    Serial.println();
    Serial.printf("Publishing a MQTT message on topic '%s' at QoS 0, packetId: %i\n", MQTT_PUB_TOPIC, packetIdPub1);;
    Serial.print("payload: ");
    Serial.println(placeholder);
    Serial.println();
    delay(debounceInterval);    
    digitalWrite(NodeMCU_BUILTIN_LED, HIGH); // When the pin is 1/HIGH, the LED is off
  }

  // If the button is pressed, send an MQTT message
  if (digitalRead(buttonPin7) == LOW) {
    digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
    const char* placeholder = "Button 7";
    // Publish an MQTT message on topic
    uint16_t packetIdPub1 = client.publish(MQTT_PUB_TOPIC, placeholder);
    Serial.println();
    Serial.printf("Publishing a MQTT message on topic '%s' at QoS 0, packetId: %i\n", MQTT_PUB_TOPIC, packetIdPub1);;
    Serial.print("payload: ");
    Serial.println(placeholder);
    Serial.println();
    delay(debounceInterval);
    digitalWrite(NodeMCU_BUILTIN_LED, HIGH); // When the pin is 1/HIGH, the LED is off
  }

  currentAnalogValue = analogRead(analogInputPin0);

  // If the button is pressed, send an MQTT message
  if (currentAnalogValue == 1024) {    
    // print the readings in the Serial Monitor
    Serial.print("Pin A0 value: ");
    Serial.print(currentAnalogValue);
    digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
    const char* placeholder = "Button 0";
    // Publish an MQTT message on topic
    uint16_t packetIdPub1 = client.publish(MQTT_PUB_TOPIC, placeholder);
    Serial.println();
    Serial.printf("Publishing a MQTT message on topic '%s' at QoS 0, packetId: %i\n", MQTT_PUB_TOPIC, packetIdPub1);;
    Serial.print("payload: ");
    Serial.println(placeholder);
    Serial.println();
    delay(debounceInterval);
    digitalWrite(NodeMCU_BUILTIN_LED, HIGH); // When the pin is 1/HIGH, the LED is off  
  }

  delay(100);
  server.handleClient();

}

void callback(char* topic, byte* payload, unsigned int length) {
  digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
  String mqttPayload;
  String mqttTopic = topic;
  for (int i = 0; i < length; i++) {
    mqttPayload += (char)payload[i];
  }
  buffer[nextIndex] = mqttPayload;
  nextIndex = (nextIndex + 1) % bufferSize; // Increment index and wrap around if necessary
  Serial.println();
  Serial.println("Received a MQTT message.");
  Serial.println("topic: " + mqttTopic);
  Serial.println("payload: " + mqttPayload);
  digitalWrite(NodeMCU_BUILTIN_LED, HIGH); // When the pin is 1/HIGH, the LED is off
}

void handleRoot() {
  digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
  String response;
  response += "ESP8266 MQTT payload display web server.\n\nOlder payloads near top.\n\n";
  for (int i = 0; i < bufferSize; i++) {
    response += "payload: ";
    response += buffer[(nextIndex + i) % bufferSize] + "\n"; // Add messages in the correct order (newest near the bottom to oldest at the top)
  }
  response += "\nNewer payloads near bottom.\n\nESP8266 MQTT payload display web server.";
  server.send(200, "text/plain", response);
  digitalWrite(NodeMCU_BUILTIN_LED, HIGH); // When the pin is 1/HIGH, the LED is off
}