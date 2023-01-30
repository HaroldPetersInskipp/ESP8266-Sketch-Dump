#include <Arduino.h>

const int buttonPin = D3;   // Button input
const float voltage = 3.3;  // Reference voltage
const int rangePin = D4;    // Range selection pin

int range = 0;              // Range setting (0-2)

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize button and range selection pins
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(rangePin, INPUT_PULLUP);
}

void loop() {
  // Check if button is pressed
  if (digitalRead(buttonPin) == LOW) {
    // Read the voltage across the resistor
    float resVoltage = analogRead(A0) * voltage / 1024.0;
    //Calculate resistance
    float resistance = (voltage - resVoltage) / resVoltage;
    // Check the range selection
    if (digitalRead(rangePin) == LOW) {
        range++;  // increment the range setting when the rangePin is pressed
        delay(200);  // delay to debounce
    } 
    // Range 0: 0-1k Ohms
    if (range % 3 == 0) {
      resistance *= 1000;
      Serial.println("Range 0: 0-1k Ohms");
    } 
    // Range 1: 1k-10k Ohms
    else if (range % 3 == 1) {
      resistance *= 10000;
      Serial.println("Range 1: 1k-10k Ohms");
    } 
    // Range 2: 10k-100k Ohms
    else if (range % 3 == 2) {
      resistance *= 100000;
      Serial.println("Range 2: 10k-100k Ohms");
    }

    // Print resistance to the serial port in a detailed way
    Serial.print("Resistance: ");
    Serial.print(resistance);
    Serial.println(" Ohms");
    delay(100);  // Delay to prevent debouncing
  }
}
