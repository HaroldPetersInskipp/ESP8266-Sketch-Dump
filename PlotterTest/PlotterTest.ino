// Declare variables for the built in LED's
const int ESP8266_BUILTIN_LED = 2;
const int NodeMCU_BUILTIN_LED = 16;

const int upperThreshhold = 25;
const int lowerThreshhold = 15;

int sum = 0;
int numValues = 0;

void setup() {
  pinMode(ESP8266_BUILTIN_LED, OUTPUT);   // Initialize the ESP8266's built in LED as an output
  pinMode(NodeMCU_BUILTIN_LED, OUTPUT);   // Initialize the NodeMCU's built in LED as an output
  digitalWrite(NodeMCU_BUILTIN_LED, 1);   // Set the initial state of the NodeMCU_BUILTIN_LED to OFF by making the voltage 1/HIGH
  digitalWrite(ESP8266_BUILTIN_LED, 1);   // Set the initial state of the ESP8266_BUILTIN_LED to OFF by making the voltage 1/HIGH
  Serial.begin(115200);                   // Initialize serial communication
}

void loop() {
  // Read the analog value from pin A0
  int analogValue = analogRead(A0);

  if (analogValue >= upperThreshhold) {
    digitalWrite(NodeMCU_BUILTIN_LED, 0); // Turn the LED ON by making the voltage 0/LOW
  } else if (analogValue <= lowerThreshhold) {
    digitalWrite(NodeMCU_BUILTIN_LED, 0); // Turn the LED ON by making the voltage 0/LOW
  } else {
    digitalWrite(NodeMCU_BUILTIN_LED, 1); // Turn the LED OFF by making the voltage 1/HIGH
  }

  // Set serial plotter bound soft limit lines
  int serialPlotterLowerBound = 0;
  int serialPlotterUpperBound = 40;

  // Print the analog value of Pin A0 to the serial port with soft limits
  Serial.print("Pin A0:");
  Serial.println(analogValue);
  Serial.print("LowerBound:");
  Serial.println(serialPlotterLowerBound);
  Serial.print("UpperBound:");
  Serial.println(serialPlotterUpperBound);

  // Calculate the average value after a certain time period
  // Add the analog value to the sum
  sum += analogValue;
  numValues++;
  int averageValue = sum / numValues;
  if (numValues >= 10) {
    // Reset the sum and number of values
    sum = 0;
    numValues = 0;
  }

  // Print the average value to the serial port
  Serial.print("Average:");
  Serial.println(averageValue);

  // Delay for a small amount of time
  delay(250);
}