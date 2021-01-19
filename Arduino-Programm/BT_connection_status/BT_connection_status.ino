#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if(event == ESP_SPP_SRV_OPEN_EVT){
    Serial.println("Client connected.");
  } 
  if(event == ESP_SPP_CLOSE_EVT) {
    Serial.println("Client disconnected.");
  }
}

void setup() {
  Serial.begin(115200);
  SerialBT.register_callback(callback);
  SerialBT.begin("BT_status");
}

void loop() {

}
