const int piezoPin = 34;               

void setup() {
  pinMode(piezoPin, INPUT); 
  Serial.begin(115200); 
} 

void loop() {    
  Serial.println(analogRead(piezoPin)); 
  delay(200); 
} 
