#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

const long sensorPin = 25;              
unsigned long sensor_value = 0;

void setup() {
  pinMode(sensorPin, INPUT);
  Serial.begin(115200);
  SerialBT.begin("Sensortest");
} 

void loop() {   
  sensor_value = analogRead(sensorPin);
  SerialBT.println(sensor_value);
  delay(1000);
} 
