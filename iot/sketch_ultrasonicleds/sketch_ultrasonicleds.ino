#include <Ultrasonic.h> 

#define INTERVALO_LEITURA 250 // (ms)

unsigned int distancia = 0;

// Define pinos de TRIGGER E ECHO do Sensor
#define PIN_TRIGGER 4
#define PIN_ECHO 5

// Inicializa Sensor
Ultrasonic ultrasonic(PIN_TRIGGER, PIN_ECHO);

// Pinos
#define PIN_GREEN_LED 17
#define PIN_YELLOW_LED 18
#define PIN_RED_LED 19
#define PIN_BUZZER 21

void setup() {
  // Define velocidade de transmissão
  Serial.begin(115200);

  // Configurar Saidas (Leds e Buzzer)
  configurarSaidas();

}

void loop() {

  verificarDistancia();
  delay(INTERVALO_LEITURA);
  
}

void configurarSaidas() {
  pinMode(PIN_GREEN_LED, OUTPUT);
  pinMode(PIN_YELLOW_LED, OUTPUT);
  pinMode(PIN_RED_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
}

void verificarDistancia() {

  // retorna a distância do sensor ultrasonico
  distancia = ultrasonic.read();

  for (int i = PIN_GREEN_LED; i <= PIN_RED_LED; i++) {
    digitalWrite(i, LOW);
  }
  
  digitalWrite(PIN_BUZZER, LOW);
  
  if ( distancia <= 5 ) {
    digitalWrite(PIN_RED_LED, HIGH);
    digitalWrite(PIN_BUZZER, HIGH);
  }
  else if (distancia <= 20) {
    digitalWrite(PIN_YELLOW_LED, HIGH);
  }
  else if (distancia > 20) {
    digitalWrite(PIN_GREEN_LED, HIGH);
  }
  
}
