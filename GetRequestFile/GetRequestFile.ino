//Here is an updated version of the sketch that uses "wget" to download a small file and outputs the file contents to the serial port on a button press:

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "wifi_ssid";
const char* password = "your_password";

const int buttonPin = D1; // GPIO pin connected to the button

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP); // set buttonPin as an input with a pull-up resistor

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
}

void loop() {
  if (digitalRead(buttonPin) == LOW) { // button is pressed
    HTTPClient http;
    http.begin("https://www.example.com/small-file.txt");
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.println("Error on HTTP request: " + http.errorToString(httpCode));
    }
    http.end();
  }
  delay(100); // debounce delay
}
/*
This sketch will connect to the WiFi network specified by ssid and password, then wait for a button press on the buttonPin GPIO pin. When the button is pressed, it will use HTTPClient to download the small file located at "https://www.example.com/small-file.txt" and output the file contents to the serial port. The button press is debounced using a delay of 100 milliseconds.
*/