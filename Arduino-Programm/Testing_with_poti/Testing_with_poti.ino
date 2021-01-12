/*
    Hang'n Hold

    ====================

    This program let's you connect two bluetooth devices (two microcontrollers with sensors) to measure and process the receiving values.
    On the     

    
    Author: Maximlian Maier <maximilian.h.maier@gmail.com>
     
*/

// Initialize Bluetooth

#include <BluetoothSerial.h>

#define min_sensor_value 2000
#define measure_interval 1000

BluetoothSerial SerialBT;

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
    

    if(sensor1_start_value >= min_sensor_value && sensor2_start_value >= min_sensor_value) {
      measuring = true;
      break;
    }
  }

  while(measuring == true) {
    sensor1_value = analogRead(sensorPin1);
    sensor2_value = analogRead(sensorPin2);

    sensor1_sum += sensor1_value;
    sensor2_sum += sensor2_value;

    if(sensor1_value < 200 && sensor2_value < 200) {
      measuring = false;
    }
  }

  sensor_sum_max = sensor1_sum + sensor2_sum;

  sensor1_percentage = (sensor_sum_max / sensor1_sum) * 100;
  sensor2_percentage = (sensor_sum_max / sensor2_sum) * 100;

  SerialBT.print("Sensor 1 percentage: ");
  SerialBT.println(sensor1_percentage);
  SerialBT.print("Sensor 2 percentage: ");
  SerialBT.println(sensor2_percentage);
}

void battery_status() {
  
}

void arduino_version() {
  
}

void date() {
  
}

void calibration() {
  
}


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
