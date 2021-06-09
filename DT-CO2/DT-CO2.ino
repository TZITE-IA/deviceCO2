#include "CO2Sensor.h"
#include <ESP8266WiFi.h>              //Incluye la librería ESP8266WiFi

const char* ssid = "Amoxtli";     //Indicamos el nombre de la red WiFi (SSID) a la que queremos conectarnos.
const char* password = "1521I1524+"; //Indicamos la contraseña de de red WiFi

const char* host = "dtco2.herokuapp.com";  //Declaramos el servidor de conexión

CO2Sensor co2Sensor(A0, 0.99, 100);
String locate="Afuera";
int i = 1;
int val = 0;
void setup() {
  co2Sensor.calibrate();
  Serial.begin(115200);               //Inicializamos el Puerto Serie
  Serial.println();//Inicializamos la conexión Wi-Fi en modo Station
  connect();
  pinMode(D0, OUTPUT); pinMode(D1, OUTPUT);
  digitalWrite(D0, LOW); digitalWrite(D1, LOW);
}

void loop() {
  WiFiClient client; //Inicializamos el cliente (client)
 /* 
  if(i<=1){ 
    locate="Adentro"; 
  }else{ 
    locate="Afuera"; 
    i=0;
  }
  i++;*/
  readSensor();
  
  Serial.printf("\n[Conectando a %s ... ", host);  //Establecemos la conexión con el servidor
  if (client.connect(host, 80)){
    Serial.println("conectado]");
    Serial.println("[Enviando peticion]");         //Enviamos la petición de datos
    String PostData = "{\"name\": \"sensor\",\"site\": \""+ locate +"\", \"series\": {\"value\": " + String(val) + ",\"name\":\"mock\"}}";
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
  if (val >= 750){
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
  val = co2Sensor.read();
  if (val >= 500 || val <= 400) {
    co2Sensor.calibrate();
    delay(100);
    int val = co2Sensor.read();
  }
  if (val >= 750){
    alarm();
  }
}

void alarm(){
  for(int a=0; a<=3; a++){
    if(a==1 || a==3){
      digitalWrite(D0, LOW);
      digitalWrite(D1, LOW);
    }else{
      digitalWrite(D0, HIGH);
      digitalWrite(D1, HIGH);
    }
    delay(200);
  }
}
