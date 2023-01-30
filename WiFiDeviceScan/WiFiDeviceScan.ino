#include <ESP8266WiFi.h>
#include <ESPping.h>

const char* ssid = "wifi_ssid";
const char* password = "your_password";

void setup() {
Serial.begin(115200);
WiFi.begin(ssid, password);
Serial.println("Connecting to WiFi...");
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("Connected to WiFi!");
Serial.println("Scanning for devices on the same WiFi network...");
delay(3000); // give some time for WiFi to fully connect
}

void loop() {
WiFi.disconnect(); // disconnect from WiFi to ensure a fresh scan
WiFi.reconnect(); // reconnect to WiFi
int n = WiFi.scanNetworks();
Serial.println("Number of devices found: " + String(n));
for (int i = 0; i < n; i++) {
IPAddress ip = WiFi.localIP();
byte mac[6];
WiFi.macAddress(mac);
String macString = "";
for (int i = 0; i < 6; i++) {
macString += String(mac[i], HEX);
if (i < 5) {
macString += ":";
}
}
String deviceInfo = "Device " + String(i+1) + ": " + "IP Address: " + ip.toString() + " MAC Address: " + macString;
deviceInfo += " Hostname: " + WiFi.hostname(ip);
deviceInfo += " Signal Strength: " + String(WiFi.RSSI(ip)) + "dBm";

// determine device type based on MAC address prefix
String macPrefix = macString.substring(0, 8);
String deviceType;
if (macPrefix == "00:23:69") {
  deviceType = "Smartphone";
} else if (macPrefix == "00:50:F2") {
  deviceType = "Laptop";
} else if (macPrefix == "00:12:17") {
  deviceType = "Smart TV";
} else {
  deviceType = "Unknown";
}
deviceInfo += " Device Type: " + deviceType;

Serial.println(deviceInfo);

// ping device and display results
PingResult result = ping(ip);
if (result.success) {
  Serial.println("Ping successful. Latency: " + String(result.time) + "ms");
} else {
  Serial.println("Ping failed");
}
delay(500); // delay between pings to avoid overwhelming the network

}