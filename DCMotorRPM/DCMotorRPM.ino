#include <Arduino.h>

// Constants
const int numReadings = 5;       // number of readings to take for each RPM measurement
const int analogPin = A0;        // pin to read from
const int motorPolePairs = 2;    // number of pole pairs in the motor
const float samplePeriod = 0.2;  // time between each RPM measurement, in seconds

// Variables
int readings[numReadings];       // array to hold analog readings
int readIndex = 0;               // index of the current reading in the array
float rpm = 0;                   // current RPM of the motor
unsigned long lastSampleTime;    // time of the last RPM measurement

void setup() {
  // Set up serial communication to display RPM in the Arduino IDE serial plotter
  Serial.begin(115200);

  // Initialize the lastSampleTime variable
  lastSampleTime = millis();
}

void loop() {
  // Take multiple readings and average them
  readings[readIndex] = analogRead(analogPin);
  readIndex = (readIndex + 1) % numReadings;

  // Calculate the average of the readings
  int sum = 0;
  for (int i = 0; i < numReadings; i++) {
    sum += readings[i];
  }
  float average = (float)sum / numReadings;

  // Calculate RPM based on the average value
  rpm = (60 / (samplePeriod * motorPolePairs)) * average;

  // Display RPM in the serial plotter
  Serial.println(rpm);

  // Wait until it's time to take the next RPM measurement
  while (millis() - lastSampleTime < samplePeriod * 1000) {
    // Do nothing
  }
  lastSampleTime = millis();
}
