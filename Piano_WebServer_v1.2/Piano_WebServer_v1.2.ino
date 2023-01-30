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
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Serve web page
  server.on("/", [](){
    String html = "<html><body>";
    html += "<h1>Piano</h1>";
    for (int i = 1; i <= 8; i++) { // adding octaves
        for (int j = 0; j < 12; j++) {
        int note = notes[j] * pow(2, i - 4);
        html += "<button type='button' onClick='playNote(" + String(note) + ")'>"+note_names[j]+" "+ String(i) +"</button>";
      }
      html += "<br>";
    }
    html += "<script>";
    html += "function playNote(note) { var audioCtx = new (window.AudioContext || window.webkitAudioContext)(); var oscillator = audioCtx.createOscillator(); oscillator.type = 'square'; oscillator.frequency.value = note; oscillator.connect(audioCtx.destination); oscillator.start(); setTimeout(() => oscillator.stop(), 500); }";
    html += "</script>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });

  // Start server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
}