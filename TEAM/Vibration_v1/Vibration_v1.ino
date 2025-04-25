const int ledPin = 14;          // Arcade button LED
const int vibrationPin = 27;    // Vibration sensor input

bool lastVibrationState = HIGH;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(vibrationPin, INPUT);   // For most modules, HIGH = normal, LOW = shake

  digitalWrite(ledPin, LOW);
  Serial.begin(115200);
  Serial.println("Vibration-triggered flash system ready.");
}

void loop() {
  bool vibrationState = digitalRead(vibrationPin);

  // Trigger on falling edge (shake detected)
  if (lastVibrationState == HIGH && vibrationState == LOW) {
    Serial.println("Vibration detected! Flashing LED...");
    flashButtonLED(5, 100);  // 5 times, 100ms per on/off
  }

  lastVibrationState = vibrationState;
}

void flashButtonLED(int flashes, int interval) {
  for (int i = 0; i < flashes; i++) {
    digitalWrite(ledPin, HIGH);
    delay(interval);
    digitalWrite(ledPin, LOW);
    delay(interval);
  }
}
