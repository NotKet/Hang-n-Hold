#include "LedLib.h"
#include "Pangodream_18650_CL.h"
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

#define fw_version 1.14
#define min_sensor_value 1000

const long sensorPin[] = {34, 35}, ledPin = 22;
const long otherPins[] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 
  14, 15, 16, 17, 18, 19, 21, 23, 25, 26, 27, 32, 
  33, 36, 39
  };   

LedLib ledlib(ledPin);
Pangodream_18650_CL BL;

bool deviceConnected = false, oldDeviceConnected = false, measuring;

void start_msg() {
  SerialBT.println(
    "Mögliche Eingaben: \n"
    "  'v' - Firmware version\n"
    "  'd' - Date\n"
    "  'c' - Calibration\n"
    "  'm' - Measurement\n"
    "  'b' - Battery status\n");
}

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if(event == ESP_SPP_SRV_OPEN_EVT){
    Serial.println("Client Connected");
    deviceConnected = true;
    ledlib.led_on();
    start_msg();
  }
 
  if(event == ESP_SPP_CLOSE_EVT ){
    Serial.println("Client disconnected");
    deviceConnected = false;
  }
}


void measurement() {
  Serial.println("Start measuring");
  int i = 0;
  measuring = true;
  unsigned long sensor1_values[100], sensor2_values[100];
  unsigned long sensor1_start_value, sensor2_start_value, 
      sensor1_sum, sensor2_sum, sensor_sum, average, 
      percentage_module1 = 0, percentage_module2 = 0;
  unsigned long endTime = 0, startTime;

  while(measuring) {
    sensor1_start_value = analogRead(sensorPin[0]);
    sensor2_start_value = analogRead(sensorPin[1]);
  
    if(sensor1_start_value > min_sensor_value && sensor2_start_value > min_sensor_value) {
      startTime = millis();
      Serial.println("Measuring ...");
      
      while(1) {
        sensor1_values[i] = analogRead(sensorPin[0]);
        sensor2_values[i] = analogRead(sensorPin[1]);
        i++;
        
        if(sensor1_values[i] < min_sensor_value && sensor2_values[i] < min_sensor_value) {
          endTime = (millis() - startTime); // / 1000;
          measuring = false;
          break;
        }
     
        delay(1000);
      }  
    }
    delay(1000);
  }
  
  for(int y=0; y<i; y++) {
    sensor1_sum += sensor1_values[y];
    sensor2_sum += sensor2_values[y];
  }

  Serial.println(sensor1_sum),
  Serial.println(sensor2_sum);
  
  
  sensor_sum = sensor1_sum + sensor2_sum;

  Serial.println(sensor_sum);

  average = sensor_sum / (2*i);
  percentage_module1 = (sensor1_sum / sensor_sum) * 100;
  percentage_module2 = (sensor2_sum / sensor_sum) * 100;
  
  Serial.print("Average: ");
  Serial.println(average);
  Serial.print("Percentage1: ");
  Serial.println(percentage_module1);
  Serial.print("Percentage2: ");
  Serial.println(percentage_module2); 
  Serial.print("Time: ");
  Serial.println(endTime);

  Serial.println("Measurement finished");
}

void firmware_version() {
  SerialBT.println(fw_version);
} 

void calibration() {
  Serial.println("Start calibrating");
  
  unsigned long sensor1_value, sensor2_value;
  
  while(1) {
    sensor1_value = analogRead(sensorPin[0]);
    sensor2_value = analogRead(sensorPin[1]);
    
    Serial.print("Sensor 1: ");
    Serial.println(sensor1_value);
    Serial.print("Sensor 2: ");
    Serial.println(sensor2_value);
    Serial.println("");

    delay(1000);

    if(SerialBT.available()) {
      char state = SerialBT.read();
      if(state == 'o') {
        SerialBT.println("Calibration finised");
        break;
      }
    }
  }
} 

void mc_battery_status() {
  Serial.print("Value from pin: ");
  Serial.println(analogRead(34));
  Serial.print("Average value from pin: ");
  Serial.println(BL.pinRead());
  Serial.print("Volts: ");
  Serial.println(BL.getBatteryVolts());
  Serial.print("Charge level: ");
  Serial.println(BL.getBatteryChargeLevel());
  Serial.println("");
  delay(1000);
}

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  SerialBT.register_callback(callback);
  SerialBT.begin("Hang'n Hold - Module 1");
}

void loop() {
  while(!deviceConnected) {
    ledlib.blink_once();
  }
  
  if(SerialBT.available()) {
    char state = SerialBT.read();

    switch(state) {
    case 'm':
      measurement();
      break;

    case 'c':
      calibration();
      break;

    case 'v':
      firmware_version();
      break;

    case 'b':
      mc_battery_status();
      break;

    default:
      break;
    }
  }
}
