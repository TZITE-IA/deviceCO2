#include "CO2Sensor.h"

//CO2Sensor co2Sensor(A0, 0.99, 100);

void setup() {
  Serial.begin(9600);
  Serial.println("=== Initialized ===");
  //co2Sensor.calibrate();
  pinMode(A0, INPUT);
}

void loop() {
  int val = analogRead(A0);
  Serial.print("CO2 value: ");
  Serial.println(val);

  delay(500);
}
