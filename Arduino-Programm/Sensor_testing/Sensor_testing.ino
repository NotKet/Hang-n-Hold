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
#include "EEPROM.h"

#define EEPROM_SIZE 1
#define sm_value 115200

BluetoothSerial SerialBT;

const long sensorPin = 35, ledPin = 22, sensor1_value = 0, sensor2_value = 0;              
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
    EEPROM.write(0, );
    EEPROM.commit();
  } 
  
  if(event == ESP_SPP_CLOSE_EVT) 
  {
    Serial.println("Client disconnected.");
    bt_connected = false;
  }
}


void setup() 
{
  Serial.begin(sm_value);
  SerialBT.register_callback(callback);

  pinMode(ledPin, OUTPUT);
  EEPROM.begin(EEPROM_SIZE)
  
  if(!SerialBT.begin("Sensortest")) {
    Serial.println("An error ocurred initializing Bluetooth");
  } else {
    Serial.println("Bluetooth initialized");
  }

  ledstate = EEPROM.read(0)
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
