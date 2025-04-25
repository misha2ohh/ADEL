const int touchOutPin = 36; // or 39 if that's where KEY is connected

void setup() {
  Serial.begin(115200);
  pinMode(touchOutPin, INPUT);
}

void loop() {
  int state = digitalRead(touchOutPin);
  Serial.println(state == LOW ? "Touched!" : "Not touched");
  delay(100);
}
