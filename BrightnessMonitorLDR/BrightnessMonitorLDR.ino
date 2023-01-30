#include <Arduino.h>
#include <ESP8266WiFi.h>

const int LDR_PIN = A0;  // Light Dependent Resistor connected to pin A0

void setup() {
  // Initialize serial communication at 115200 baud
  Serial.begin(115200);

  // Wait for the serial connection to be established
  while (!Serial) {
    ;
  }
}

void loop() {
  // Read the analog value from the LDR
  int ldrValue = analogRead(LDR_PIN);

  // Print the value to the serial plotter
  Serial.println(ldrValue);

  // Delay for a small amount of time
  delay(100);
}

/*
To wire the LDR to the ESP8266 NodeMCU ESP-12E microcontroller, you will need to follow these steps:

    Connect one leg of the LDR to a voltage divider circuit, which will reduce the voltage applied to the LDR to a level that the microcontroller's analog input can handle. To build the voltage divider, you will need two resistors with values between 1K and 10K ohms. Connect one end of one resistor to 3.3V and the other end to the LDR. Connect one end of the other resistor to the LDR and the other end to GND.

    Connect the other leg of the LDR to the analog input pin on the microcontroller. In this case, the LDR is connected to pin A0 on the ESP8266 NodeMCU ESP-12E.

    Connect the GND pin on the microcontroller to one of the GND pins on the ESP8266 NodeMCU ESP-12E.

    Connect the 3.3V pin on the microcontroller to one of the 3.3V pins on the ESP8266 NodeMCU ESP-12E.

Here is a diagram showing how the LDR should be wired to the microcontroller:

  3.3V -- resistor -- LDR -- resistor -- GND
                       |
                       A0

This wiring setup will allow you to measure the resistance of the LDR and use it to detect the brightness level. As the brightness level increases, the resistance of the LDR will decrease, resulting in a higher analog input value on the microcontroller.
*/