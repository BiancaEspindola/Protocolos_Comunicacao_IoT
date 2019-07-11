/*



*/
///Define inicial para expandir os limites de pacote da bibilioteca
///padrao apenas 128 bytes
#define MQTT_MAX_PACKET_SIZE 1024

#include <ESP8266WiFi.h> 
///library de implementacao MQTT
#include <PubSubClient.h>

// Update these with values suitable for your network.

//const char* ssid = "rede esp";
//const char* password = "LarmUfsc2019";
//const char* mqtt_server = "192.168.0.100";

//Configuracao de rede wifi

const char* ssid = "Millenium Falcon";
const char* password = "buraconegro";
const char* mqtt_server = "192.168.1.105";

//mqtt_server = endereco do broker

const uint8_t payload[] = "LhHARKKv1ONY4qikdMzdCEldaqkFWhI6pm7bEfLDVROkABWSQPXcfK2RXBZfFv2jowtFWU4Ddj2R2foXbzSA852XpENN9BT5qfOBCr2MlvoKbWmmNMrxQ4doMo39yOezwClkmc73inJgpWRm3heEJ37uZ4cHMq9gGX6PDnWLMN6car32jpEAuJYowaktIFgCqVplRH1Lt8P31FQKii12hTjVREIURHFpJCdo5h9H1IDpjYksuedUZGNLBAiCmWRHo4ioOXGi851DwTqyfkK18bIxjBZb7GlESNeOjZdlCXqOLSud234lrJ8G5Y28EQihTWCYRSXP1eOUdnIR42xdZD6lUxBYP2vdhsnS2I8dOH1W752rtRP8FlPy07XobAcRJl6KuJPYLwWJoen2ACFQHozuGhAsRIIc7NYoxOTCACRUsqM0tpCqH8irJjwiP52qQXbpxWjxG5vJSLxUlvYKrGgXJPyTo9x0768Ht4GvaGycmBSlgxYijjy1pMuVfVFa";

///cria objeto de cliente wifi
WiFiClient espClient;
///cria objeto mqtt de cliente que recebe objeto wifi 
PubSubClient client(espClient);
///o tempo de agora queo  millis() vai devolver
long now;
///estado atual eh de envio
bool sending = 1;
///iniciar configuração do wif
void setup_wifi() {

  delay(10);
  // conecta na rede wifi
  Serial.println();
  Serial.print("Conectando a rede ");
  Serial.println(ssid);
  //inicia o wifi na rede com dada senha
  WiFi.begin(ssid,password);
  ///enquanto nao conectar continua printando pontinhos
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  ///iniciando semente do random
  ///com numero recebido do micros
  randomSeed(micros());
  //sucesso na conexao
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  ///apos conexao com sucesso printar a rede em que se conectou
  Serial.println(WiFi.localIP());
}


///Cria uma interrupcao para cada vez que um pacote em certo topico eh recebido
///No caso do primeiro ESP que apenas envia, nao ha tratamento dela
///os pacotes mqtt que sao recebidos nao sao importantes
void callback(char* topic, byte* payload, unsigned int length) {



}

//caso caia a conexao, tentar se reconectar
void reconnect() {
  // tenta indefinitivamente conectar-se
  while (!client.connected()) {
    Serial.print("Estabelecendo conexao MQTT..\n");
    // Cria um nome de cliente aleatorio
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Tenta conectar com retained = True
    if (client.connect(clientId.c_str()) ) {
      Serial.println("conectado!");
      // assim que conectar, publicar um topico
      client.publish("conexao", "ESP1 Conectado!");
      // nao eh necessario de inscrever num topico
      //client.subscribe("ESP1");
    } else {
      ///falhou a conexao...
      Serial.print("falha...");
      Serial.print(client.state());
      Serial.println("reconectando de novo em 5 segundos\n");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  ///setup para incializar leds e serial
  ///e chamar o setup do wifi
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  ///conectar com o broker mqtt especificado
  client.setServer(mqtt_server, 1883);
  ///configurar o callback
  client.setCallback(callback);
}

void loop() {

  //tenta sempre se manter conectad sempre
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  ///se ele se encontra no estado de sending
  ///enviar

  ///Envia mensagem com identificador true para "retainment"
  ///broker vai salvar as mensagens que ele recebe do dispositiv
  ///enviar mensagens a todo momento,
  ///porem printando a cada segundo para demonstrar
  printf("Iniciando 1000 envios de 512 bytes\n");
  now = millis();
  for(int i = 0; i < 1000; i++)
  {
     client.publish("ESP2", payload, 512, true);
  }
  printf("Tempo decorrido %dms \n",(millis() - now));
  printf("Desconectando do broker \n");
  ///apos 1 minuto o dispositivo para de enviar
  client.publish("conexao", "ESP1 Conectado!");
  client.disconnect(); 
  delay(30000);

}
