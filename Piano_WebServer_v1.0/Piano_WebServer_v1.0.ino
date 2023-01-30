#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "wifi_ssid";
const char* password = "your_password";

// Array of notes in the octave
const int notes[] = {262, 294, 330, 349, 392, 440, 494};

// Current octave
int octave = 4;

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
    html += "<form>";
    html += "Octave: <input type='number' name='octave' min='1' max='8' value='" + String(octave) + "'><br>";
    for (int i = 0; i < 7; i++) {
      int note = notes[i] * pow(2, octave - 4);
      html += "<button type='button' onClick='playNote(" + String(note) + ")'>Note " + String(i) + "</button>";
    }
    html += "<br><input type='submit' value='submit'>"
    html += "</form>";
    html += "<script>";
    html += "function playNote(note) { var audioCtx = new (window.AudioContext || window.webkitAudioContext)(); var oscillator = audioCtx.createOscillator(); oscillator.type = 'square'; oscillator.frequency.value = note; oscillator.connect(audioCtx.destination); oscillator.start(); setTimeout(() => oscillator.stop(), 500); }";
    html += "</script>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });

  // Handle form submission
  server.on("/", HTTP_POST, [](){
    if (server.hasArg("octave")) {
      octave = server.arg("octave").toInt();
    }
    server.send(302, "text/plain", "");
    server.sendHeader("Location", "/");
  });

  // Start server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
}