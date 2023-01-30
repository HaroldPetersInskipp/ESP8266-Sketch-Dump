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
  // Print the heading for the serial data
  Serial.println("Ohmmeter Data:");
  Serial.println("-----------------");
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
      Serial.println("Range: 0-1k Ohms");
    } 
    // Range 1: 1k-10k Ohms
    else if (range % 3 == 1) {
      resistance *= 10000;
      Serial.println("Range: 1k-10k Ohms");
    } 
    // Range 2: 10k-100k Ohms
    else if (range % 3 == 2) {
      resistance *= 100000;
      Serial.println("Range: 10k-100k Ohms");
    }

    // Print resistance to the serial port in a detailed way
    Serial.print("Measured resistance: ");
    Serial.print(resistance);
    Serial.println(" Ohms");
    // Print the voltage across the resistor
    Serial.print("Measured voltage: ");
    Serial.print(resVoltage);
    Serial.println(" Volts");
    // Print the current through the resistor
    float current = resVoltage / resistance;
    Serial.print("Measured current: ");
    Serial.print(current);
    Serial.println(" Amps");
    // Print the power dissipation on the resistor
    float power = resVoltage * current;
    Serial.print("Power dissipation: ");
    Serial.print(power);
    Serial.println(" Watts");
    // Print separator for the next data
    Serial.println("-----------------");
    delay(1000);  // Delay to prevent debouncing
  }
}
