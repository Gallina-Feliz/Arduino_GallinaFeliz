#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "Xxx";
const char* password = "Juan2005**";

ESP8266WebServer server(80);

float weight = 0.0;
float distance = 0.0;

void setup(void) {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Esperar conexión
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado a ");
  Serial.println(ssid);
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/plain", "Hello from esp8266 server web!");
  });

  server.on("/data", []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");

    // Crear documento JSON
    StaticJsonDocument<200> doc;
    doc["weight"] = weight;
    doc["distance"] = distance;

    String json;
    serializeJson(doc, json);

    // Enviar respuesta JSON
    server.send(200, "application/json", json);
  });

  server.onNotFound([]() {
    server.send(404, "text/plain", "Uri not found " + server.uri());
  });

  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop(void) {
  // Leer datos del puerto serial
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    int weightIndex = data.indexOf("W,");
    int distanceIndex = data.indexOf(",D,");
    
    if (weightIndex != -1 && distanceIndex != -1) {
      weight = data.substring(weightIndex + 2, distanceIndex).toFloat();
      distance = data.substring(distanceIndex + 3).toFloat();
      Serial.print("Peso recibido: ");
      Serial.println(weight);
      Serial.print("Distancia recibida: ");
      Serial.println(distance);
    }
  }

  server.handleClient();
}

