#include "LedLib.h"
#include "Pangodream_18650_CL.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define fw_version 1.13
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
BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
uint8_t value = 0;

void start_msg() {
  pCharacteristic->setValue(
    "Mögliche Eingaben: \n"
    "  'v' - Firmware version\n"
    "  'd' - Date\n"
    "  'c' - Calibration\n"
    "  'm' - Measurement\n"
    "  'b' - Battery status\n");
  pCharacteristic->notify();
}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Device connected");
      ledlib.led_on();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("Device disconnected");
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();
    
    if (rxValue.length() > 0) {
      Serial.print("Input: ");
      Serial.println(rxValue[0]);
      Serial.println("");

      for (int i = 0; i < rxValue.length(); i++) {
        switch(rxValue[i]) {
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
        }
      } 
    }
  }
};

void ble_setup() {
  BLEDevice::init("HangnHold - Module 1");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                      );
  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();
  pServer->getAdvertising()->start();
}

void measurement() {
  int i = 0;
  measuring = true;
  unsigned long sensor1_values[100], sensor2_values[100];
  unsigned long sensor1_start_value, sensor2_start_value, 
      sensor1_sum, sensor2_sum, sensor_sum, average, 
      percentage_module1 = 0, percentage_module2 = 0;
  unsigned long endTime = 0, startTime;

  Serial.println("Starting measurement");
  
  while(measuring) {
    sensor1_start_value = analogRead(sensorPin[0]);
    sensor2_start_value = analogRead(sensorPin[1]);
  
    if(sensor1_start_value > min_sensor_value && sensor2_start_value > min_sensor_value) {
      startTime = millis();
      Serial.println("Measuring ...");
      
      while(1) {
        sensor1_values[i] = analogRead(sensorPin[0]);
        sensor2_values[i] = analogRead(sensorPin[1]);
        
        if(sensor1_values[i] < min_sensor_value && sensor2_values[i] < min_sensor_value) {
          endTime = (millis() - startTime); // / 1000;
          measuring = false;
          Serial.println("Messung beendet");
          break;
        }
  
        i++;
        delay(1000);
      }  
    }
    delay(1000);
  }
  
  for(int y=0; y<=i; y++) {
    sensor1_sum += sensor1_values[y];
    sensor2_sum += sensor2_values[y];
  }

  Serial.println(i);
  Serial.println(sensor1_sum),
  Serial.println(sensor2_sum);
  
  
  sensor_sum = sensor1_sum + sensor2_sum;

  Serial.println(sensor_sum);

  average = sensor_sum / (2*i);
  percentage_module1 = (sensor1_sum / sensor_sum) * 100;
  percentage_module2 = (sensor2_sum / sensor_sum) * 100;
  
  Serial.print("Durchschnitt: ");
  Serial.println(average);
  Serial.print("Prozentsatz Modul 1: ");
  Serial.println(percentage_module1);
  Serial.print("Prozentsatz Modul 2: ");
  Serial.println(percentage_module2); 
  Serial.print("Gesamtzeit: ");
  Serial.println(endTime);
}

void firmware_version() {
  char txString[8];
  dtostrf(fw_version, 1, 2, txString);
  pCharacteristic->setValue(txString);
} 

void calibration() {
  unsigned long startTime = millis(), endTime = 0;
  unsigned long sensor1_value = 0, sensor2_value = 0;
  
  while(1) {
    sensor1_value = analogRead(sensorPin[0]);
    sensor2_value = analogRead(sensorPin[1]);

    if(sensor1_value < min_sensor_value && sensor2_value < min_sensor_value) {
      break;
    }
    
    Serial.print("Sensor 1: ");
    Serial.println(sensor1_value);
    Serial.print("Sensor 2: ");
    Serial.println(sensor2_value);
    Serial.println("");

    delay(1000);
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
  
  ble_setup();
}

void loop() {
  while(!deviceConnected) {
    ledlib.blink_once();
  }
}
