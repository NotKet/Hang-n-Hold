#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

const long piezoPin = 34;
unsigned long value = 0, previousMillis = 0, piezoValue = 0; 
const long interval = 500;

void setup() {  
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); 
  Serial.println("The device started, now you can pair it with bluetooth!");
} 

void loop() { 
  unsigned long currentMillis = millis();
  piezoValue = analogRead(piezoPin);
  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    SerialBT.println(piezoValue); 
    Serial.println(piezoValue);
  }
} 
 
