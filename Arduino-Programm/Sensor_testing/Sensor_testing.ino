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
=======
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
>>>>>>> 6cc10fa5ee6bf02b00334e6e403820272d44edd1
  } 
  if(event == ESP_SPP_CLOSE_EVT) {
    Serial.println("Client disconnected.");
    bt_connected = false;
  }
} 

<<<<<<< HEAD
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
>>>>>>> 88a934e6bec2ac2ade2e7e183ba74e72aa2dfaaa
=======
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
>>>>>>> 6cc10fa5ee6bf02b00334e6e403820272d44edd1
} 
