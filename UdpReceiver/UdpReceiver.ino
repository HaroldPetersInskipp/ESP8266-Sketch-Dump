#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "wifi_ssid";
const char* password = "your_password";

WiFiUDP Udp;
unsigned int localPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    // start listening for incoming UDP packets
    Udp.begin(localPort);
  }
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // read the packet into the buffer
    int len = Udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
    }
    // print the packet to the serial monitor
    Serial.println("Packet received: ");
    Serial.println(incomingPacket);
  }
}