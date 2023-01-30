#include <Arduino.h>

// Pin definitions
const int buttonPin = D3;
const int buzzerPin = D4;
const int analogPin = A0;

// Array of notes and note frequencies
const int notes[] = {261, 277, 293, 311, 329, 349, 369, 392, 415, 440, 466, 493, 523};

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    // Read value from analog pin
    int analogValue = analogRead(analogPin);
    // Calculate the corresponding note index
    int noteIndex = map(analogValue, 0, 1023, 0, 12);
    // Play the note
    tone(buzzerPin, notes[noteIndex]);
    // Wait for button release
    while (digitalRead(buttonPin) == LOW) {
      delay(10);
    }
    // Stop the note
    noTone(buzzerPin);
  }
}
