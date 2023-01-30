#include <Arduino.h>

// Define the pins for the buttons
#define BUTTON_1  D1
#define BUTTON_2  D2
#define BUTTON_3  D3
#define BUTTON_4  D4
#define BUTTON_5  D5
#define BUTTON_6  D6
#define BUTTON_7  D7
const int analogInputPin0 = A0;

const int NodeMCU_BUILTIN_LED = D0; // External LED (GPIO16) pin D0
const int debounceInterval = 500;   // Set the debounce interval for the buttons, in milliseconds (1000 = 1 second)
int currentAnalogValue = 0;         // Initalize the currentAnalogValue to 0

void setup() {
  // Initialize the serial port
  Serial.begin(115200);

  delay(debounceInterval);

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

  // Initilize the following built in LED's as outputs and set their starting state
  Serial.println("Setting the following built in LED's as outputs.");
  pinMode(NodeMCU_BUILTIN_LED, OUTPUT);
  digitalWrite(NodeMCU_BUILTIN_LED, HIGH); // When the pin is 1/HIGH, the LED is off
  //digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
  Serial.println("LED: 'NodeMCU_BUILTIN_LED' (GPIO16)");

  // Set the following button pins as inputs with pull-up resistors
  Serial.println("Setting the following pins as inputs with pull-up resistors for buttons.");
  pinMode(analogInputPin0, INPUT);
  Serial.println("Pin: 'A0' as button 0 to roll 1d4");
  pinMode(BUTTON_1, INPUT_PULLUP);
  Serial.println("Pin: 'D1' as button 1 to roll 1d6");
  pinMode(BUTTON_2, INPUT_PULLUP);
  Serial.println("Pin: 'D2' as button 2 to roll 1d8");
  pinMode(BUTTON_3, INPUT_PULLUP);
  pinMode(BUTTON_4, INPUT_PULLUP);
  Serial.println("Pin: 'D4' as button 4 to roll 1d12");
  pinMode(BUTTON_5, INPUT_PULLUP);
  Serial.println("Pin: 'D5' as button 5 to roll 1d20");
  pinMode(BUTTON_6, INPUT_PULLUP);
  Serial.println("Pin: 'D6' as button 6 to roll 1d100");
  pinMode(BUTTON_7, INPUT_PULLUP);
  Serial.println("Pin: 'D7' as button 7 to roll 1dcustom");


  // End the boot process and print client ready message
  Serial.println("Ending the boot process...\n");
  Serial.println("RNG dice roller started successfully.\n");
}

void loop() {
  currentAnalogValue = analogRead(analogInputPin0);
  
  // Check if button 0 is pressed
  if (currentAnalogValue == 1024) {
    // Roll a 4-sided die and print the result to the serial port
    int roll = random(1, 5);
    Serial.print("Rolling a 4-sided die: ");
    Serial.println(roll);
    if (roll >= 3) {
      digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
    }
    delay(debounceInterval);
  }
  
  // Check if button 1 is pressed
  else if (digitalRead(BUTTON_1) == LOW) {
    // Roll a 6-sided die and print the result to the serial port
    int roll = random(1, 7);
    Serial.print("Rolling a 6-sided die: ");
    Serial.println(roll);
    if (roll >= 4) {
      digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
    }
    delay(debounceInterval);
  }
  
  // Check if button 2 is pressed
  else if (digitalRead(BUTTON_2) == LOW) {
    // Roll a 8-sided die and print the result to the serial port
    int roll = random(1, 9);
    Serial.print("Rolling a 8-sided die: ");
    Serial.println(roll);
    if (roll >= 5) {
      digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
    }
    delay(debounceInterval);
  }
  
  // Check if button 3 is pressed
  else if (digitalRead(BUTTON_3) == LOW) {
    // Roll a 10-sided die and print the result to the serial port
    int roll = random(1, 11);
    Serial.print("Rolling a 10-sided die: ");
    Serial.println(roll);
    if (roll >= 6) {
      digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
    }
    delay(debounceInterval);
  }
  
  // Check if button 4 is pressed
  else if (digitalRead(BUTTON_4) == LOW) {
    // Roll a 12-sided die and print the result to the serial port
    int roll = random(1, 13);
    Serial.print("Rolling a 12-sided die: ");
    Serial.println(roll);
    if (roll >= 7) {
      digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
    }
    delay(debounceInterval);
  }
  
  // Check if button 5 is pressed
  else if (digitalRead(BUTTON_5) == LOW) {
    // Roll a 20-sided die and print the result to the serial port
    int roll = random(1, 21);
    Serial.print("Rolling a 20-sided die: ");
    Serial.println(roll);
    if (roll >= 11) {
      digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
    }
    delay(debounceInterval);
  }
  
  // Check if button 6 is pressed
  else if (digitalRead(BUTTON_6) == LOW) {
    // Roll a 100-sided die and print the result to the serial port
    int roll = random(1, 101);
    Serial.print("Rolling a 100-sided die: ");
    Serial.println(roll);
    if (roll >= 51) {
      digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
    }
    delay(debounceInterval);
  }

  // Check if button 7 is pressed
  else if (digitalRead(BUTTON_7) == LOW) {
    // Roll a custom die and print the result to the serial port
    int sides = readCustomDieSides();
    int roll = random(1, sides+1);
    Serial.print("Rolling a custom die with ");
    Serial.print(sides);
    Serial.println(" sides");
    Serial.print("Result: ");
    Serial.println(roll);
    delay(debounceInterval);
  }
  
  delay(100);
  digitalWrite(NodeMCU_BUILTIN_LED, HIGH); // When the pin is 1/HIGH, the LED is off
}

int readCustomDieSides(){
  int sides = 0;
  while(sides <= 0){
    Serial.println("Creating a custom die to roll");
    Serial.println("Enter the number of sides:");
    while(Serial.available()<=0);
    sides = Serial.parseInt();
  }
  return sides;
}  Serial.println("Pin: 'D3' as button 3 to roll 1d10");
