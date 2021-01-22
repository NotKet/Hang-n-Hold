#include "BluetoothSerial.h"
#include "LedLib.h"

BluetoothSerial SerialBT;

const long sensorPin = 35, ledPin = 22;              
unsigned long sensor_value = 0;
boolean bt_connected = false;

LedLib ledlib(ledPin);

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
  if(event == ESP_SPP_SRV_OPEN_EVT)
  {
    Serial.println("Client connected.");
    bt_connected = true;
    ledlib.led_on();
  } 
  if(event == ESP_SPP_CLOSE_EVT) 
  {
    Serial.println("Client disconnected.");
    bt_connected = false;
  }
}

void setup() 
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  SerialBT.register_callback(callback);
  SerialBT.begin("Sensortest");
} 

void loop() 
{ 
  while(!bt_connected)
  {
    ledlib.blink_once();
  }
    
  sensor_value = analogRead(sensorPin);
  SerialBT.println(sensor_value);
  delay(1000);
} 
