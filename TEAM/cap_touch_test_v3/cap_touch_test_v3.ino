const int touchPin = 36;  // Or 39, depending on where you wired CH0

void setup() {
  Serial.begin(115200);
  pinMode(touchPin, INPUT);
}

void loop() {
  int value = digitalRead(touchPin);

  if (value == LOW) {
    Serial.println("Touch detected! ✨");
  } else {
    Serial.println("No touch.");
  }

  delay(200);
}
