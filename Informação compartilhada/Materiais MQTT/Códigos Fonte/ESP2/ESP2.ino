/*

*/
///Define inicial para expandir os limites de pacote da bibilioteca
///padrao apenas 128 bytes
#define MQTT_MAX_PACKET_SIZE 1024


#include <ESP8266WiFi.h>
///biblioteca que implementa MQTT
#include <PubSubClient.h>

// redes wifi

//const char* ssid = "rede esp";
//const char* password = "LarmUfsc2019";
//const char* mqtt_server = "192.168.0.100";

const char* ssid = "Millenium Falcon";
const char* password = "buraconegro";
const char* mqtt_server = "192.168.1.105";


//cria uma classe de cliente wifi
WiFiClient espClient;
//cria uma classe de client pubsub mqtt
PubSubClient client(espClient);
//tempo da ultima mensagem
long lastMsg = 0;
//contador de mensagem
long msgcount = 0;
//valor now comunmente recebido por millis()
long now;

long tempo_total = 0;
long total = 0;
//funcao de configuracao wifi
void setup_wifi() {

  delay(10);
  // comecar conectando ao wifi
  Serial.println();
  Serial.print("Conectando a rede");
  Serial.println(ssid);
  //conecta-se a uma rede utilizando uma snha
  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereco IP: ");
  //conexao bem sucedida printa sobre a rede
  Serial.println(WiFi.localIP());
}


//como este eh o segundo ESP eh interessante 
//tratar no callback as mensagens para ele

void callback(char* topic, byte* payload, unsigned int length) {
  ///contabiliza mensagens chegando
    msgcount++;  


  

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Estabelecendo conexao MQTT..\n");
    // Cria um client ID random
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Tenta conectar-se com o cliente
    if (client.connect(clientId.c_str())) {
      ///se bem sucedido printa na serial
      ///e publica no topico de conexao
      Serial.println("conectado!");
      client.publish("conexao", "ESP2 Conectado!");
      client.subscribe("ESP2");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("reconectando de novo em 5 segundos\n");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  ////sempre tenta manter-se conectado
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  //recebe do millis o tempo de agora
  now = millis();
  ////se o tempo de agora passou de 250ms da ultima vez
  //hora de printar as mensagens recebidas
  if (now - lastMsg > 250) {
    ///total de mensagens
    total = (total + msgcount);
    ///se recebeu pelo menos uma mensagem, printar
    ///se nao apenas zerar o timer geral e timer de mensagem
    ///caso esteja recebendo fluxo de mensagens, contabilizar tempo para receber
    if(msgcount > 0){
    ///sequencia de mensagens recebidas, contabilizar o tempo decorrido para recebe-las
    ///em tempos de millis pequenos, a sequencia pode ser quebrada
    ///ocorrendo a quebra da contagem de tempo
    ///caso o ESP esteja em pequenos periodos de parada
    tempo_total =  (tempo_total + (now -lastMsg));
    ///comeca a contar de zero por isto o -250
    printf("Msg [%d] Total [%d] : T %dms\n\r",msgcount,total,tempo_total);
    }else
    {
    ///nao houve mensagens entao nao ha o que contabilizar tempo para receber
    tempo_total = 0;
    }
    ///ja foram contabilizadas, zerar o contador
    msgcount = 0;
    ///zerar o timer
    lastMsg = now;
  }
}
