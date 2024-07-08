// Definiciones de pines
#define TRIG_PIN 12    // Pin TRIG del sensor ultrasónico
#define ECHO_PIN 11    // Pin ECHO del sensor ultrasónico

void setup() {
  Serial.begin(9600); // Comunicación con el ESP8266
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Leer distancia del sensor ultrasónico
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  int pingTravelTime = pulseIn(ECHO_PIN, HIGH);
  float distance = pingTravelTime * 0.034 / 2; // Convertir el tiempo de viaje en distancia

  // Enviar datos al ESP8266
  Serial.println(distance);

  delay(2000); // Esperar antes de la siguiente lectura
}
