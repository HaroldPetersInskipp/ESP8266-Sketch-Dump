#include <Arduino.h>

// ESP8266 NodeMCU Pin definitions
const int buttonPin = D3; // Button input and built in FLASH button

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  delay(500);
  
  // Initialize button pin
  pinMode(buttonPin, INPUT_PULLUP);

  // Print boot message
  Serial.println("\n\nStarting the boot process...");

  // Print firmware version
  Serial.print("Firmware version: ");
  Serial.println(ESP.getSdkVersion());

  // Print details about the device's firmware
  Serial.print("Boot version: ");
  Serial.println(ESP.getBootVersion());
  Serial.print("Boot mode: ");
  Serial.println(ESP.getBootMode());
  Serial.print("CPU frequency: ");
  Serial.println(ESP.getCpuFreqMHz());
  Serial.print("Flash size: ");
  Serial.println(ESP.getFlashChipSize());

  // Print memory usage
  Serial.print("Free heap: ");
  Serial.println(ESP.getFreeHeap());
  Serial.print("Heap fragmentation: ");
  Serial.println(ESP.getHeapFragmentation());

  // Print chip information
  Serial.print("Chip ID: ");
  Serial.println(ESP.getChipId());
  Serial.print("Flash chip ID: ");
  Serial.println(ESP.getFlashChipId());

  // Print boot complete message
  Serial.println("Ending the boot process...\n\n");
  Serial.println("Press the onboard FLASH button (pin D3) to read the system voltage");
}

void loop() {
  // Check if button is pressed
  if (digitalRead(buttonPin) == LOW) {
    //Read system voltage
    int voltage = analogRead(A0);
    //Conversion to millivolts
    voltage = (voltage / 1024.0) * ESP.getVcc(); 
    // Print voltage to the serial port in a detailed way
    Serial.print("System Voltage: ");
    Serial.print(voltage);
    Serial.println(" mV");
    delay(250); // Delay to prevent debouncing
  }
}
