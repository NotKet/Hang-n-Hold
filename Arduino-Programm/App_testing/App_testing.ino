#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

const int pin = 34;

unsigned long num = 0;
unsigned long sensorValue = 0;


void setup() {
  pinMode(pin, INPUT);
  SerialBT.begin("ESP32");
}

void loop() {
  if(SerialBT.connected()){
    sensorValue = analogRead(pin);
    SerialBT.println(sensorValue);
  }
  delay(1000);
}
