// Importa a biblioteca DHT (Sensor de Temperatura e Umidade)
#include "DHT.h"

// Numero do Pino Digital conectado no Sensor
#define DHTPIN 4     

// Define o tipo de sensor DHT (DHT11, DHT22)
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Inicializa o Sensor
DHT dht(DHTPIN, DHTTYPE);

// O método setup roda somente uma vez, ao iniciar/reiniciar a placa
void setup() {
  // Define velocidade
  Serial.begin(115200);

  // Imprime mensagem informando que vai iniciar o sensor DHT
  Serial.println("Teste do DHT22");

  // Inicializa o Sensor DHT
  dht.begin();
}

// O metodo loop executa de forma repetida
void loop() {
  // Aguarda 2 segundos antes da próxima medição
  delay(2000);

  // Lê umidade (%)
  float h = dht.readHumidity();
  // Lê temperatura em Celsius (C)
  float t = dht.readTemperature();

  // Verifica se algo foi lido e se der falha, sai e tenta novamente
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Falha ao ler o sensor DHT!"));
    return;
  }

  // Se conseguir ler dados, imprime na saída Serial
  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.print("%  Temperatura: ");
  Serial.print(t);
  Serial.println("°C ");
}
