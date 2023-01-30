#include <ESP8266WiFi.h>
#include <curl/curl.h>

const char* ssid = "wifi_ssid";
const char* password = "your_password";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
}

void loop() {
  CURL *curl;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://www.example.com/small-file.txt");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteCallback);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      Serial.println("curl_easy_perform() failed: " + String(curl_easy_strerror(res)));
    }
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  delay(5000); // download the file every 5 seconds
}

size_t curlWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
  size_t numBytes = size * nmemb;
  Serial.write((const uint8_t*)ptr, numBytes);
  return numBytes;
}

/*
This sketch will connect to the WiFi network specified by ssid and password, then use curl to download the small file located at "https://www.example.com/small-file.txt" and output the file contents to the serial port. The file will be downloaded every 5 seconds as specified by the delay in the loop function.
*/