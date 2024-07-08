#include "HX711.h"

// Pines de conexión
const int LOADCELL_DOUT_PIN = A1;
const int LOADCELL_SCK_PIN = A0;

// Instancia del objeto HX711
HX711 scale;

// Peso conocido (en gramos) para la calibración
const float KNOWN_WEIGHT = 500.0; // Cambia esto por el peso que usarás para la calibración

void setup() {
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  // Esperar a que el sensor esté listo
  while (!scale.is_ready()) {
    Serial.println("Esperando al sensor...");
    delay(1000);
  }

  // Tara la balanza
  Serial.println("Tarando...");
  scale.tare();
  Serial.println("Sensor tarado. Por favor, coloca el peso conocido en la celda de carga.");

  // Esperar unos segundos para asegurar que el peso está colocado
  delay(5000);

  // Calcular el factor de calibración
  long reading = scale.read_average(20);
  float calibration_factor = reading / KNOWN_WEIGHT;
  Serial.print("Lectura del sensor: ");
  Serial.println(reading);
  Serial.print("Factor de calibración: ");
  Serial.println(calibration_factor);

  // Establecer el factor de calibración en el sensor
  scale.set_scale(calibration_factor);
  Serial.println("Calibración completa. Puedes remover el peso conocido.");

  // Esperar unos segundos para remover el peso
  delay(5000);

  // Tara nuevamente para asegurarse de que la balanza esté en cero
  scale.tare();
  Serial.println("Sensor tarado nuevamente. Listo para medir pesos.");
}

void loop() {
  // Leer y mostrar el peso
  if (scale.is_ready()) {
    float weight = scale.get_units(10);
    Serial.print("Peso: ");
    Serial.print(weight, 2);
    Serial.println(" g");

    // Enviar el peso al ESP8266
    Serial.print("W,");  // Prefijo para identificar el dato de peso
    Serial.println(weight, 2);
  } else {
    Serial.println("Esperando al sensor...");
  }

  delay(1000);
}
