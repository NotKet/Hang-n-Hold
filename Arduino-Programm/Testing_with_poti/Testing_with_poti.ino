/*
 * Date: 28.12.2020
 * Author: Maximilian Maier
 * Project: Hang'n Hold
 * Description: Outputting poti-values to the app       
 * 
*/

// Initialize Bluetooth

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)             
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it    
#endif  

#define calibration_min_value 1000
#define min_sensor_value 2000
#define measure_interval 1000

BluetoothSerial SerialBT;



// Declaring variables

const long sensor1 = 34,sensor2 = 35, led = 33;
unsigned long sensor1_value = 0, sensor1_start_value = 0, sensor2_value = 0, sensor2_start_value = 0, previousMillis = 0, sensor1_sum = 0, sensor2_sum = 0, sensor_sum_max = 0, sensor1_percentage = 0, sensor2_percentage = 0, calibration_sensor1 = 0, calibration_sensor2 = 0;
bool measuring = false;

unsigned long sensor1_arr [1000], sensor2_arr [1000];

char app_input;




// Starting a measurement

void measurement() {
  measuring = true;
  unsigned long count = 0;
  
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

    sensor1_arr[count] = sensor1_value;
    sensor2_arr[count] = sensor2_value;
    
    if(sensor1_value < 200 && sensor2_value < 200) {
      measuring = false;
    }

    count += 1;
  }

  for(int i = 0; i <= count;  i += 1) {
    sensor1_sum += sensor1_arr[i];
    sensor2_sum += sensor2_arr[i];
  }

  sensor_sum_max = sensor1_sum + sensor2_sum;

  sensor1_percentage = (sensor1_sum / sensor_sum_max) * 100;
  sensor2_percentage = (sensor2_sum / sensor_sum_max) * 100;
  
  SerialBT.print("Sensor 1 percentage: ");
  SerialBT.println(sensor1_percentage);
  SerialBT.print("Sensor 2 percentage: ");
  SerialBT.println(sensor2_percentage);
}





void date() {
  
}





void calibration() {
  SerialBT.println("Kalibrierung wird gestarted ...");
  
  while(1){
    SerialBT.println("Bitte belaste nun Griff 1!");
    calibration_sensor1 = analogRead(sensor1);
    
    if(calibration_sensor1 >= calibration_min_value) {
      SerialBT.println("Sensor 1 funktioniert einwandfrei.");
      break;
    } else {
      SerialBT.println("Ups! Etwas ist schiefgelaufen. Versuch es nochmal.");
    }

    delay(5000);
  }

  while(1) {
    SerialBT.println("Bitte belaste nun Griff 2!");
    calibration_sensor2 = analogRead(sensor2);

    if(calibration_sensor2 >= calibration_min_value) {
      SerialBT.println("Sensor 1 funktioniert einwandfrei.");
      break;
    } else {
      SerialBT.println("Ups! Etwas ist schiefgelaufen. Versuch es nochmal.");
    }

    delay(5000);
  }
  
}




void arduino_version() {
  
}




void battery_status() {
  
}


// Initalize led, Serial Bluetooth Monitor, Serial Monitor

void setup() {
  pinMode(led, INPUT);
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
