#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "wifi_ssid";
const char* password = "your_password";

const int buttonPin = D3;
bool buttonPressed = false;

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(buttonPin, INPUT_PULLUP);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  buttonPressed = digitalRead(buttonPin) == LOW;

  if (buttonPressed) {
    Serial.println("Scanning for devices...");
    int n = WiFi.scanNetworks();

    for (int i = 0; i < n; i++) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.print((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      
      //resolve hostname
      IPAddress ip = WiFi.hostByName(WiFi.SSID(i));
      if (ip == INADDR_NONE) {
        Serial.println(" (not a IOT device) ");
      } else {
        Serial.print(" (IOT device IP: ");
        Serial.print(ip);
        Serial.println(")");
      }
      delay(10);
    }
  }
}


error: invalid conversion from 'int' to 'uint8_t*' {aka 'unsigned char*'} [-fpermissive]
   25 |       uint8_t * mac = WiFi.BSSID(i);
Compilation error: invalid conversion from 'const char*' to 'char*' [-fpermissive]