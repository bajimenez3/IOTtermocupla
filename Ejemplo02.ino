
//#include "UbidotsMicroESP8266.h"
// Este archivo incluye una actualizacion de las librerias de Ubidots y observaras
// al conectarte a Ubidots que se crea otro dispostivo (Device) automaticamente
// tienes que configurar las etiquetas segun el nuevo dispositivo que se crea
// y adicionar la variable var_led  .... todo lo demás es identico 
#include "max6675.h"
#include "Ubidots.h"
#include "ESP8266WiFi.h"

#define DEVICE  "brs"  // Put here your Ubidots device label
#define VARIABLE  "led"  // Put here your Ubidots variable label
#define ID_TEMP "var_temp" 
#define TOKEN  "BBFF-YgSlxjIWI73A9otMHJGSKWlj1v6f23"  // Put here your Ubidots TOKEN

#define WIFISSID "JIMENEZ_CNT" // Put here your Wi-Fi SSID
#define PASSWORD "chavelaybonito2020" // Put here your Wi-Fi password
int thermoDO = 13;
int thermoCS = 12;
int thermoCLK = 14;
//int b = 1;
//float temp=0; //Se crea una variable flotante para almacenar la temperatura
//long t;
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
//int vccPin = 1;
int ledPin = 4;
const char* server = "184.106.153.149";
String apiKey ="2ZVVDYPSVCTQZPWI";
void enviarTempTS(float temp)
{  
   WiFiClient client;
  
   if (client.connect(server, 80)) { // usar 184.106.153.149 ó api.thingspeak.com
   Serial.println("WiFi Client connected ");
   
   String postStr = apiKey;
   postStr += "&field1=";
   postStr += String(temp);
   postStr += "\r\n\r\n";
   
   client.print("POST /update HTTP/1.1\n");
   client.print("Host: api.thingspeak.com\n");
   client.print("Connection: close\n");
   client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
   client.print("Content-Type: application/x-www-form-urlencoded\n");
   client.print("Content-Length: ");
   client.print(postStr.length());
   client.print("\n\n");
   client.print(postStr);
   delay(1000);
   
   }
 client.stop();
}

Ubidots ubidots(TOKEN);
float Valor_Temperatura = 0;

void setup() {
    Serial.begin(115200); 
    ubidots.wifiConnect(WIFISSID, PASSWORD);
   pinMode(ledPin, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 1);
    
}

void loop() 
{
    float Valor_Led = ubidots.get(DEVICE, VARIABLE);
    if (Valor_Led != ERROR_VALUE){
      Serial.print(F(">>>>>>>>> VALOR OBTENIDO POR EL LED: "));
      Serial.println(Valor_Led);
      if (Valor_Led==1){digitalWrite(ledPin, HIGH);}
      else{digitalWrite(ledPin, LOW);}
    }else{
      Serial.println(F("Error getting Valor_Led"));
    }

    //if (Valor_Temperatura>100)Valor_Temperatura=0;
  
    Valor_Temperatura = thermocouple.readCelsius() ;
     Serial.print(F(">>>>>>>>> VALOR TEMP: "));
      Serial.println(Valor_Temperatura);
      enviarTempTS(Valor_Temperatura);
    ubidots.add(ID_TEMP, Valor_Temperatura); 
    //ubidots.send();
     bool bufferSent = false;
  bufferSent = ubidots.send(DEVICE);  // Will send data to a device label that matches the device Id

  if (bufferSent) {
    // Do something if values were sent properly
    Serial.println("Values sent by the device");
  }
    delay(500);
}
