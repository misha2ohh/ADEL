const int buttonPin = 32;  // Your button input pin
const int ledPin = 14;     // Your LED pin

unsigned long lastPressTime = 0;
bool lastButtonState = HIGH;
int buttonID = 1;  // You can set this uniquely for each button if you have multiple

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);

  // Print CSV header
  Serial.println("timestamp_ms,button_id,time_since_last");
}

void loop() {
  bool buttonState = digitalRead(buttonPin);
  unsigned long now = millis();

  if (buttonState == LOW) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  // Detect new press (falling edge)
  if (lastButtonState == HIGH && buttonState == LOW) {
    unsigned long timeSinceLast = now - lastPressTime;
    lastPressTime = now;

    // Print in CSV format
    Serial.print(now);
    Serial.print(",");
    Serial.print(buttonID);
    Serial.print(",");
    Serial.println(timeSinceLast);
  }

  lastButtonState = buttonState;
}
