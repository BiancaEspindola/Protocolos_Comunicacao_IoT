#include <ESP8266WiFi.h>
#include "coap_client.h"

//Cria instancia de cliente coap
coapClient coap;

//Parametros para conexao WiFi
const char* ssid = "Patrick01";
const char* password = "nicolefeia";

//IP e porta padrao do servidor CoAP. Esse IP deve ser obtido executando seu servidor CoAP e observado o endereco IP mostrado no monitor serial.
IPAddress ip(192,168,0,105);
int port = 5683;

//Cabecalho da resposta callback do cliente
void callback_response(coapPacket &packet, IPAddress ip, int port);

//Resposta callback
void callback_response(coapPacket &packet, IPAddress ip, int port){
    char p[packet.payloadlen + 1];
    memcpy(p, packet.payload, packet.payloadlen);
    p[packet.payloadlen] = NULL;

    //Resposta do servidor CoAP para teste de ping
    if(packet.type == 3 && packet.code == 0){
        Serial.println("ping ok");
    }

    Serial.println(p);
}

void setup(){
    Serial.begin(9600);

    WiFi.begin(ssid, password);
    
    //Conectando a Wifi
    Serial.println("");
    Serial.print("Conectando na rede ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        //yield();
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi conectado");
    
    //Mostra o IP do cliente no monitor serial
    Serial.println(WiFi.localIP());

    //Instancia a resposta do cliente
    coap.response(callback_response);

    //Inicia o cliente CoAP na porta 5683
    coap.start(5683);
}


//Variaveis para o teste de vazao
int start_time, end_time, n = 0;
char *payload = "LhHARKKv1ONY4qikdMzdCEldaqkFWhI6pm7bEfLDVROkABWSQPXcfK2RXBZfFv2j";
/*
A respeito do payload:
Aparentemente, o limite de bytes de payload do pacote CoAP e de 64 bytes. Estudar a respeito.
O payload que deveria ser utilizado pelo grupo esta na linha abaixo. Tamanho: 512 bytes.
LhHARKKv1ONY4qikdMzdCEldaqkFWhI6pm7bEfLDVROkABWSQPXcfK2RXBZfFv2jowtFWU4Ddj2R2foXbzSA852XpENN9BT5qfOBCr2MlvoKbWmmNMrxQ4doMo39yOezwClkmc73inJgpWRm3heEJ37uZ4cHMq9gGX6PDnWLMN6car32jpEAuJYowaktIFgCqVplRH1Lt8P31FQKii12hTjVREIURHFpJCdo5h9H1IDpjYksuedUZGNLBAiCmWRHo4ioOXGi851DwTqyfkK18bIxjBZb7GlESNeOjZdlCXqOLSud234lrJ8G5Y28EQihTWCYRSXP1eOUdnIR42xdZD6lUxBYP2vdhsnS2I8dOH1W752rtRP8FlPy07XobAcRJl6KuJPYLwWJoen2ACFQHozuGhAsRIIc7NYoxOTCACRUsqM0tpCqH8irJjwiP52qQXbpxWjxG5vJSLxUlvYKrGgXJPyTo9x0768Ht4GvaGycmBSlgxYijjy1pMuVfVFa
*/

void loop() {
    /*
    Requisicoes:

    GET
    coap.get(ip, port, "light");
    Parametros: ip do servidor, porta padrao do servidor, nome do recurso

    PUT
    coap.put(ip, port, "resourcename", "0", strlen("0"));
    Parametros: ip do servidor, porta padrao do servidor, nome do recurso, payload, payloadlength

    POST
    int msgid = coap.post(ip, port, "resourcename", "0", strlen("0"));
        
    DELETE
    int msgid = coap.delet(ip, port, "resourcename");

    PING
    int msgid = coap.ping(ip, port);

    OBSERVACAO
    coap.observe(ip, port, "obs", 0);
    */
    
    coap.loop();

    //Teste de vazao. n é o numero de mensagens enviadas.
    Serial.println("Iniciando teste de vazao");
    start_time = millis();
    for (n = 0; n < 1000; n++){
        //Serial.println("Mensagem numero: " + String(n));
        coap.post(ip, port, "light", payload, strlen(payload));
        delay(0);
    }
    end_time = millis();

    Serial.println(String(n) + " mensagens foram enviadas em " + String(end_time - start_time) + " milissegundos");
    Serial.println("");

    delay(30000);
}