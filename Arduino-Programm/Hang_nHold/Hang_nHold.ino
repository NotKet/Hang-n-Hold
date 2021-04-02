#include "LedLib.h"
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

bool deviceConnected = false, oldDeviceConnected = false, measuring = false;

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
  float sensor1_values[100], sensor2_values[100];

  float sensor1_start_value, sensor2_start_value, 
        sensor1_sum, sensor2_sum, sensor_sum, average, 
        percentage_module1, percentage_module2,
        endTime, startTime, temp1, temp2;


  SerialBT.println("\nStart measuring\n");
  int cnt = 0;
  measuring = true;

  while(measuring) {
    sensor1_start_value = analogRead(sensorPin[0]);
    sensor2_start_value = analogRead(sensorPin[1]);
  
    if(sensor1_start_value > min_sensor_value && sensor2_start_value > min_sensor_value) {
      SerialBT.println("\nMeasuring ...\n");
      startTime = millis();
      
      while(1) {
        temp1 = analogRead(sensorPin[0]);
        temp2 = analogRead(sensorPin[1]);
        
        if(temp1 < min_sensor_value || temp2 < min_sensor_value) {
          endTime = (millis() - startTime) / 1000;
          measuring = false;
          break;
        }

        sensor1_values[cnt] = temp1;
        sensor2_values[cnt] = temp2;

        cnt++;
     
        delay(500);
      }  
    }
  }
  
  for(int y=0; y<=cnt; y++) {
    sensor1_sum += sensor1_values[y];
    sensor2_sum += sensor2_values[y];
  }
  
  sensor_sum = sensor1_sum + sensor2_sum;
  average = sensor_sum / (2*cnt);
  percentage_module1 = (sensor1_sum / sensor_sum) * 100;
  percentage_module2 = (sensor2_sum / sensor_sum) * 100;

  SerialBT.print("\nAverage: ");
  SerialBT.println(average);
  SerialBT.print("Module 1: ");
  SerialBT.print(percentage_module1);
  SerialBT.println("%");
  SerialBT.print("Module 2: ");
  SerialBT.print(percentage_module2); 
  SerialBT.println("%");
  SerialBT.print("Time: ");
  SerialBT.print(endTime);
  SerialBT.println("s");
  
  SerialBT.println("\nMeasurement finished\n");
}


void firmware_version() {
  SerialBT.println(fw_version);
} 

void calibration() {
  float sensor1_value, sensor2_value;

  Serial.println("Start calibrating");
  
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

    default:
      break;
    }
  }
}
