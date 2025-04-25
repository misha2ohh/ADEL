void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("If you see this, Serial works!");
}

void loop() {
  delay(1000);
  Serial.println("Tick...");
}
