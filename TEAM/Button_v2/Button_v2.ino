// Pin assignments
const int buttonPin = 32;  // Button input
const int ledPin = 14;     // Button LED output

bool lastButtonState = HIGH;  // Tracks previous state (INPUT_PULLUP logic)

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Button goes LOW when pressed
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // LED starts off

  Serial.begin(115200);
  Serial.println("Ready: Press the button to light it up and log to Serial");
}

void loop() {
  bool buttonState = digitalRead(buttonPin);

  // Turn on LED if button is pressed
  if (buttonState == LOW) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  // Detect new press (falling edge)
  if (lastButtonState == HIGH && buttonState == LOW) {
    Serial.println("Button pressed!");
  }

  // Update last state
  lastButtonState = buttonState;
}
