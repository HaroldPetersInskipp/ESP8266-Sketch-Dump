#include <Arduino.h>

const int buttonPin = D3;   // Button input
const float voltage = 3.3;  // Reference voltage
const int rangePin = D4;    // Range selection pin

int range = 0;              // Range setting (0-3)

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  delay(1000);

  // Initialize button and range selection pins
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(rangePin, INPUT_PULLUP);
  // Print the heading for the serial data
  Serial.println("Ohmmeter Data:");
  Serial.println("-----------------");
  Serial.println("Range settings (0-3)");
  Serial.println("Range 0: 0-1k Ohms");
  Serial.println("Range 1: 1k-10k Ohms");
  Serial.println("Range 2: 10k-100k Ohms");
  Serial.println("Range 3: 100k-1M Ohms");
  Serial.println("-----------------");
  Serial.println("Setting default Range to 0: 0k-1k Ohms");
  Serial.println("-----------------");
  Serial.println("CONTROLS:");
  Serial.print("1. Press the button connected to (GPIO");
  Serial.print(rangePin);
  Serial.println(")\n   To cycle through Range settings (0-3)\n");
  Serial.print("2. Press the button connected to (GPIO");
  Serial.print(buttonPin);
  Serial.println(")\n   To take measurements");
  Serial.println("-----------------");
}

void loop() {
  // Check the range selection
  if (digitalRead(rangePin) == LOW) {
    if (range == 0) {
      Serial.println("Setting Range to 1: 1k-10k Ohms");
      Serial.println("-----------------");
      range += 1;  // increment the range setting when the rangePin is pressed
      delay(250);  // delay to debounce
    }
    else if (range == 1) {
      Serial.println("Setting Range to 2: 10k-100k Ohms");
      Serial.println("-----------------");
      range += 1;  // increment the range setting when the rangePin is pressed
      delay(250);  // delay to debounce
    }
    else if (range == 2) {
      Serial.println("Setting Range to 3: 100k-1M Ohms");
      Serial.println("-----------------");
      range += 1;  // increment the range setting when the rangePin is pressed
      delay(250);  // delay to debounce
    }
    else if (range >= 3) {
      Serial.println("Setting Range to 0: 0-1k Ohms");
      Serial.println("-----------------");     
      range = 0;
      delay(250);  // delay to debounce
    }
  }

  // Check if button is pressed
  if (digitalRead(buttonPin) == LOW) {
    // Read the voltage across the resistor
    float A0reading = analogRead(A0);
    Serial.print("Pin A0 reading: ");
    Serial.println(A0reading);
    float resVoltage = A0reading * voltage / 1024.0;
    //Calculate resistance
    float resistance = (voltage - resVoltage) / resVoltage;
    // Range 0: 0-1k Ohms
    if (range == 0) {
      resistance *= 1000;
      Serial.println("Range 0: 0-1k Ohms");
    } 
    // Range 1: 1k-10k Ohms
    else if (range == 1) {
      resistance *= 10000;
      Serial.println("Range 1: 1k-10k Ohms");
    } 
    // Range 2: 10k-100k Ohms
    else if (range == 2) {
      resistance *= 100000;
      Serial.println("Range 2: 10k-100k Ohms");
    }
    // Range 3: 100k-1M Ohms
    else if (range == 3) {
      resistance *= 1000000;
      Serial.println("Range 3: 100k-1M Ohms");
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
    current *= 1000;
    Serial.print("Measured current: ");
    Serial.print(current);
    Serial.println(" Milliamperes");
    // Print the power dissipation on the resistor
    float power = resVoltage * current;
    Serial.print("Power dissipation: ");
    Serial.print(power);
    Serial.println(" Watts");
    // Print separator for the next data
    Serial.println("-----------------");
    delay(250);  // delay to debounce
  }
}
