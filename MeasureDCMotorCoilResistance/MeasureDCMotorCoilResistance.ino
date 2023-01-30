#include <Arduino.h>

const int buttonPin1 = D2; // button 1 input pin
const int buttonPin2 = D3; // button 2 input pin
const int buttonPin3 = D4; // button 3 input pin
const int motorPin = D1;   // GPIO pin driving the motor
const int analogPin = A0;  // analog pin for measuring resistance

void setup() {
  // initialize serial communication and the button and motor pins
  Serial.begin(115200);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(motorPin, OUTPUT);
  Serial.println("To measure the coil resistance of a DC motor");
  Serial.println("Press Button 1/D2 to start the motor");
  Serial.println("Press Button 2/D3 to stop the motor");
  Serial.println("Press Button 3/D4 to measure the coil resistance");
}

void loop() {
  // check if button 1 is pressed
  if (digitalRead(buttonPin1) == LOW) {
    // start the motor and wait for it to reach full speed
    digitalWrite(motorPin, HIGH);
    Serial.println("Starting DC motor");
    delay(1000);
  }

  // check if button 2 is pressed
  if (digitalRead(buttonPin2) == LOW) {
    // stop the motor
    digitalWrite(motorPin, LOW);
    Serial.println("Stopping DC motor");
  }

  // check if button 3 is pressed
  if (digitalRead(buttonPin3) == LOW) {
    // read the resistance of the motor
    int resistance = analogRead(analogPin);

    // convert the resistance to ohms and print it out
    float ohms = (float)resistance * (5.0 / 1023.0);
    Serial.print("Coil resistance: ");
    Serial.print(ohms);
    Serial.println(" Ohms");
  }

  // wait for a bit before checking the buttons again
  delay(100);
}

// TODO: include button debouncing