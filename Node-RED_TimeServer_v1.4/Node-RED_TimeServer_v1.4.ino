#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// Replace with a different esp8266unitNumber to identify multiple ESP8266 NodeMCU devices
const char* esp8266unitNumber = "ESP8266Client_1";  // Set a esp8266unitNumber to identify each ESP8266 NodeMCU (example "ESP8266Client_1")

// Replace with your network credentials
const char* WIFI_SSID = "wifi_ssid";                   // Set the WIFI ssid to connect to (example "WiFi SSID")
const char* WIFI_PASSWORD = "your_password";            // Set the WIFI password (example "Wifi Password")

// Replace with the IP address of your Node-RED API server
const char* NODERED_SERVER = "10.0.0.111";          // Set the Node-RED server ip address (example "10.0.0.111")
const int NODERED_PORT = 1880;                      // Set the Node-RED port number (default port is 1880)

// Declare variables for the built in LED's
const int NodeMCU_BUILTIN_LED = D0;    // Onboard LED (GPIO16) pin D0
// const int ESP8266_BUILTIN_LED = D4; // Onboard LED (GPIO2) pin D4
// When the pin is 1/HIGH, the LED is off
// When the pin is 0/LOW, the LED is on

// Replace with the correct timezone_offset for your location (ctrl-f "adjustment" to change the sign +-)
int timezone_offset = 7;

String line;
String timestamp;
String unix_timestamp;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(500);

  // Print boot message
  Serial.println("\n\nStarting the boot process...");

  // Print device identifier
  Serial.print("Device identifier: ");
  Serial.println(esp8266unitNumber);

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
  // digitalWrite(NodeMCU_BUILTIN_LED, HIGH); // When the pin is 1/HIGH, the LED is off
  digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on
  Serial.println("LED: 'NodeMCU_BUILTIN_LED' (GPIO16)");

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

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

  // Print the remaining details about the device's WiFi capabilities
  Serial.print("WiFi mode: ");
  Serial.println(WiFi.getPhyMode());
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

  // End the boot process and print client ready message
  Serial.println("Ending the boot process...\n");
  Serial.print(esp8266unitNumber);
  Serial.println(" should now be operational.\n");
}

void loop() {
  digitalWrite(NodeMCU_BUILTIN_LED, LOW); // When the pin is 0/LOW, the LED is on

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Rebooting");
    delay(200);
    Serial.end();
    delay(200);
    setup();
  } else {
    if (client.connect(NODERED_SERVER, NODERED_PORT)) {
      // Make a GET request to the API endpoint for a response
      client.println("GET /timeserver_all HTTP/1.1");
      client.println("Host: " + String(NODERED_SERVER));
      client.println("Connection: close");
      client.println();
    } else {
      Serial.println("Stuck here");
    }
    while (client.connected()) {
      while (client.available()) {
        line = client.readStringUntil('\r');
      }
    }

    String api_response = extractJson(line);
    Serial.print("\nAPI endpoint response: ");
    Serial.println(api_response);

    timestamp = getTimestamp(line);
    Serial.print("Timestamp: ");
    Serial.println(timestamp);

    unix_timestamp = String(getLocalUnixTimestamp());
    Serial.print("Unix Timestamp: ");
    Serial.println(unix_timestamp);

    String current_time = getCurrentTimeHHMMSS();
    Serial.print("Current time: ");
    Serial.println(current_time);

    client.stop();
  }

  digitalWrite(NodeMCU_BUILTIN_LED, HIGH); // When the pin is 1/HIGH, the LED is off
  delay(10000); // interval of 10s between each update
}

String extractJson(String json) {
  String response = json.substring(1, json.length());
  return response;
}

String getTimestamp(String json) {
  int startIndex = json.indexOf("timestamp") + 11; // start index of timestamp value
  int endIndex = json.indexOf(",", startIndex); // end index of timestamp value
  String response = json.substring(startIndex, endIndex);
  return response;
}

long long getLocalUnixTimestamp() {
  long long timestamp_long = strtoll(timestamp.c_str(), NULL, 10) / 1000;
  int adjustment = timezone_offset * 3600;
  // Change the below sign to a plus(+) or minus(-) depending on your timezone
  return timestamp_long - adjustment;
}

String getCurrentTimeHHMMSS() {
  time_t now = getLocalUnixTimestamp();
  char time[20];
  strftime(time, 20, "%r", localtime(&now));
  return String(time);
}