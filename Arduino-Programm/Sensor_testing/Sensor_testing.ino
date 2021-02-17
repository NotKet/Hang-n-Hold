#include "BluetoothSerial.h"

#define serial_monitor 115200

BluetoothSerial SerialBT;

const long sensorPin = 35, ledPin = 22;            
unsigned long sensor_value = 0, ledstate = HIGH;
boolean bt_connected = false;

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
  if(event == ESP_SPP_SRV_OPEN_EVT)
  {
    Serial.println("Client connected.");
    bt_connected = true;
    digitalWrite(ledPin, HIGH);
  } 
  
  if(event == ESP_SPP_CLOSE_EVT) 
  {
    Serial.println("Client disconnected.");
    bt_connected = false;
  }
}


void setup() 
{
  Serial.begin(serial_monitor);
  SerialBT.register_callback(callback);

  pinMode(ledPin, OUTPUT);
  SerialBT.begin("Sensortesting");
} 


void loop() 
{ 
  if(!bt_connected)
  {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  } 
  Serial.println(digitalRead(sensorPin));
} 
