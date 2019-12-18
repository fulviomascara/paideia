// Importa a biblioteca de Wifi
#include <WiFi.h>

// Importa a biblioteca DHT (Sensor de Temperatura e Umidade)
#include "DHT.h"

// Importa a biblioteca de MQTT
#include <PubSubClient.h>

// Numero do Pino Digital conectado no Sensor
#define DHTPIN 4     

// Define o tipo de sensor DHT (DHT11, DHT22)
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Define configurações para Wifi
const char* ssid = "LassuVisitante";
const char* password =  "paideia@lassu";

// Define configurações para MQTT
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* idmqtt = "esp32_mqtt";

// Define Tópicos para Publicação de Temperatura e Umidade
#define TOPICO_PUBLISH_TEMPERATURA  "paideia/grupo0/temperatura"
#define TOPICO_PUBLISH_UMIDADE      "paideia/grupo0/umidade"

// Inicializa o Sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Criar um Cliente Wifi
WiFiClient espClient;

// Criar um Cliente MQTT
PubSubClient MQTT(espClient);

// Metodos
void initWiFi(void);
void initMQTT(void);
void reconnectMQTT(void);
void reconnectWiFi(void);
void VerificaConexoesWiFIEMQTT(void);

// O método setup roda somente uma vez, ao iniciar/reiniciar a placa
void setup() {
  // Define velocidade
  Serial.begin(115200);

  // Imprime mensagem informando que vai iniciar o sensor DHT
  Serial.println("Teste do DHT22");

  // Inicializa o Sensor DHT
  dht.begin();

  // Inicializa a conexão WiFi
  initWiFi();

  /* Inicializa a conexao ao broker MQTT */
  initMQTT();
  
}

// O metodo loop executa de forma repetida
void loop() {
  // Aguarda 2 segundos antes da próxima medição
  delay(2000);

  // Verifica conexão WiFi e MQTT
  VerificaConexoesWiFIEMQTT();

  // Lê umidade (%)
  float umidade = dht.readHumidity();
  // Lê temperatura em Celsius (C)
  float temperatura = dht.readTemperature();

  // Verifica se algo foi lido e se der falha, sai e tenta novamente
  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Falha ao ler o sensor DHT!");
    return;
  }

  // Se conseguir ler dados, imprime na saída Serial
  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.print("%  Temperatura: ");
  Serial.print(temperatura);
  Serial.println("°C ");

  char temperatura_str[10] = {0};
  char umidade_str[10]     = {0};
     
  /* garante funcionamento das conexões WiFi e ao broker MQTT */
  VerificaConexoesWiFIEMQTT();
 
  /* Compoe as strings a serem enviadas pro dashboard (campos texto) */
  sprintf(temperatura_str,"%.2fC", temperatura);
  sprintf(umidade_str,"%.2f", umidade);

  // Publica temperatura no topico MQTT
  MQTT.publish(TOPICO_PUBLISH_TEMPERATURA, temperatura_str);
  MQTT.publish(TOPICO_PUBLISH_UMIDADE, umidade_str);
   
  /* keep-alive da comunicação com broker MQTT */
  MQTT.loop();
  
}

void initWiFi(void) 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(ssid);
    Serial.println("Aguarde");
      
    reconnectWiFi();
}

void reconnectWiFi(void) 
{
    // Se já está conectado a rede WI-FI, nada é feito. 
    // Caso contrário, são efetuadas tentativas de conexão
    if (WiFi.status() == WL_CONNECTED)
        return;
          
    WiFi.begin(ssid, password); // Conecta na rede WI-FI
      
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(ssid);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}

void initMQTT(void) 
{
    //informa qual broker e porta deve ser conectado
    MQTT.setServer(mqttServer, mqttPort);   
}

void reconnectMQTT(void) 
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(mqttServer);
        if (MQTT.connect(idmqtt)) 
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
        } 
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}

void VerificaConexoesWiFIEMQTT(void)
{
    if (!MQTT.connected()) 
        reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
      
     reconnectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}
