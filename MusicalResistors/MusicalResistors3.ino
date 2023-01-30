#include <ResistanceToFrequency.h>

// Pin definitions
const int buttonPin = D3;
const int buzzerPin = D4;
const int analogPin = A0;

// Create the object
ResistanceToFrequency rtf;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    // Read value from analog pin
    int resistance = analogRead(analogPin);
    //Convert resistance to frequency
    float frequency = rtf.getFrequency(resistance);
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
