#include "Pangodream_18650_CL.h"
#include "BluetoothSerial.h"
#include "LedLib.h"

#define fw_version 1.00


BluetoothSerial SerialBT;
Pangodream_18650_CL BL;

const long sensorPin[] = {34, 35}, ledPin = 22, otherPins = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 23, 25, 26, 27, 32, 33, 36, 39];               

boolean bt_connected = false, measuring = false;

const unsigned long min_sensor_value = 2000;

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
  measuring = false;
  unsigned long sensor1_values[100], sensor2_values[100];
  unsigned long sensor1_start_value, sensor2_start_value, sensor1_sum, sensor2_sum, sensor_sum,average, percentage_module1, percentage_module2;
  
  SerialBT.println("Messung wurde gestartet.");
  SerialBT.println("Bitte hänge dich nun an die Griffe...");
  
  while(measuring) 
  {
    sensor1_start_value = analogRead(sensorPin[0]);
    sensor2_start_value = analogRead(sensorPin[1]);

    SerialBT.print("Sensor 1: ");
    SerialBT.println(sensor1_start_value);
    SerialBT.print("Sensor 2: ");
    SerialBT.println(sensor2_start_value);
    SerialBT.println("");

  
    if(sensor1_start_value >= min_sensor_value && sensor2_start_value >= min_sensor_value) 
    {
      SerialBT.println("Starting the measurement.");
      SerialBT.println("");
      
      while(1) 
      {
        sensor1_values[i] = analogRead(sensorPin[0]);
        sensor2_values[i] = analogRead(sensorPin[1]);

        SerialBT.print("i: ");
        SerialBT.println(i);
        SerialBT.print("Sensor 1: ");
        SerialBT.println(sensor1_values[i]);
        SerialBT.print("Sensor 2: ");
        SerialBT.println(sensor2_values[i]);
        SerialBT.println("");
        
        if(sensor1_values[i] <= min_sensor_value && sensor2_values[i] <= min_sensor_value) 
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
  Serial.println(analogRead(33));
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
  SerialBT.println("Type 'o' to end calibration.");
  SerialBT.println();

  unsigned long startTime = millis();
  unsigned long endTime = 0;
  
  while(1) 
  {
    SerialBT.print("Sensor 1: ");
    SerialBT.println(analogRead(sensorPin[0]));
    SerialBT.print("Sensor 2: ");
    SerialBT.println(analogRead(sensorPin[1]));
    SerialBT.println("");

    delay(1000);

    if(SerialBT.available()) 
    {
      if(SerialBT.read() == 'o') 
      {
        endTime = millis() - startTime;
        SerialBT.println(endTime);
        break;
      }
    }
  }
} 

void setup() 
{
  pinMode(ledPin, OUTPUT);

  for(int i=0; i<sizeof(otherPins); i++) {
    pinMode(otherPins[i], OUTPUT);
    digitalWrite(otherPins[i], LOW); 
  }
  
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
    ledlib.blink_once();
  }
  if(SerialBT.available()) 
  {
    switch(SerialBT.read()) 
    {
      case 's':
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
