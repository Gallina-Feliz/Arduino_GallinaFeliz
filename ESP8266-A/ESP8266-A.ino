#include <dummy.h>

/*
 *  WeMos D1 mini (esp8266)
 *  Simple web server with JSON response
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "Zzz";
const char* password = "123456780";

ESP8266WebServer server(80);

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/plain", "Hello from esp8266 server web!");
  });

  server.on("/inline", []() {
    
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/data", []() {
    // Generate random data
    server.sendHeader("Access-Control-Allow-Origin","*");
    // float sonido =random(898);

    // Create JSON document
    StaticJsonDocument<200> doc;
    doc["sonido"]= sonido; 

    String json;
    serializeJson(doc, json);

    // Send JSON response
    server.send(200, "application/json", json);
  });

  server.onNotFound([]() {
    server.send(404, "text/plain", "Uri not found " + server.uri());
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
