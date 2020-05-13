void setup() {
	Serial.begin(115200);
	pinMode(0,INPUT_PULLUP);
}

void loop () {
	Serial.print(digitalRead(0));
}
