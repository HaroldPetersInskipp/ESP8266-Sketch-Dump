#include <Arduino.h>

// Pin definitions
const int buttonPin = D3;
const int buzzerPin = D4;
const int analogPin = A0;

// Constant to convert resistance value to frequency
const float resistanceConstant = 0.01;
// Minimum and maximum frequencies
const int minFrequency = 261;
const int maxFrequency = 523;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    // Read value from analog pin
    int resistance = analogRead(analogPin);
    //Convert resistance to frequency
    float frequency = map(resistance * resistanceConstant, 0, 1023, minFrequency, maxFrequency);
    // Play the note
    tone(buzzerPin, frequency);
    // Wait for button release
    while (digitalRead(buttonPin) == LOW) {
      delay(10);
    }
    // Stop the note
    noTone(buzzerPin);
  }
}
