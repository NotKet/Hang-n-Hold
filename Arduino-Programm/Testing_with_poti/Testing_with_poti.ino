/*
 * Date: 28.12.2020
 * Author: Maximilian Maier
 * Project: Hang'n Hold
 * Description: Outputting poti-values to the app         
 * 
*/

// Initianlize Bluetooth

#include "BluetoothSerial.h"
#include "TimeLib.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)             
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it    
#endif  

BluetoothSerial SerialBT;

//*******************************

// Declaring variables

const long sensorPin = 34, ledPin = 33, interval = 1000;
unsigned long sensor_value = 0, sensor_start_value = 0, previousMillis = 0, sum = 0;
bool messung = false;

char app_input;

//*******************************

void calibration() {
  unsigned long count = 0;
  SerialBT.println("Messung wird gestartet. Bitte hänge dich nun an die Griffe ...");
  sensor_start_value = analogRead(sensorPin);

  if(sensor_value > 2000) {
    messung = true;
  }

  while(messung == true) {
    sensor_value = analogRead(sensorPin);  
    SerialBT.println(sensor_value);

    count += 1;
    
    if(sensor_value < 100) {
      messung = false;
  }

  for(int i=0; i < count; i+=1) {
    
  }
  messung = false;
}

void current_date() {
   time_t t = now();
   SerialBT.print(day(t));
   SerialBT.print("-");
   SerialBT.print(month(t));
   SerialBT.print("-");
   SerialBT.print(year(t));
   SerialBT.print(",");
   SerialBT.print(hour(t));
   SerialBT.print(":");
   SerialBT.print(minute(t));
}

//*******************************

// Setup-function

void setup() {
  pinMode(ledPin, INPUT);
  Serial.begin(115200);
  SerialBT.begin("Hang'n Hold - 1");
}

//*******************************

// Program loop

void loop() {

  if(SerialBT.available()) {
    SerialBT.println("'v' - Version, 'd' - Date, 'c' - calibration, 'i' - LED on, 'o' - LED off.'");
    app_input = SerialBT.read();
  
    switch(app_input) {
  
      case "c":
        calibration();
        break;
  
      case "d":
        current_date();
        break;
        
      default:
        break;
    }
  }
}   
