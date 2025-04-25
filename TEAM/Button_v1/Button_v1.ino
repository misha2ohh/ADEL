// Pin assignments
const int buttonPin = 32;     // Button input pin
const int ledPin = 14;        // LED inside the button

// Timing variables
unsigned long previousMillis = 0;
const long interval = 2000;   // 2000ms = 2 seconds
bool ledState = LOW;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Assuming button connects to GND when pressed
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);   // Start LED in known state

  Serial.begin(115200);
  Serial.println("Blinking LED every 2 seconds...");
}

void loop() {
  unsigned long currentMillis = millis();

  // Blink LED every 2000ms
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
}
