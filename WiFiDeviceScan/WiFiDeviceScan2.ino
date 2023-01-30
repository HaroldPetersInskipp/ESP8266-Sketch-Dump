#include <WiFi.h>
#include <WiFiUdp.h>

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

    for (int i = 0; i < 255; i++) {
        WiFiUDP Udp;
        Udp.beginPacket("10.0.0." + String(i), 2000);
        Udp.write("HELLO");
        Udp.endPacket();
        delay(5);
    }

    int cb = WiFi.scanNetworks();
    for (int i = 0; i < cb; i++) {
        Serial.print(WiFi.SSID(i));
        Serial.print(", ");
        Serial.println(WiFi.BSSIDstr(i));
    }
  }
}
