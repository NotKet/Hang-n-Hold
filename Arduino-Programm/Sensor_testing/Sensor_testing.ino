#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

const int sensorPin = 25, ledPin = 22;               
unsigned long sensor_value = 0;

void setup() {
  pinMode(ledPin, INPUT); 
  Serial.begin(115200);
  SerialBT.begin("Hang'n Hold - 1"); 
} 

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  while(true) {
    if(event == ESP_SPP_SRV_OPEN_EVT){
    Serial.println("Client Connected");
    break;
    }
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
  }
} 

void loop() {    
  sensor_value = analogRead(sensorPin); 
  SerialBT.println(sensor_value);
  delay(1000);
} 
