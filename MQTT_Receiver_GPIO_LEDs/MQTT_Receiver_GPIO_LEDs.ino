#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "wifi_ssid";
const char* password = "your_password";
const char* mqttServer = "10.0.0.111";
const int mqttPort = 1883;
const char* mqttUser = "MQTT_USERNAME";
const char* mqttPassword = "your_password";
const char* esp8266unitNumber = "ESP8266Client_1";

// Declare variables for the built in LED's
const int ESP8266_BUILTIN_LED = 2;
const int NodeMCU_BUILTIN_LED = 16;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  // Set the pin modes
  pinMode(ESP8266_BUILTIN_LED, OUTPUT); // Initialize the ESP8266's built in LED as an output
  pinMode(NodeMCU_BUILTIN_LED, OUTPUT); // Initialize the NodeMCU's built in LED as an output
  digitalWrite(NodeMCU_BUILTIN_LED, 1); // Set the initial state of the NodeMCU_BUILTIN_LED to OFF by making the voltage 1/HIGH
  digitalWrite(ESP8266_BUILTIN_LED, 1); // Set the initial state of the ESP8266_BUILTIN_LED to OFF by making the voltage 1/HIGH
  setupWiFi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void setupWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(esp8266unitNumber, mqttUser, mqttPassword)) {
      Serial.println("connected");
      client.subscribe("gpio/set");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println();
  Serial.println("Message arrived");
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  String message;

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.println(message);

  if (String(topic) == "gpio/set") {
    int pinNumber = message.substring(0, message.indexOf("/")).toInt();
    String pinState = message.substring(message.indexOf("/") + 1);

    // Check the pin number
    if (pinNumber == 2) {
      // Check that the pin state is a valid value
      if (pinState == "ON") {
        digitalWrite(ESP8266_BUILTIN_LED, LOW);
      } else if (pinState == "OFF") {
        digitalWrite(ESP8266_BUILTIN_LED, HIGH);
      } else {
        Serial.println("[Error] in MQTT message payload. Pin state must be 'ON' or 'OFF'.");
      }
    } else if (pinNumber == 16) {
      // Check that the pin state is a valid value
      if (pinState == "ON") {
        digitalWrite(NodeMCU_BUILTIN_LED, LOW);
      } else if (pinState == "OFF") {
        digitalWrite(NodeMCU_BUILTIN_LED, HIGH);
      } else {
        Serial.println("[Error] in MQTT message payload. Pin state must be 'ON' or 'OFF'.");
      }
    } else {
      Serial.println("[Error] in MQTT message payload.");
      Serial.println("Expected 'pinNumber/pinState' like '2/ON'.");
      Serial.println("Pin number must be '2' or '16', Pin state must be 'ON' or 'OFF'.");
    }
  }
}
