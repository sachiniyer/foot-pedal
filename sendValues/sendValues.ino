int buttreg[] = {1,2,3,4,5};

void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(buttreg[i], INPUT_PULLUP);
    Serial.begin(115200);
  }
}

void loop() {
  for (int i = 0; i < 5; i++) {
    Serial.print(analogRead(buttreg[i]));
  }
  Serial.print(analogRead(26));
  Serial.print(analogRead(30));
  delay(10);
}
