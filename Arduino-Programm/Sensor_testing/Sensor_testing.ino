<<<<<<< HEAD
<<<<<<< HEAD
const int piezoPin = 34;               

void setup() {
  pinMode(piezoPin, INPUT); 
  Serial.begin(115200); 
} 

void loop() {    
  Serial.println(analogRead(piezoPin)); 
  delay(200); 
=======
#include "BluetoothSerial.h"
=======
>>>>>>> aa8f16cfbc70348c8436f6142acd07806d39a6e7
#include "LedLib.h"
#include "BluetoothSerial.h"

#define serial_monitor 115200

BluetoothSerial SerialBT;

const long sensorPin = 35, ledPin = 22;            
unsigned long sensor_value = 0, ledstate = HIGH;
boolean bt_connected = false;

LedLib ll(ledPin);

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
  if(event == ESP_SPP_SRV_OPEN_EVT)
  {
    Serial.println("Client connected.");
    bt_connected = true;
    ll.led_on();
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

<<<<<<< HEAD
void loop() {   
  while(bt_connected != true) {
    ledlib.blink_once();
  }
>>>>>>> 88a934e6bec2ac2ade2e7e183ba74e72aa2dfaaa
=======

void loop() 
{ 
  if(!bt_connected)
  {
    ll.led_blink();
  } 
>>>>>>> aa8f16cfbc70348c8436f6142acd07806d39a6e7
} 
