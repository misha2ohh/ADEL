const int buttonPin = 32;
const int ledPin = 14;
const int vibrationPin = 27;

// For analog sensors: uncomment this and plug into an analog pin
// const int vibrationAnalogPin = 34;

bool lastVibrationState = HIGH;
bool lastButtonState = HIGH;

unsigned long vibrationTimestamp = 0;
unsigned long roundStartTime = 0;
int vibrationCount = 0;
int roundDuration = 10000; // 10 seconds

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(vibrationPin, INPUT);  // For digital

  Serial.begin(115200);
  Serial.println("timestamp,vibration_event,vibration_timestamp,vibration_count_per_round,vibration_during_press,vibration_intensity");
}

void loop() {
  unsigned long now = millis();

  // Reset per round
  if (now - roundStartTime >= roundDuration) {
    vibrationCount = 0;
    roundStartTime = now;
    Serial.println("----- NEW ROUND -----");
  }

  bool vibrationDetected = digitalRead(vibrationPin) == LOW; // LOW = vibration
  bool buttonPressed = digitalRead(buttonPin) == LOW; // LOW = pressed

  if (vibrationDetected && lastVibrationState == HIGH) {
    vibrationTimestamp = now;
    vibrationCount++;

    // Simulate intensity for digital sensor (set to 1)
    int intensity = 1;

    // If analog sensor, uncomment below:
    // int intensity = analogRead(vibrationAnalogPin);

    // Log to Serial
    Serial.print(now); Serial.print(",");
    Serial.print(1); Serial.print(",");
    Serial.print(vibrationTimestamp); Serial.print(",");
    Serial.print(vibrationCount); Serial.print(",");
    Serial.print(buttonPressed ? 1 : 0); Serial.print(",");
    Serial.println(intensity);
  }

  lastVibrationState = digitalRead(vibrationPin);
}
