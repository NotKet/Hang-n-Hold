int led = 35;
int btn = 34;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(btn, INPUT);
  Serial.begin(9600);
}

void loop() {
  int btn_status = digitalRead(btn);
  if(btn_status == HIGH) {
    Serial.print("Led: ");
    Serial.println(btn_status);
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
    Serial.print("Led: ");
    Serial.println(btn_status);
  }
}
