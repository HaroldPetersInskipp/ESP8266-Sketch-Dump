#include <Arduino.h>

// Define the pins for the buttons
#define BUTTON_1  D1
#define BUTTON_2  D2
#define BUTTON_3  D3
#define BUTTON_4  D4
#define BUTTON_5  D5
#define BUTTON_6  D6
#define BUTTON_7  D7
#define BUTTON_8  A0

void setup() {
  // Initialize the serial port
  Serial.begin(115200);

  // Set the button pins as inputs
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);
  pinMode(BUTTON_4, INPUT);
  pinMode(BUTTON_5, INPUT);
  pinMode(BUTTON_6, INPUT);
  pinMode(BUTTON_7, INPUT);
  //pinMode(BUTTON_8, INPUT);
}

void loop() {
    
  // Check if button 1 is pressed
  if (digitalRead(BUTTON_1) == LOW) {
    // Roll a 4-sided die and print the result to the serial port
    int roll = random(1, 5);
    Serial.print("Rolling a 4-sided die: ");
    Serial.println(roll);
  }
  
  // Check if button 2 is pressed
  else if (digitalRead(BUTTON_2) == LOW) {
    // Roll a 6-sided die and print the result to the serial port
    int roll = random(1, 7);
    Serial.print("Rolling a 6-sided die: ");
    Serial.println(roll);
  }
  
  // Check if button 3 is pressed
  else if (digitalRead(BUTTON_3) == LOW) {
    // Roll a 8-sided die and print the result to the serial port
    int roll = random(1, 9);
    Serial.print("Rolling a 8-sided die: ");
    Serial.println(roll);
  }
  
  // Check if button 4 is pressed
  else if (digitalRead(BUTTON_4) == LOW) {
    // Roll a 10-sided die and print the result to the serial port
    int roll = random(1, 11);
    Serial.print("Rolling a 10-sided die: ");
    Serial.println(roll);
  }
  
  // Check if button 5 is pressed
  else if (digitalRead(BUTTON_5) == LOW) {
    // Roll a 12-sided die and print the result to the serial port
    int roll = random(1, 13);
    Serial.print("Rolling a 12-sided die: ");
    Serial.println(roll);
  }
  
  // Check if button 6 is pressed
  else if (digitalRead(BUTTON_6) == LOW) {
    // Roll a 20-sided die and print the result to the serial port
    int roll = random(1, 21);
    Serial.print("Rolling a 20-sided die: ");
    Serial.println(roll);
  }
  
  // Check if button 7 is pressed
  else if (digitalRead(BUTTON_7) == LOW) {
    // Roll a 100-sided die and print the result to the serial port
    int roll = random(1, 101);
    Serial.print("Rolling a 100-sided die: ");
    Serial.println(roll);
  }
  
  // Check if button 8 is pressed
  else if (digitalRead(BUTTON_8) == LOW) {
    // Roll a custom die and print the result to the serial port
    int sides = readCustomDieSides();
    int roll = random(1, sides+1);
    Serial.print("Rolling a custom die with ");
    Serial.print(sides);
    Serial.print(" sides : ");
    Serial.println(roll);
  }
  
  delay(10);
}

int readCustomDieSides(){
  int sides = 0;
  while(sides <= 0){
    Serial.println("Enter the number of sides for the die:");
    while(Serial.available()<=0);
    sides = Serial.parseInt();
  }
  return sides;
}