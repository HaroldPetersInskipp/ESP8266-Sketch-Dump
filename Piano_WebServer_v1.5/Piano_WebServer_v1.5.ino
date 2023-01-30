// ESP8266 NodeMCU Piano Keyboard Web Server by Harold Peters Inskipp

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "wifi_ssid";
const char* password = "your_password";

// Array of notes and note names
const int notes[] = {261, 277, 293, 311, 329, 349, 369, 392, 415, 440, 466, 493, 523};
const char* note_names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

// Create server instance
ESP8266WebServer server(80);

void setup() {
  
  Serial.begin(115200);

  delay(500);

  // Print boot message
  Serial.println("\n\nStarting the boot process...");

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

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi succesfully");

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

  // Serve web page
  server.on("/", [](){
    String html = "<html><head><style>";
    html += ".key { width: 24px; height: 100px; font-size: 10px; text-align: center; float: left; margin-right: -1px; display: inline-flex; }";
    html += ".white { background-color: white; }";
    html += ".black { background-color: black; color: white; }";
    html += ".piano { width: 100%; display: flex; flex-wrap:wrap; }";
    html += ".white.key { width: 24px; }";
    html += ".black.key { width: 22px; height: 80px; }";
    html += "</style></head><body>";
    html += "<h1>ESP8266 NodeMCU Piano Keyboard</h1>";
    html += "<div class='piano'>";
    for (int i = 1; i <= 8; i++) {
      for (int j = 0; j < 12; j++) {
        int note = notes[j] * pow(2, i - 4);
        String color = "white";
        if (j == 1 || j == 3 || j == 6 || j == 8 || j == 10) {
          color = "black";
        }
        html += "<button class='key "+color+"' onClick='playNote(" + String(note) + ")'>" + note_names[j] + " " + String(i) + "</button>";
    }
    }
    html += "</div>";
    html += "<script>";
    html += "function playNote(note) { var audioCtx = new (window.AudioContext || window.webkitAudioContext)(); var oscillator = audioCtx.createOscillator(); oscillator.type = 'square'; oscillator.frequency.value = note; oscillator.connect(audioCtx.destination); oscillator.start(); setTimeout(() => oscillator.stop(), 250); }";
    html += "</script>";
    html += "</body></html>";
    server.send(200, "text/html", html);
    });

    // Start server
    server.begin();
    Serial.println("ESP8266 NodeMCU Piano Keyboard web server has started");
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");

}

void loop() {
server.handleClient();
}