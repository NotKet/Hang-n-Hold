
int piezoPin = 34;               
 
void setup() {
 Serial.begin(115000);
}
 
void loop() {
  int wert = analogRead(piezoPin);   
  Serial.println(wert);
  delay(200);
}
