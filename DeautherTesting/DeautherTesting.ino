#include <ESP8266WiFi.h>

const int buttonPin = D3;
int deauth_count = 5; // Number of deauthentication packets to send
int max_press_count = 100; // Maximum number of times the button can be pressed
int deauth_interval = 10; // Minimum time interval between deauthentication attempts (in seconds)
bool deauth_enabled = true; // Flag to enable/disable deauthentication feature
int press_count = 0; // Counter for button press
unsigned long last_deauth_time = 0; // Timestamp of last deauthentication attempt
int current_ssid = 0; // Index of the current network SSID being scanned

enum menu_options {
  SCAN_NETWORKS = 1,
  SELECT_NETWORK = 2,
  DEAUTH_DEVICES = 3,
  CHANGE_SETTINGS = 4,
  EXIT = 5
};

void printMenu() {
  Serial.println("Please select an option:");
  Serial.println("1. Scan for networks");
  Serial.println("2. Select a network");
  Serial.println("3. Deauthenticate devices");
  Serial.println("4. Change settings");
  Serial.println("5. Exit");
}

void scanNetworks() {
  int networks = WiFi.scanNetworks();
  delay(500);
  if(networks == 0) {
    Serial.println("No networks found");
    return;
  }
  Serial.println("Found networks:");
  for(int i = 0; i < networks; i++) {
    Serial.println(i + 1 + ". " + WiFi.SSID(i));
  }
}

void selectNetwork() {
  scanNetworks();
  Serial.print("Enter the number of the network you wish to connect to: ");
  while(!Serial.available());
  int network_num = Serial.parseInt();
  if(network_num < 1 || network_num > WiFi.scanNetworks()) {
    Serial.println("Invalid option selected");
    return;
  }
  current_ssid = network_num - 1;
  String ssid = WiFi.SSID(current_ssid);
  Serial.println("Connecting to " + ssid + "...");
  WiFi.begin(ssid.c_str(), NULL); // if the network is open you can use this line
  delay(1000);
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to " + ssid);
    return;
  }
  Serial.println("Successfully connected to " + ssid);
}


void deauthDevices() {
  if(current_ssid < 0) {
    Serial.println("Please select a network first");
    return;
  }
  Serial.print("Are you sure you want to deauthenticate devices on " + WiFi.SSID(current_ssid) + "? (y/n): ");
  while(!Serial.available());
  char confirm = Serial.read();
  if(confirm != 'y') {
    Serial.println("Deauthentication cancelled");
    return;
  }
  Serial.println("Sending deauthentication packets...");
  WiFi.disconnect(true); // Disconnect and send deauthentication packets to connected devices
  Serial.println("Deauthentication process completed successfully.");
}

void changeSettings() {
  Serial.println("Current settings:");
  Serial.println("Deauth count: " + String(deauth_count));
  Serial.println("Deauth interval: " + String(deauth_interval));
  Serial.println("Max press count: " + String(max_press_count));
  Serial.println("Deauth enabled: " + String(deauth_enabled));
  Serial.println("Enter new values or leave blank to keep current value:");
  Serial.print("Deauth count: ");
  while(!Serial.available());
  String input = Serial.readString();
  if(input.length() > 0) {
    deauth_count = input.toInt();
  }
  Serial.print("Deauth interval: ");
  while(!Serial.available());
  input = Serial.readString();
  if(input.length() > 0) {
    deauth_interval = input.toInt();
  }
  Serial.print("Max press count: ");
  while(!Serial.available());
  input = Serial.readString();
  if(input.length() > 0) {
    max_press_count = input.toInt();
  }
  Serial.print("Deauth enabled (y/n): ");
  while(!Serial.available());
  char input_char = Serial.read();
  if(input_char == 'y') {
    deauth_enabled = true;
  } else if (input_char == 'n') {
    deauth_enabled = false;
  }
  Serial.println("Settings updated");
}

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Set button pin as input with pull-up resistor
  WiFi.disconnect(); // Disconnect from any previous connections
  WiFi.mode(WIFI_STA); // Set ESP8266 to station mode
  Serial.begin(115200);
  delay(2000);
  current_ssid = -1;
}

void loop() {
  if(digitalRead(buttonPin) == LOW) { // Check if button is pressed

    if(!deauth_enabled) {
      Serial.println("Deauthentication feature is currently disabled");
      return;
    }
    if(press_count >= max_press_count) {
      Serial.println("Maximum press limit reached");
      return;
    }
    if((millis() - last_deauth_time) < (deauth_interval * 100)) {
      Serial.println("Deauthentication attempt too soon. Please wait");
      return;
    }
    press_count++;
    last_deauth_time = millis();
    printMenu();
    while(!Serial.available());
    int option = Serial.parseInt();
    switch (option) {
      case SCAN_NETWORKS:
      scanNetworks();
      break;
      case SELECT_NETWORK:
      selectNetwork();
      break;
      case DEAUTH_DEVICES:
      deauthDevices();
      break;
      case CHANGE_SETTINGS:
      changeSettings();
      break;
      case EXIT:
      return;
      default:
      Serial.println("Invalid option selected");
      break;
    }
  }
}
