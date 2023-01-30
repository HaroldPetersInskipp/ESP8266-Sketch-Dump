#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

// Set mode to STATION, ACCESS POINT, ANALOG AVERAGE, or LAST BOOT  by commenting out all but one of the following lines
 const char* mode = "STATION";
// const char* mode = "ACCESS POINT";
// const char* mode = "ANALOG AVERAGE";
// const char* mode = "LAST BOOT";

// Declare ssid and password variables to connect to a local WiFi network using the ESP8266WiFi library
const char* ssid = "wifi_ssid";
const char* password = "your_password";
const char* ap_ssid = "ESP8266AP";

// Declare variable to keep track of measurments
static int counter = 0;      // Counter variable to keep track of time elapsed since last boot
const int analogPin = A0;    // Set analog pin for measuring
const int NUM_SAMPLES = 10;  // Number of samples to take when averaging measurments

// Declare variables for the built in LED's
const int ESP8266_BUILTIN_LED = 2;  // Onboard LED (GPIO2)
const int NodeMCU_BUILTIN_LED = 16; // External LED (GPIO16)
// Both BUILTIN_LED's operate in “inverted” mode, with regard to the pin levels
// When the pin is 1/HIGH, the LED is off
// When the pin is 0/LOW, the LED is on

// [REQUIRED] Function "setup" that runs once at beginning of program
void setup() {
  // Initialize serial communication and set baud rate to 115200
  Serial.begin(115200);

  // Wait for serial communication
  while (!Serial) {
    ;
  }

  // Initialize ESP8266 NodeMCU pins/LEDs and set their states
  pinMode(A0,INPUT);                    // Initialize the analog pin A0 as an input
  pinMode(ESP8266_BUILTIN_LED, OUTPUT); // Initialize the ESP8266's built in LED as an output
  pinMode(NodeMCU_BUILTIN_LED, OUTPUT); // Initialize the NodeMCU's built in LED as an output
  digitalWrite(ESP8266_BUILTIN_LED, 1); // Set the initial state of the ESP8266_BUILTIN_LED to OFF by making the voltage 1/HIGH
  digitalWrite(NodeMCU_BUILTIN_LED, 1); // Set the initial state of the NodeMCU_BUILTIN_LED to OFF by making the voltage 1/HIGH
  
  // Print boot message
  Serial.println("\nStarting boot process...");
  
  // Print firmware version
  Serial.print("Firmware version: ");
  Serial.println(ESP.getSdkVersion());
  
  // Print details about the device's firmware using the ESP8266WiFi library
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

  if (mode == "ACCESS POINT") {
    // Blink the onboard LED to indicate WiFi connection attempt
    digitalWrite(ESP8266_BUILTIN_LED, !digitalRead(ESP8266_BUILTIN_LED));
    // Setup in access point mode
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ap_ssid, password);
    Serial.print("Initializing ");
    Serial.print(ap_ssid);
    Serial.println(" Access point");
    Serial.print("Access point IP address: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("Access point SSID: ");
    Serial.println(ap_ssid);
    Serial.println("Access point is now available for clients to connect");
  }

  if (mode != "ACCESS POINT") {
    // Setup in station mode
    WiFi.mode(WIFI_STA);

    // Begin connection to WiFi using the ESP8266WiFi library
    WiFi.begin(ssid, password);

    // Print Wifi connection setup messages
    Serial.println("Initializing connection to the WiFi network");
    while (WiFi.status() != WL_CONNECTED) {
      // Blink the onboard LED to indicate WiFi connection attempt
      digitalWrite(ESP8266_BUILTIN_LED, !digitalRead(ESP8266_BUILTIN_LED));
      delay(1000);
      Serial.println("Connecting..");
    }
    Serial.println("Connected to the WiFi network successfully");

    // Turn the LED OFF by making the voltage 1/HIGH
    digitalWrite(ESP8266_BUILTIN_LED, 1);

    // Print STATION mode specific details about the device's WiFi capabilities
    Serial.print("WiFi SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("WiFi local IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("WiFi subnet mask: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("WiFi gateway IP address: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("WiFi DNS server IP address: ");
    Serial.println(WiFi.dnsIP());
  }

  // Print the remaining details about the device's WiFi capabilities
  Serial.print("WiFi mode: ");
  Serial.println(WiFi.getMode());
  Serial.print("WiFi channel: ");
  Serial.println(WiFi.channel());
  Serial.print("WiFi MAC address: ");
  Serial.println(WiFi.macAddress());

  // Print WiFi connection status
  Serial.print("WiFi status: ");
  Serial.println(WiFi.status());

  // Print WiFi signal strength
  Serial.print("WiFi RSSI: ");
  Serial.println(WiFi.RSSI());
}

// [REQUIRED] Function "loop" that runs over-and-over-and-over until the device is reset
void loop() {
  // Display number of clients connected to the access point
  if (mode == "ACCESS POINT") {
    // Turn the LED ON by making the voltage 0/LOW
    digitalWrite(ESP8266_BUILTIN_LED, 0);
    // Blank line in serial
    Serial.println();
    // Get the number of connected clients
    int numClients = WiFi.softAPgetStationNum();
    Serial.print("Number of connected clients: ");
    Serial.println(numClients);
    // Print the details of each connected client
    for (int i = 0; i < numClients; i++) {
      Serial.print("  Client #");
      Serial.println(i + 1);
    }
    delay(1000);


  } else if (mode == "STATION") {
    // Turn the LED ON by making the voltage 0/LOW
    digitalWrite(ESP8266_BUILTIN_LED, 0);

    // START OF HTTP CODE //
    WiFiClient client;
    HTTPClient http;
    if (analogRead(analogPin) >= 30) {
      Serial.println();
      Serial.println("Button has been pressed");
      if (http.begin(client, "http://time.jsontest.com/")) { 
        int httpCode = http.GET();
        // httpCode will be negative on error
        Serial.printf("HTTP GET request response code: %d\n", httpCode);
        if (httpCode > 0) {
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = http.getString();
            DynamicJsonDocument doc(1024);
            doc["msg"] = payload;
            serializeJson(doc, Serial);
            Serial.println();
          }
        } else {
          Serial.println("HTTP GET request failed");
        }
        http.end();
      } else {
        Serial.println("HTTP Unable to connect");
      }
    }
    // END OF HTTP CODE //

    delay(500);

  } else if (mode == "LAST BOOT") {
    // Turn the LED ON by making the voltage 0/LOW
    digitalWrite(ESP8266_BUILTIN_LED, 0);
    // Display time since last boot in ten second intervals
    Serial.print("Last boot: ");
    Serial.print(counter);
    Serial.println(" seconds ago");
    counter += 1; // Increment counter by 1 second
    delay(1000);  // Delay for 1 second


  } else if (mode == "ANALOG AVERAGE") {
    // Turn the LED ON by making the voltage 0/LOW
    digitalWrite(ESP8266_BUILTIN_LED, 0);
    // read the activity level
    int activityLevel = readActivityLevelAverage();
    Serial.print("Average:");
    Serial.println(activityLevel);
    // Print the average value to the serial port
    delay(250);
  }
  // Turn the LED OFF by making the voltage 1/HIGH
  digitalWrite(ESP8266_BUILTIN_LED, 1);
  delay(100);

  // Do NOTHING and loop or put MORE CODE HERE

}

// Function "readActivityLevelAverage" to read and display the average value of pin A0
int readActivityLevelAverage() {
  const int upperThreshhold = 25;
  const int lowerThreshhold = 15;
  int sum = 0;
  int numValues = 0;

  // Read the analog value from pin A0
  int analogValue = analogRead(analogPin);

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

  // Calculate the average value
  // Add the analog value to the sum
  sum += analogValue;
  numValues++;
  int averageValue = sum / numValues;
  if (numValues >= 10) {
    // Reset the sum and number of values
    sum = 0;
    numValues = 0;
  }

  int activityLevel = averageValue;

  return activityLevel;
}