#include "BluetoothSerial.h"
#include "LedLib.h"

BluetoothSerial SerialBT;

const int sensorPin = 25, ledPin = 22;               
unsigned long sensor_value = 0;

boolean bt_connected = false;

LedLib ledlib(ledPin);

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if(event == ESP_SPP_SRV_OPEN_EVT){
    Serial.println("Client connected.");
    bt_connected = true;
    ledlib.led_on();
  } 
  if(event == ESP_SPP_CLOSE_EVT) {
    Serial.println("Client disconnected.");
    bt_connected = false;
  }
} 

void battery_status() {
  
}

void arduino_version() {
  
}

void date() {
  
}

void calibration() {
  
} 

void setup() {
  Serial.begin(115200);
  SerialBT.register_callback(callback);
  if(!SerialBT.begin("Hang'n Hold 1")){
    Serial.println("An error occurred initializing Bluetooth");
  }else{
    Serial.println("Bluetooth initialized");
  }
} 

void loop() {   
  while(bt_connected != true) {
    ledlib.blink_once();
  }
} 

void setup() {
  
}

void loop() {
  
}
