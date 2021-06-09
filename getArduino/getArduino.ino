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
  HTTPClient http;
  http.begin(host);

  // Send HTTP GET request
  int httpResponseCode = http.GET();
  delay(2000);
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}
