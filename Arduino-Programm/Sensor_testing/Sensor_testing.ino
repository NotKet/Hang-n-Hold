#include "BluetoothSerial.h"
#include "LedLib.h"

BluetoothSerial SerialBT;

const int sensorPin = 25, ledPin = 22;               
unsigned long sensor_value = 0;

boolean bt_connected = false;


LedLib ledlib(ledPin);

void setup() {
  Serial.begin(115200);
  SerialBT.register_callback(callback)
  if(!SerialBT.begin("ESP32")){
    Serial.println("An error occurred initializing Bluetooth");
  }else{
    Serial.println("Bluetooth initialized");
  }
} 
  
void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if(event == ESP_SPP_SRV_OPEN_EVT){
    Serial.println("Client connected.");
    bt_connected = true;
    ledlib.led_on();
  } 
  if (event == ESP_SPP_CLOSE_EVT) {
    Serial.println("Client disconnected.");
    bt_connected = false;
  }
} 

void loop() {   
  while(bt_connected == false) {
    ledlib.blink_once();
  }
   
  sensor_value = analogRead(sensorPin); 
  SerialBT.println(sensor_value);
  delay(1000);
} 
