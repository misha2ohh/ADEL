const int vibrationPin = 27; // Replace with your actual pin
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 20; // basic debounce

// Vibration duration tracking
bool inVibration = false;
unsigned long vibrationStartTime = 0;
unsigned long vibrationDuration = 0;

// Rolling intensity tracking
int vibrationCount = 0;
unsigned long intensityWindowStart = 0;
const unsigned long intensityWindow = 5000; // 5 seconds

void setup() {
  pinMode(vibrationPin, INPUT);
  Serial.begin(115200);
  Serial.println("timestamp_ms,vibration_event,vibration_duration_ms,vibration_count_last_5s");
}

void loop() {
  unsigned long now = millis();
  bool vibrationDetected = digitalRead(vibrationPin) == LOW;

  // Count vibration if detected
  if (vibrationDetected && !inVibration && (now - lastDebounceTime > debounceDelay)) {
    inVibration = true;
    vibrationStartTime = now;
    vibrationCount++;
    lastDebounceTime = now;
  }

  // Detect end of vibration
  if (!vibrationDetected && inVibration) {
    vibrationDuration = now - vibrationStartTime;
    inVibration = false;

    // Log CSV line
    Serial.print(now); Serial.print(",");
    Serial.print(1); Serial.print(","); // vibration_event = true
    Serial.print(vibrationDuration); Serial.print(",");
    Serial.println(vibrationCount);
  }

  // Reset vibration count every 5 seconds
  if (now - intensityWindowStart >= intensityWindow) {
    vibrationCount = 0;
    intensityWindowStart = now;
  }
}
