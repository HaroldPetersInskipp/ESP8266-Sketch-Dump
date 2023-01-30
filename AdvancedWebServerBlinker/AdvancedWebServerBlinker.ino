// ESP8266WebServer with built-in LED monitoring by Harold Peters Inskipp

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// SET WIFI SSID & PASSWORD BELOW
#ifndef STASSID
#define STASSID "wifi_ssid"
#define STAPSK  "your_password"
#endif

// Declare variables for WiFi ssid & password
const char *ssid = STASSID;
const char *password = STAPSK;

// Declare variables for the built in LED's
const int ESP8266_BUILTIN_LED = 2;
const int NodeMCU_BUILTIN_LED = 16;
// Both BUILTIN_LED's operate in “inverted” mode, with regard to the pin levels
// When the pin is 1/HIGH, the LED is off
// When the pin is 0/LOW, the LED is on

// Declare variable for analog pin A0 values, and set to 0
int sensorValue = 0;

// Declare an object of ESP8266WebServer library using port 80
ESP8266WebServer server(80);

// Handle the root directory response of the server
void handleRoot() {
  // Turn the LED ON by making the voltage 0/LOW
  digitalWrite(ESP8266_BUILTIN_LED, 0);

  // Declare a 400 character array initialized to null characters
  char temp[400];

  // Declare variables for time
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  // Content of page to serve
  snprintf(temp, 400,
  "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello world from a ESP8266</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
  </body>\
  </html>",
  hr, min % 60, sec % 60);

  // Serve code 200, content type, and contents
  server.send(200, "text/html", temp);

  // Turn the LED OFF by making the voltage 1/HIGH
  digitalWrite(ESP8266_BUILTIN_LED, 1);
}

// Handle File Not Found response of the server
void handleNotFound() {
  // Turn the LED ON by making the voltage 0/LOW
  digitalWrite(ESP8266_BUILTIN_LED, 0);

  // Message to serve
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  // Serve code 404, content type, and message
  server.send(404, "text/plain", message);

  // Turn the LED OFF by making the voltage 1/HIGH
  digitalWrite(ESP8266_BUILTIN_LED, 1);
}

// [REQUIRED] "setup" runs once at beginning of program
void setup(void) {
  pinMode(ESP8266_BUILTIN_LED, OUTPUT); // Initialize the ESP8266's built in LED as an output
  pinMode(NodeMCU_BUILTIN_LED, OUTPUT); // Initialize the NodeMCU's built in LED as an output
  pinMode(A0,INPUT);                    // Initialize the analog pin A0 as an input
  digitalWrite(NodeMCU_BUILTIN_LED, 1); // Set the initial state of the NodeMCU_BUILTIN_LED to OFF by making the voltage 1/HIGH
  digitalWrite(ESP8266_BUILTIN_LED, 1); // Set the initial state of the ESP8266_BUILTIN_LED to OFF by making the voltage 1/HIGH
  Serial.begin(115200);                 // Set serial baud rate
  WiFi.mode(WIFI_STA);                  // Set WiFi mode
  WiFi.begin(ssid, password);           // Connect to a WiFi network with ssid and password
  Serial.println("");                   // Print a "\n" in serial output

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    // Wait for 0.5 of a second
    delay(500);
    // Print a period in serial output while connecting
    Serial.print(".");          
  }

  // Print Wifi status messages
  Serial.println("");                   // Print a "\n" in serial output
  Serial.print("Connected to SSID: ");
  Serial.println(ssid);                 // Print SSID in serial output
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());       // Print IP address in serial output
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());    // Print MAC address in serial output

  // Print MDNS status messages
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  // Specify URLs and code to execute for incoming HTTP requests
  server.on("/", handleRoot);
  server.on("/inline", []() {
    // Serve code 200, content type, and contents
    server.send(200, "text/plain", "this works as well");
  });

  // Handle HTTP requests for URLs that don't exist
  server.onNotFound(handleNotFound);

  // Start the server
  server.begin();   

  // Print server status in serial output                   
  Serial.println("HTTP server started");  
}

// [REQUIRED] "loop" runs over-and-over-and-over until the device is reset
void loop(void) {
  delay(200);                             // Wait for 0.2 of a second
  digitalWrite(NodeMCU_BUILTIN_LED, 0);   // Turn the LED ON by making the voltage 0/LOW
  delay(200);                             // Wait for 0.2 of a second
  digitalWrite(NodeMCU_BUILTIN_LED, 1);   // Turn the LED OFF by making the voltage 1/HIGH
  delay(1000);                            // Wait for 1 second
  server.handleClient();                  // Update server
  MDNS.update();                          // Update MDNS
  Serial.println("Working...");           // Display a message in serial output
  delay(200);                             // Wait for 0.2 of a second
  sensorValue = analogRead(A0);           // Read the analog value of the A0 pin
  Serial.print("Analog Pin Value : ");    // Display a message in serial output
  Serial.println(sensorValue);            // Display the analog value in serial output
}