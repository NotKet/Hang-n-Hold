/*
 * Date: 28.12.2020
 * Author: Maximilian Maier
 * Project: Hang'n Hold
 * Description: Outputting poti-values to the app       
 * 
*/

// Initianlize Bluetooth

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)             
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it    
#endif  

#define min_sensor_value 2000
#define measure_interval 1000

BluetoothSerial SerialBT;

const long sensor1 = 34,sensor2 = 35, led = 33;
unsigned long sensor1_value = 0, sensor1_start_value = 0, sensor2_value = 0, sensor2_start_value = 0, previousMillis = 0, sensor1_sum = 0, sensor2_sum = 0, sensor_sum_max = 0, sensor1_percentage = 0, sensor2_percentage = 0;
bool measuring = false;

char app_input;


void measurement() {
  measuring = true;
  
  SerialBT.println("Messung wird gestartet. Bitte hänge dich nun an die Griffe ...");

  while(1) {
    sensor1_start_value = analogRead(sensor1);
    sensor2_start_value = analogRead(sensor2);
    

    if(sensor1_start_value >= min_sensor_value && sensor2_start_value >= min_sensor_value) {
      measuring = true;
      break;
    }
  }

  while(measuring == true) {
    sensor1_value = analogRead(sensor1);
    sensor2_value = analogRead(sensor2_value);

    sensor1_sum += sensor1_value;
    sensor2_sum += sensor2_value;

    if(sensor1_value < 200 && sensor2_value < 200) {
      measure = false;
    }
  }

  sensor_sum_max = sensor1_sum + sensor2+sum;

  sensor1_percentage = (sensor_sum_max / sensor1_sum) * 100;
  sensor2_percentage = (sensor_sum_max / sensor2_sum) * 100;

  SerialBT.print("Sensor 1 percentage: ");
  SerialBT.println(sensor1_percentage);
  SerialBT.print("Sensor 2 percentage: ");
  SerialBT.println(sensor2_percentage);
}


void setup() {
  pinMode(ledPin, INPUT);
  Serial.begin(115200);
  SerialBT.begin("Hang'n Hold - 1");
}


void loop() {

  if(SerialBT.available()) {
    SerialBT.println("'v' - Version, 'd' - Date, 'c' - calibration.'");
    app_input = SerialBT.read();
  }
  
  switch(app_input) {
    case 'm':
      measurement();
      break;

    case 'b':
      battery_status();
      break;

    case 'd':
      date();
      break;

    case 'v':
      arduino_version();
      break;

    case 'c':
      calibration();
      break;
  }
}   
