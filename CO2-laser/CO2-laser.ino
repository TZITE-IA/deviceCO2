#include <Wire.h>

#include "SparkFun_SCD30_Arduino_Library.h"
SCD30 airSensor;

void setup(){
  Serial.begin(115200);
  Wire.begin();
  pinMode(D0, OUTPUT); pinMode(D3, OUTPUT);
  digitalWrite(D0, LOW); digitalWrite(D3, LOW);
  if (airSensor.begin() == false){
    Serial.println("Air sensor not detected. Please check wiring. Freezing...");
    while (1);
  }
}

void loop(){
  if (airSensor.dataAvailable()){
    
    int ppm = airSensor.getCO2();
    int temp = airSensor.getTemperature();
    int hum = airSensor.getHumidity();
    Serial.print("co2(ppm):");
    Serial.print(ppm);
    Serial.print(" temp(C):");
    Serial.print(airSensor.getTemperature(), 1);
    Serial.print(" humidity(%):");
    Serial.print(airSensor.getHumidity(), 1);
    if(ppm>1400){
      alarm();
    }
    Serial.println();
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
