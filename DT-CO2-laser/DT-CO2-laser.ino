#include <ESP8266WiFi.h>              //Incluye la librería ESP8266WiFi

const char* ssid = "Amoxtli";         //Indicamos el nombre de la red WiFi (SSID) a la que queremos conectarnos.
const char* password = "1521I1524+";  //Indicamos la contraseña de de red WiFi

const char* host = "dtco2.herokuapp.com";  //Declaramos el servidor de conexión

#include <Wire.h>
#include "SparkFun_SCD30_Arduino_Library.h"
SCD30 airSensor;
int ppm = 0; int temp = 0; int hum = 0;
String locate = "lateral"; //lateral, central
void setup(){
  Serial.begin(115200);
  Wire.begin();
  pinMode(D0, OUTPUT); pinMode(D3, OUTPUT);
  digitalWrite(D0, LOW); digitalWrite(D3, LOW);
  connect(); 
  if (airSensor.begin() == false){
    Serial.println("Air sensor not detected. Please check wiring. Freezing...");
    while (1);
  }
}

void loop(){
  WiFiClient client;                               //Inicializamos el cliente (client)
  Serial.printf("\n[Conectando a %s ... ", host);  //Establecemos la conexión con el servidor
  if (client.connect(host, 80)){
    Serial.println("conectado]");
    Serial.println("[Enviando peticion]");         //Enviamos la petición de datos
    readSensor();
    String PostData = "{\"name\": \"Sensor\",\"site\": \""+ locate +"\", \"series\": {\"value\": "+String(ppm) +",\"temp\": "+String(temp)+",\"hum\": "+String(hum) +",\"name\":\"mock\"}}";
    Serial.println(PostData);
    client.println("PUT /update HTTP/1.1");
    client.println("Host: dtco2.herokuapp.com");
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/json;");
    client.print("Content-Length: ");
    client.println(PostData.length());
    client.println();
    client.println(PostData);
    Serial.println("[Respuesta:]");  //Leemos la respuesta del servidor
    while (client.connected()){
      if (client.available()){
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }
    client.stop();                   //Finalizamos la conexión con el servidor
    Serial.println("\n[Desconectado]");
  }
  else{
    Serial.println("conexion con el servidor no lograda!]");
    client.stop();
    connect();
  }
  if (ppm >= 1450){
    alarm();
    delay(50);
  }else{
    delay(700);
  }
}
  
void connect() {
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
}

void readSensor(){
  if (airSensor.dataAvailable()){
    ppm = airSensor.getCO2();
    temp = airSensor.getTemperature();
    hum = airSensor.getHumidity();
    Serial.print("co2(ppm):");
    Serial.print(ppm);
    Serial.print(" temp(C):");
    Serial.print(temp);
    Serial.print(" humidity(%):");
    Serial.print(hum);
  }
}

void alarm(){
  digitalWrite(D0, HIGH); digitalWrite(D3, HIGH);
  delay(300);
  digitalWrite(D0, LOW); digitalWrite(D3, LOW);
  delay(150);
  digitalWrite(D0, HIGH); digitalWrite(D3, HIGH);
  delay(300);
  digitalWrite(D0, LOW); digitalWrite(D3, LOW);
}
