#include <ESP8266WiFi.h>
#include <ESPping.h>

// Replace with your network credentials
const char* ssid = "wifi_ssid";
const char* password = "your_password";

void setup() {

  Serial.begin(115200);

  delay(1000);

  // Connect to WiFi
  WiFi.mode(WIFI_STA); 
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi succesfully");
  Serial.print("IP: ");
  Serial.print(WiFi.localIP());
  Serial.print(" Gateway: ");
  Serial.println(WiFi.gatewayIP());
  Serial.println("You can try to ping me ;-)");
}

void loop() {
  // Ping IP
  for (uint8_t i = 1; i < 254; i++) {
    const IPAddress remote_ip(10,0,0,i);
    Serial.print(remote_ip);
    Ping.ping(remote_ip, 1);
    Serial.printf(" response time : %d/%.2f/%d", Ping.minTime(), Ping.averageTime(), Ping.maxTime());
    Serial.println("ms");
    delay(2000);
  }

}
