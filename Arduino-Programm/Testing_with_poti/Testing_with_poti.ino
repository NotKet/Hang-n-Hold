/*
<<<<<<< HEAD
 * Date: 28.12.2020
 * Author: Maximilian Maier
 * Project: Hang'n Hold
 * Description: Outputting poti-values to the app       
 * 
=======
    Hang'n Hold

    ====================

    This program let's you connect two bluetooth devices (two microcontrollers with sensors) to measure and process the receiving values.
    On the     

    
    Author: Maximlian Maier <maximilian.h.maier@gmail.com>
     
>>>>>>> 88a934e6bec2ac2ade2e7e183ba74e72aa2dfaaa
*/

// Initialize Bluetooth

<<<<<<< HEAD
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)             
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it    
#endif  

#define calibration_min_value 1000
=======
#include <BluetoothSerial.h>

>>>>>>> 88a934e6bec2ac2ade2e7e183ba74e72aa2dfaaa
#define min_sensor_value 2000
#define measure_interval 1000

BluetoothSerial SerialBT;

<<<<<<< HEAD


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
=======
const long sensorPin1 = 25,sensorPin2 = 26, ledPin = 33;
unsigned long sensor1_value = 0, sensor1_start_value = 0, sensor2_value = 0, sensor2_start_value = 0, previousMillis = 0, sensor1_sum = 0, sensor2_sum = 0, sensor_sum_max = 0, sensor1_percentage = 0, sensor2_percentage = 0;
bool measuring = false;

char app_input;


void measurement() {
  measuring = true;
  
  SerialBT.println("Messung wird gestartet. Bitte hänge dich nun an die Griffe ...");

  while(1) {
    sensor1_start_value = analogRead(sensorPin1);
    sensor2_start_value = analogRead(sensorPin2);
>>>>>>> 88a934e6bec2ac2ade2e7e183ba74e72aa2dfaaa
    

    if(sensor1_start_value >= min_sensor_value && sensor2_start_value >= min_sensor_value) {
      measuring = true;
      break;
    }
  }

  while(measuring == true) {
<<<<<<< HEAD
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
=======
    sensor1_value = analogRead(sensorPin1);
    sensor2_value = analogRead(sensorPin2);

    sensor1_sum += sensor1_value;
    sensor2_sum += sensor2_value;

    if(sensor1_value < 200 && sensor2_value < 200) {
      measuring = false;
    }
>>>>>>> 88a934e6bec2ac2ade2e7e183ba74e72aa2dfaaa
  }

  sensor_sum_max = sensor1_sum + sensor2_sum;

<<<<<<< HEAD
  sensor1_percentage = (sensor1_sum / sensor_sum_max) * 100;
  sensor2_percentage = (sensor2_sum / sensor_sum_max) * 100;
  
=======
  sensor1_percentage = (sensor_sum_max / sensor1_sum) * 100;
  sensor2_percentage = (sensor_sum_max / sensor2_sum) * 100;

>>>>>>> 88a934e6bec2ac2ade2e7e183ba74e72aa2dfaaa
  SerialBT.print("Sensor 1 percentage: ");
  SerialBT.println(sensor1_percentage);
  SerialBT.print("Sensor 2 percentage: ");
  SerialBT.println(sensor2_percentage);
}

<<<<<<< HEAD




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
=======
void battery_status() {
  
}

void arduino_version() {
  
}

void date() {
  
}

void calibration() {
>>>>>>> 88a934e6bec2ac2ade2e7e183ba74e72aa2dfaaa
  
}


<<<<<<< HEAD
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
=======
void setup() {
  pinMode(ledPin,OUTPUT);
  Serial.begin(115200);
  SerialBT.register_callback(callback);
  if(!SerialBT.begin("Hang'n Hold - 1")) {
    Serial.println("An error occurred initializing Bluetooth.");
  } else {
    Serial.println("Bluetooth initialized.");
  }
}


void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  // Callback function implementation

   while(true) {
    if (event == ESP_SPP_OPEN_EVT) {
      Serial.println("Client connected!");
      digitalWrite(ledPin, HIGH);
      break;
    }
      
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  }
}

void loop() {
  
  if(SerialBT.available()) {
    SerialBT.println("'v' - Version, 'd' - Date, 'c' - Calibration, 'm' - Measurement, 'b' - Battery status.'");
    app_input = SerialBT.read();
  }
 
>>>>>>> 88a934e6bec2ac2ade2e7e183ba74e72aa2dfaaa
  
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
