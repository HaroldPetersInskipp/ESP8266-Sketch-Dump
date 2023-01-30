#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <time.h>

// Declare counter variable to keep track of time elapsed since last boot
static int counter = 0; 

// Declare ssid and password variables to connect to a local WiFi network using the ESP8266WiFi library
const char* ssid = "wifi_ssid";
const char* password = "your_password";
const char* ap_ssid = "ESP8266AP";

// Set WiFi mode to STATION or ACCESS POINT by commenting out only one of the following lines
const char* mode = "STATION";
// const char* mode = "ACCESS POINT";

void setup() {
  // Initialize serial communication and set baud rate to 115200
  Serial.begin(115200);

  // Wait for serial communication
  while (!Serial) {
    ;
  }
  
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
      delay(1000);
      Serial.println("Connecting..");
    }
    Serial.println("Connected to the WiFi network successfully");

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

  // TODO: Update section to - Print current time from web call
  Serial.print("Current time: ");
  time_t now = time(nullptr);
  Serial.println(ctime(&now));
}

void loop() {
  // Display number of clients connected to the access point
  if (mode == "ACCESS POINT") {
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
  }

  if (mode == "STATION") {

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
    delay(120000);

  }

  // Display time since last boot in ten second intervals
  Serial.print("Time since last boot: ");
  Serial.print(counter);
  Serial.println(" seconds");
  counter += 10; // Increment counter by 10 seconds
  delay(10000);  // Delay for 10 seconds

  // Do NOTHING and loop or put MORE CODE HERE
}