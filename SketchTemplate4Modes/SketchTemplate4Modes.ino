#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

// Declare counter variable to keep track of time elapsed since last boot
static int counter = 0; 

// Declare ssid and password variables to connect to a local WiFi network using the ESP8266WiFi library
const char* ssid = "wifi_ssid";
const char* password = "your_password";
const char* ap_ssid = "ESP8266AP";

// Set mode to STATION, ACCESS POINT, ANALOG AVERAGE, or LAST BOOT  by commenting out all but one of the following lines
 const char* mode = "STATION";
// const char* mode = "ACCESS POINT";
// const char* mode = "ANALOG AVERAGE";
// const char* mode = "LAST BOOT";

// Declare variables for the built in LED's
const int ESP8266_BUILTIN_LED = 2;  // Onboard LED (GPIO2)
const int NodeMCU_BUILTIN_LED = 16; // External LED (GPIO16)
// Both BUILTIN_LED's operate in “inverted” mode, with regard to the pin levels
// When the pin is 1/HIGH, the LED is off
// When the pin is 0/LOW, the LED is on
const int analogPin = A0;  // analog pin for measuring
const int NUM_SAMPLES = 10;  // Number of samples to take of CPU usage

void setup() {
  // Initialize serial communication and set baud rate to 115200
  Serial.begin(115200);

  // Wait for serial communication
    while (!Serial) {
      ;
    }

  pinMode(ESP8266_BUILTIN_LED, OUTPUT); // Initialize the ESP8266's built in LED as an output
  pinMode(NodeMCU_BUILTIN_LED, OUTPUT); // Initialize the NodeMCU's built in LED as an output
  pinMode(A0,INPUT);                    // Initialize the analog pin A0 as an input
  digitalWrite(NodeMCU_BUILTIN_LED, 1); // Set the initial state of the NodeMCU_BUILTIN_LED to OFF by making the voltage 1/HIGH
  digitalWrite(ESP8266_BUILTIN_LED, 1); // Set the initial state of the ESP8266_BUILTIN_LED to OFF by making the voltage 1/HIGH

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

  if (mode == "STATION") {
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

void loop() {

  // Display number of clients connected to the access point
  if (mode == "ACCESS POINT") {
    // Blink the external LED to indicate device usage
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
    // Blink the external LED to indicate device usage
    digitalWrite(ESP8266_BUILTIN_LED, 0);
    WiFiClient client;
    HTTPClient http;
    if (http.begin(client, "http://time.jsontest.com/")) {      
      // start connection and send HTTP header
      int httpCode = http.GET();
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.println();
        Serial.printf("[HTTP] GET request response code: %d\n", httpCode);
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          StaticJsonDocument<200> doc;
          doc["msg"] = payload;
          serializeJsonPretty(doc, Serial);
          Serial.println();
        }
      } else {
        Serial.println("HTTP GET request failed");
      }
      http.end();
    } else {
      Serial.println("HTTP Unable to connect");
    }
    // START BLINKER DEMO
    const int delayTime = 300;
    const int numBlinks = 200;
    for (int i = 0; i < numBlinks; i++) {
      digitalWrite(ESP8266_BUILTIN_LED, 1);
      delay(delayTime);
      digitalWrite(ESP8266_BUILTIN_LED, 0);
      delay(delayTime);
    }
    // END BLINKER DEMO


  } else if (mode == "LAST BOOT") {
    // Blink the external LED to indicate device usage
    digitalWrite(ESP8266_BUILTIN_LED, 0);
    // Display time since last boot in ten second intervals
    Serial.print("Last boot: ");
    Serial.print(counter);
    Serial.println(" seconds ago");
    counter += 1; // Increment counter by 1 second
    delay(1000);  // Delay for 1 second


  } else if (mode == "ANALOG AVERAGE") {
    // read the activity level
    int activityLevel = readActivityLevelAverage();
    Serial.print("Average:");
    Serial.println(activityLevel);
    // Print the average value to the serial port
    delay(250);
  }

  digitalWrite(ESP8266_BUILTIN_LED, 1); // Set the state of the ESP8266_BUILTIN_LED to OFF by making the voltage 1/HIGH
  delay(100);

  // Do NOTHING and loop or put MORE CODE HERE

}

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

  int activityLevel = averageValue;

  return activityLevel;
}