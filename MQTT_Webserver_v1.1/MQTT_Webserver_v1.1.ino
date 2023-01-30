#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>

const char* ssid = "wifi_ssid";
const char* password = "your_password";
const char* mqttServer = "10.0.0.111";
const int mqttPort = 1883;
const char* mqttUser = "MQTT_USERNAME";
const char* mqttPassword = "your_password";
const char* esp8266unitNumber = "ESP8266Client_3";

WiFiClient espClient;
PubSubClient client(espClient);
ESP8266WebServer server(80);

const int bufferSize = 10; // Maximum number of messages to store in the buffer
String buffer[bufferSize]; // Buffer to store MQTT messages
int nextIndex = 0; // Index to add the next message to the buffer

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


WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.println("Connecting to WiFi..");
}
Serial.println("Connected to the WiFi network");
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
}
server.on("/", handleRoot);
server.begin();
Serial.println("Web server started");
}

void loop() {
client.loop();
server.handleClient();
}

void callback(char* topic, byte* payload, unsigned int length) {
String message;
for (int i = 0; i < length; i++) {
message += (char)payload[i];
}
buffer[nextIndex] = message;
nextIndex = (nextIndex + 1) % bufferSize; // Increment index and wrap around if necessary
Serial.println("Received message: " + message);
}

void handleRoot() {
String response;
for (int i = 0; i < bufferSize; i++) {
response += buffer[(nextIndex + i) % bufferSize] + "\n"; // Add messages in the correct order
}
server.send(200, "text/plain", response);
}