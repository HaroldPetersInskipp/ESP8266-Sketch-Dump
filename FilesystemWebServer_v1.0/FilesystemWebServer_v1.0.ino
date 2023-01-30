#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

const char* ssid = "wifi_ssid";
const char* password = "your_password";

ESP8266WebServer server(80);

// Function to handle file upload
void handleFileUpload() {
  HTTPUpload& upload = server.upload();
  if(upload.status == UPLOAD_FILE_START){
    String filename = upload.filename;
    if(!filename.startsWith("/")){
      filename = "/" + filename;
    }
    FSInfo fs_info;
    SPIFFS.info(fs_info);
    Serial.printf("Upload: %s, %lu\n", filename.c_str(), upload.totalSize);
    if(fs_info.totalBytes + upload.totalSize > fs_info.totalBytes){
      Serial.println("Not enough space!");
      server.send(500, "text/plain", "Not enough space!");
      return;
    }
    File file = SPIFFS.open(filename, "w");
    if(!file){
      Serial.println("File open failed");
      server.send(500, "text/plain", "File open failed");
      return;
    }
  } else if(upload.status == UPLOAD_FILE_WRITE){
    if(upload.currentSize > 0){
      File file = SPIFFS.open(upload.filename, "a");
      if(file){
        file.write(upload.buf, upload.currentSize);
        file.close();
      }
    }
  } else if(upload.status == UPLOAD_FILE_END){
    String file_url = String("http://") + WiFi.localIP().toString() + String("/") + upload.filename;
    Serial.printf("Uploaded file: %s, %lu bytes\n", upload.filename.c_str(), upload.totalSize);
    server.send(200, "text/plain", "Upload successful! File can be accessed at " + file_url);
  }
}

void setup() {
  // Initialize SPIFFS filesystem
  SPIFFS.begin();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Serve HTML page to display file system
  server.on("/", [](){
    String output = "<html><body>";
    output += "<h1>File System</h1>";
    output += "<form action='/upload' method='post' enctype='multipart/form-data'>";
    output += "<input type='file' name='uploadFile'><br>";
    output += "<input type='submit' value='Upload File'><br>";
    output += "</form>";
    output += "<br><br>";
    output += "<h2>Files:</h2>";
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
    String fileName = dir.fileName();
    output += "<a href='/display?file=" + fileName + "'>" + fileName + "</a><br>";
    }
    output += "</body></html>";
    server.send(200, "text/html", output);
    });

    // Handle file upload
    server.on("/upload", HTTP_POST, {
    handleFileUpload();
    }, handleFileUpload);

    // Handle file display
    server.on("/display", {
    String fileName = server.arg("file");
    String output = "<html><body>";
    output += "<h1>File: " + fileName + "</h1>";
    output += "<pre>";
    File file = SPIFFS.open(fileName, "r");
    if(file){
    while(file.available()){
    output += (char)file.read();
    }
    file.close();
    } else {
    output += "File not found!";
    }
    output += "</pre>";
    output += "</body></html>";
    server.send(200, "text/html", output);
    });

    // Start the server
    server.begin();
}

void loop() {
    server.handleClient();
}