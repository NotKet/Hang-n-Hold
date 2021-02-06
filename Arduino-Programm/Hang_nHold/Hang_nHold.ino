#include "BluetoothSerial.h"
#include "LedLib.h"
#include "TimeLib.h"

#define min_sensor_value 2000
#define fw_version 1.00
#define min_start_value 1000

BluetoothSerial SerialBT;

const long sensorPins[] = {34, 35}, ledPin = 22;               
unsigned long sensor1_values[100], sensor2_values[100], sensor1_start_value = 0, sensor2_start_value = 0;
boolean bt_connected = false, measuring = false;

LedLib ledlib(ledPin);

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if(event == ESP_SPP_SRV_OPEN_EVT){
    Serial.println("Client connected.");
    bt_connected = true;
    ledlib.led_on();
    start_msg();
  } 
  if(event == ESP_SPP_CLOSE_EVT) {
    Serial.println("Client disconnected.");
    bt_connected = false;
  }
}

void measurement() 
{
//  int count = 0;
//  measuring = true;
//  SerialBT.println("Messung wurde gestartet. Bitte hänge dich nun an die Griffe....");
//  
//  while(measuring) 
//  {
//    sensor1_start_value = analogRead(sensorPins[0]);
//    sensor2_start_value = analogRead(sensorPins[1]);
//  
//    if(sensor1_start_value >= min_start_value && sensor2_start_value >= min_start_value) 
//    {
//      while(1) 
//      {
//        sensor1_values[count] = analogRead(sensorPins[0]);
//        sensor2_values[count] = analogRead(sensorPins[1]);
//        
//        if(sensor1_values[count] <= min_start_value && sensor2_values[count] <= min_start_value) 
//        {
//          break;
//          measuring = false;
//        }
//  
//        count += 1;
//      }  
//    }
//  }

  SerialBT.println("Starting measurement");
}

void mc_battery_status() 
{
  
}

void firmware_version() 
{
  SerialBT.println(fw_version);
}

void current_date() 
{
  SerialBT.println("24.1.2021");
}

void calibration() 
{
  
} 

void setup() 
{
  Serial.begin(115200);
  SerialBT.register_callback(callback);
  SerialBT.begin("Hang'n Hold - Modul 1");
}

void start_msg() {
  SerialBT.println("'v' - Version, 'd' - Date, 'c' - Calibration, 'm' - Measurement, 'b' - Battery status.'");
}

void loop() 
{
  while(!bt_connected) 
  {
    ledlib.led_blink();
  }

  
  if(SerialBT.available()) 
  {
    switch(SerialBT.read()) 
    {
      case 'v':
        firmware_version();
        break;
  
      case 'd':
        current_date();
        break;
  
      case 'c':
        calibration();
        break;
  
      case 'm':
        measurement();
  
      case 'b':
        mc_battery_status();
    }
    msg();
  }
}
