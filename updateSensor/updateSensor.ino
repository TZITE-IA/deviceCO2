#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "CO2Sensor.h"

// Sustituir con datos de vuestra red
const char* ssid     = "Amoxtli";
const char* password = "1521I1524+";
const char* host = "https://dtco2.herokuapp.com";
String url = "/update";
int i = 0;

CO2Sensor co2Sensor(A0, 0.99, 100);

void setup() {
  co2Sensor.calibrate();
  Serial.begin(115200);  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("\n\nConectando a:\t");
  Serial.println(ssid);
  // Esperar a que nos conectemos
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print('.');
  }
  // Mostrar mensaje de exito y dirección IP asignada
  Serial.print("\nConectado a:\t");
  Serial.println(WiFi.SSID());
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
}


void loop() {
  WiFiClient client;
  if (!client.connect(host, 80)) {
    Serial.println("connection failed");
    return;
  }

  
  int val = co2Sensor.read();
  String postData = "{\"name\": \"s2\", \"series\": { \"value\": " + String(val) + ", \"name\": \"" + String(i) + "\" }}";
  String address = host + url;
  HTTPClient http;

  Serial.print("Requesting POST: " + address + "  " + postData);
  http.begin(address.c_str());
  http.addHeader("Content-Type", "application/json");
  
  auto httpCode = http.POST(postData);
  delay(100);
  if (httpCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpCode);
      }
  Serial.println("closing connection");
  http.end(); 
  i++;
  delay(1500);
}
