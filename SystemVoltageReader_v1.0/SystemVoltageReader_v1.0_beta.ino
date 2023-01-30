#include <Arduino.h>

const int buttonPin = D3;   // Button input
const float voltage = 3.3;  // Reference voltage
const int knownResistorPin = A0; // Pin for measuring known resistor voltage
float knownResistance = 470.0; // Value of known resistor (Ohms)

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize button pin
  pinMode(buttonPin, INPUT_PULLUP);

  // Print the heading for the serial data
  Serial.println("Ohmmeter Data:");
  Serial.println("-----------------");
}

void loop() {
  // Check if button is pressed
  if (digitalRead(buttonPin) == LOW) {
    // Read the voltage across the known resistor
    float knownResistorVoltage = analogRead(knownResistorPin) * voltage / 1024.0;
    // Calculate resistance of the unknown resistor
    float unknownResistance = knownResistance * (voltage / knownResistorVoltage - 1);
    // Print resistance to the serial port in a detailed way
    Serial.print("Measured resistance: ");
    Serial.print(unknownResistance);
    Serial.println(" Ohms");
    // Print separator for the next data
    Serial.println("-----------------");
    delay(1000);  // Delay to prevent debouncing
  }
}
