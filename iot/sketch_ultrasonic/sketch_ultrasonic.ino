#include <Ultrasonic.h> 

#define INTERVALO_LEITURA 250 // (ms)

unsigned int distancia = 0;

// Define pinos de TRIGGER E ECHO do Sensor
#define PIN_TRIGGER 4
#define PIN_ECHO 5

// Inicializa Sensor
Ultrasonic ultrasonic(PIN_TRIGGER, PIN_ECHO);

void setup() {
  // Define velocidade de transmissão
  Serial.begin(115200);
}

void loop() {
  // retorna a distância do sensor ultrasonico
  distancia = ultrasonic.read();

  Serial.print("Distância in CM: ");
  Serial.println(distancia);
  delay(INTERVALO_LEITURA);
}
