#include <Time.h>
#include <TimeLib.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "wifi_ssid";
const char *pass = "your_password";

const long kTimeZone = -7;
int gDay, gMonth, gYear, gSeconds, gMinutes, gHours;
int gDayLast, gMonthLast, gYearLast, gSecondsLast, gMinutesLast, gHoursLast;
long int gTimeNow;
const char *myMonthStr[] = { "", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
const char *myDowStr[] = {"", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
String gToday;

// NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Query interval
#define kNtpInterval 10000

// Set the timer to trigger immediately.
unsigned long gNtpTimer = 0;

void setup() {
  Serial.begin(115200);
  gDayLast = gMonthLast = gYearLast = gSecondsLast = gMinutesLast = gHoursLast = 0;
  // We start by connecting to a WiFi network
  setupWiFi();
  // The timeClient does all the NTP work.
  timeClient.begin();
}

void loop() {
    // Check time
    long now = millis();
  if(now >= gNtpTimer) {
    gNtpTimer = now + kNtpInterval;
    timeClient.update();
    // Get epoch time and adjust it according to the local time zone.
    gTimeNow = timeClient.getEpochTime() + (kTimeZone * 3600);
    gDay = day(gTimeNow);
    gMonth = month(gTimeNow);
    gYear = year(gTimeNow);
    gHours = hourFormat12(gTimeNow);
    gMinutes = minute(gTimeNow);
    gSeconds = second(gTimeNow);
    gToday = String(myDowStr[weekday(gTimeNow)]) + " " + String(myMonthStr[gMonth]) + " " + String(gDay) + " " + String(gYear);
    Serial.println();
    Serial.print("Today is: ");
    Serial.println(gToday);
    Serial.print("Timestamp: ");
    Serial.println(gTimeNow);
    Serial.print("Date: ");
    Serial.print(gMonth);
    Serial.print("/");
    Serial.print(gDay);
    Serial.print("/");
    Serial.println(gYear);
    Serial.print("Current time: ");
    Serial.print(gHours);
    Serial.print(":");
    if (gMinutes < 10) {
    Serial.print(0);
    }
    Serial.print(gMinutes);
    Serial.print(":");
    if (gSeconds < 10) {
    Serial.print(0);
    }
    Serial.println(gSeconds);
  }
}

void setupWiFi(void) {
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.disconnect(true);
  delay(100);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.begin(ssid, pass);
  delay(100);
  byte count = 0;
  String connectingStr = "Connecting";
  while(WiFi.status() != WL_CONNECTED && count < 100)
  {
    count ++;
    delay(500);
    Serial.println(connectingStr);
    // Display a dot for each attempt.
    connectingStr += ".";
  }
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Failed to connect to WiFi");
    delay(500);
  }
}