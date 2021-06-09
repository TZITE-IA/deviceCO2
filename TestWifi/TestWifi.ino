#include <ESP8266WiFi.h>
// Sustituir con datos de vuestra red
const char* ssid     = "Amoxtli";
const char* password = "1521I1524+";

void setup(){
  Serial.begin(115200);  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("\n\nConectando a:\t");
  Serial.println(ssid);
  // Esperar a que nos conectemos
  while (WiFi.status() != WL_CONNECTED){
    delay(200);
    Serial.print('.');
  }
  // Mostrar mensaje de exito y dirección IP asignada
  Serial.print("\nConectado a:\t");
  Serial.println(WiFi.SSID());
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
}
void loop(){
}
