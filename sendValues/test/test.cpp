#include <Arduino.h>
#include <unity.h>

int buttreg[] = {1,2,3,4,5};

void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(buttreg[i], INPUT_PULLUP);
    pinMode(24, INPUT_PULLUP);
    pinMode(25, INPUT_PULLUP);
    Serial.begin(115200);
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
  }
}

void loop() {
  for (int i = 0; i < 5; i++) {
    Serial.println(analogRead(buttreg[i]));
  }
  Serial.println("--------");
  Serial.println(analogRead(24));
  Serial.println(analogRead(25));
  delay(10);
}
