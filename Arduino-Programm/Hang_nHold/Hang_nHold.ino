#include "Pangodream_18650_CL.h"
#include "BluetoothSerial.h"
#include "LedLib.h"

#define min_sensor_value 2000
#define fw_version 1.00
#define min_start_value 1000

BluetoothSerial SerialBT;

const long sensorPins[] = {34, 35}, ledPin = 22;               

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
  int i = 0;
  measuring = true;
  unsigned long sensor1_values[100], sensor2_values[100];
  unsigned long sensor1_start_value = 0, sensor2_start_value = 0, sensor1_sum = 0, sensor2_sum = 0, sensor_sum = 0,average = 0, percentage_module1 = 0, percentage_module2 = 0;
  
  SerialBT.println("Messung wurde gestartet.");
  SerialBT.println("Bitte hänge dich nun an die Griffe...");
  
  while(measuring) 
  {
    sensor1_start_value = analogRead(sensorPins[0]);
    sensor2_start_value = analogRead(sensorPins[1]);
  
    if(sensor1_start_value >= min_start_value && sensor2_start_value >= min_start_value) 
    {
      while(1) 
      {
        sensor1_values[i] = analogRead(sensorPins[0]);
        sensor2_values[i] = analogRead(sensorPins[1]);
        
        if(sensor1_values[i] <= min_start_value && sensor2_values[i] <= min_start_value) 
        {
          break;
          measuring = false;
        }
  
        i += 1;
        delay(1000);
      }  
    }
    delay(1000);
  }
  
  for(int y=0; y<=i; y+=1)
  {
    sensor1_sum += sensor1_values[y];
    sensor2_sum += sensor2_values[y];
  }
  
  sensor_sum = sensor1_sum + sensor2_sum;

  average = sensor_sum / i;
  percentage_module1 = (sensor1_sum / sensor_sum) * 100;
  percentage_module2 = (sensor2_sum / sensor_sum) * 100;
  
  SerialBT.print("Durchschnitt: ");
  SerialBT.println(average);
  SerialBT.print("Prozentsatz Modul 1: ");
  SerialBT.println(percentage_module1);
  SerialBT.print("Prozentsatz Modul 2: ");
  SerialBT.println(percentage_module2);
  
}

void mc_battery_status() 
{
  Serial.print("Value from pin: ");
  Serial.println(analogRead(33s));
  Serial.print("Average value from pin: ");
  Serial.println(BL.pinRead());
  Serial.print("Volts: ");
  Serial.println(BL.getBatteryVolts());
  Serial.print("Charge level: ");
  Serial.println(BL.getBatteryChargeLevel());
  Serial.println("");
}

void firmware_version() 
{
  SerialBT.println(fw_version);
}

void calibration() 
{
  measuring = true;
  unsigned long sensor1_values[100], sensor2_values[100];
  unsigned long sensor1_start_value = 0, sensor2_start_value = 0, sensor1_sum = 0, sensor2_sum = 0;
  
  SerialBT.println("Kalibrierung wurde gestartet.");
  SerialBT.println("Es werden nun 10 Kalibrierungs-messungen durchführt.");
  SerialBT.println("Bitte hänge dich nun an die Griffe...");
  
  while(measuring) 
  {
    sensor1_start_value = analogRead(sensorPins[0]);
    sensor2_start_value = analogRead(sensorPins[1]);
    
    if(sensor1_start_value >= min_start_value && sensor2_start_value >= min_start_value)
    {
      for(int j=0; j<10; j+=1) 
      {
        sensor1_values[j] = analogRead(sensorPins[0]);
        sensor2_values[j] = analogRead(sensorPins[1]);
        
        if(sensor1_values[j] <= min_start_value && sensor2_values[j] <= min_start_value) 
        {
          measuring = false;
          break;
        }
        delay(1000);
      }
    }
    delay(1000);
  }
  SerialBT.println(sensor1_values);
  SerialBT.println(sensor2_values);
} 

void setup() 
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  SerialBT.register_callback(callback);
  SerialBT.begin("Hang'n Hold - Modul 1");
}

void start_msg() 
{
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
      case: 's':
        start_msg();
        break;
       
      case 'v':
        firmware_version();
        break;
  
      case 'c':
        calibration();
        break;
  
      case 'm':
        measurement();
        break;
  
      case 'b':
        mc_battery_status();
        break;
    }
  }
  delay(1000);
}
