#include "BluetoothSerial.h"                                                      // Einbinden der Bilbiothek "BluetoothSerial"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)             // Abfrage, wo abgefragt wird, 
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it    // ob das Bluetooth vorhanden ist und falls nicht
#endif                                                                            // soll "make menuconfig" gestartet werden

BluetoothSerial SerialBT;                                                         // Eine Instanz von "BluetoothSerial" mit dem Namen "SerialBT" erstellt

const int led = 5;                                                                // Konstante integer variable mit dem Namen "led" und dem Wert 5
                                                                                  // wird erstellt
char incomingChar;                                                                // Ein Character mit dem Namen "incomingChar" wird definiert


void setup() {                                                                    // Erstellen der Initialisierungsfunktion "setup"
  pinMode(led, OUTPUT);                                                           // Variable "led" wird als "OUTPUT" definiert
  Serial.begin(115200);                                                           // Serielle Kommunikation wird mit 115200 Hz Baudrate definiert
  SerialBT.begin("ESP32test");                                                    // Benennung des Bluetooth-Moduls
  Serial.println("The device started, now you can pair it with bluetooth!");      // Ausgabe eines Textes
}

void loop() {                                                                     // Erstellen der Schleifenfunktion "loop"
  if (SerialBT.available()) {                                                     // Abfrage, ob über die serielle Bluetoothschnittstelle etwas gesendet wird
    incomingChar = SerialBT.read();                                               // Erstellen der Variable "incomingChar" und zuweisung des Wertes
                                                                                  // in seriellen Schnittstelle
    Serial.println(incomingChar);                                                 // Ausgabe der variable "incomingChar" im seriellen Monitor
  }

  if(incomingChar == 'e') {                                                       // Abfrage, ob die Variable "incomingChar" gleich dem character 'e' kommt
    digitalWrite(led, HIGH);                                                      // Falls Abfrage zutrifft, wird die Variable "led" auf "HIGH" gestellt
    Serial.println("Led ist an.");                                                // Ausgabe von Text in seriellen Monitor
  } else if(incomingChar == 'a') {                                                // Falls Abfrage nicht zutrifft, wird die weitere Bedingung gestellt, ob
                                                                                  // die Variable "incomingChar" gleich dem character 'a' kommt
    digitalWrite(led, LOW);                                                       // Falls Abfrage zutrifft, wird die Variable "led auf "HIGH" gestellt
    Serial.println("Led ist aus.");                                               // Ausgabe von Text in seriellen Monitor
  } 
}
