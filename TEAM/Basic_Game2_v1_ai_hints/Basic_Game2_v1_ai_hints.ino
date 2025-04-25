// TEAM REACTION GAME WITH MATRIX, LCD, BUTTON, VIBRATION LOGGING
// Built for ESP32 with Adafruit 8x8 Matrix + I2C LCD + Arcade Button + Vibration Sensor
// v7 - Displays AI hint on the top line of the LCD and logs letter display duration

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <LiquidCrystal_I2C.h>

// ========== PIN DEFINITIONS ==========
const int buttonPin = 14;
const int ledPin = 32;
const int vibrationPin = 27;

// ========== I2C DEVICE ADDRESSES ==========
LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

// ========== GAME SETTINGS ==========
const char* targetWord = "TEAM";
const char letterPool[] = {'T', 'E', 'A', 'M', 'F', 'N'};
int letterDisplayDuration = 600; // Now adjustable
const int gameDuration = 60000; // 60 seconds

// ========== STATE VARIABLES ==========
char currentLetter = ' ';
char lastLetter = ' ';
int currentLetterIndex = 0;
String currentProgress = "";
int score = 0;
unsigned long lastLetterChangeTime = 0;
unsigned long gameStartTime;
bool inGame = false;
bool letterWasCorrect = false;
bool buttonPressedThisCycle = false;
bool waitingForStart = true;
String currentHint = "";

// Vibration logging
bool lastVibrationState = HIGH;
unsigned long vibrationStart = 0;
int vibrationCount = 0;

// Session/Round tracking
String sessionID = String((uint32_t)(esp_random()));
int roundNumber = 0;

// Accuracy tracking for adaptive difficulty
#define HISTORY_SIZE 5
bool recentResults[HISTORY_SIZE] = {true, true, true, true, true};
int historyIndex = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(23, 22);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(vibrationPin, INPUT);
  digitalWrite(ledPin, LOW);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press to Start");

  matrix.begin(0x70);
  matrix.setRotation(0);
  matrix.clear();
  matrix.writeDisplay();

  Serial.println("session_id,round_id,timestamp_ms,letter_shown,button_pressed,correct_press,missed_opportunity,vibration_event,vibration_duration_ms,vibration_count,progress,score,time_left_ms,display_duration_ms");
}

void loop() {
  unsigned long now = millis();

  if (waitingForStart) {
    static unsigned long lastFlash = 0;
    static bool ledState = false;
    if (now - lastFlash > 500) {
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
      lastFlash = now;
    }

    static bool lastButtonStateStart = HIGH;
    bool buttonStateStart = digitalRead(buttonPin);
    if (lastButtonStateStart == HIGH && buttonStateStart == LOW) {
      waitingForStart = false;
      inGame = true;
      roundNumber++;
      currentProgress = "";
      score = 0;
      currentLetterIndex = 0;
      vibrationCount = 0;
      lcd.clear();
      digitalWrite(ledPin, LOW);
      gameStartTime = millis();
      showNewLetter();
    }
    lastButtonStateStart = buttonStateStart;
    return;
  }

  unsigned long timeLeft = gameDuration - (now - gameStartTime);

  if (inGame && now - lastLetterChangeTime >= letterDisplayDuration) {
    if (letterWasCorrect && !buttonPressedThisCycle) {
      logEvent(now, false, true, true, timeLeft);
    }
    showNewLetter();
  }

  // Detect button press
  static bool lastButtonState = HIGH;
  bool buttonState = digitalRead(buttonPin);
  if (lastButtonState == HIGH && buttonState == LOW) {
    bool correct = (currentLetter == targetWord[currentLetterIndex]);
    buttonPressedThisCycle = true;

    // Update rolling accuracy history
    recentResults[historyIndex] = correct;
    historyIndex = (historyIndex + 1) % HISTORY_SIZE;
    adjustDifficulty();

    if (correct) {
      currentProgress += currentLetter;
      currentLetterIndex++;
      if (currentLetterIndex >= strlen(targetWord)) {
        score++;
        currentProgress = "";
        currentLetterIndex = 0;
      }
    } else {
      currentProgress = "";
      currentLetterIndex = 0;
    }

    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);

    logEvent(now, true, correct, false, timeLeft);
  }
  lastButtonState = buttonState;

  // Vibration logging
  bool vibrationDetected = digitalRead(vibrationPin) == LOW;
  if (vibrationDetected && lastVibrationState == HIGH) {
    vibrationStart = now;
    vibrationCount++;
    logEvent(now, false, false, false, timeLeft);
  }
  lastVibrationState = !vibrationDetected;

  // Update LCD
  lcd.setCursor(0, 0);
  lcd.print(padRight(currentHint, 16));
  lcd.setCursor(0, 1);
  lcd.print(String(timeLeft / 1000) + "s   " + String(score) + "x   ");

  // End of game
  if (inGame && now - gameStartTime >= gameDuration) {
    endGame();
  }
}

void adjustDifficulty() {
  int correctCount = 0;
  for (int i = 0; i < HISTORY_SIZE; i++) {
    if (recentResults[i]) correctCount++;
  }

  if (correctCount <= 2) {
    letterDisplayDuration = 1000;
    currentHint = "Slowing down...";
  } else if (correctCount >= 4) {
    letterDisplayDuration = 500;
    currentHint = "Speeding up!";
  } else {
    letterDisplayDuration = 750;
    currentHint = "Keep going!";
  }
}

void showNewLetter() {
  lastLetterChangeTime = millis();
  do {
    currentLetter = letterPool[random(0, sizeof(letterPool))];
  } while (currentLetter == lastLetter);
  lastLetter = currentLetter;
  buttonPressedThisCycle = false;
  letterWasCorrect = (currentLetter == targetWord[currentLetterIndex]);

  matrix.clear();
  matrix.setCursor(0, 0);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  matrix.print(currentLetter);
  matrix.writeDisplay();
}

void logEvent(unsigned long timestamp, bool buttonPressed, bool correct, bool missedOpportunity, unsigned long timeLeft) {
  Serial.print(sessionID); Serial.print(",");
  Serial.print(roundNumber); Serial.print(",");
  Serial.print(timestamp); Serial.print(",");
  Serial.print(currentLetter); Serial.print(",");
  Serial.print(buttonPressed ? 1 : 0); Serial.print(",");
  Serial.print(correct ? 1 : 0); Serial.print(",");
  Serial.print(missedOpportunity ? 1 : 0); Serial.print(",");
  Serial.print((digitalRead(vibrationPin) == LOW) ? 1 : 0); Serial.print(",");
  Serial.print((millis() - vibrationStart)); Serial.print(",");
  Serial.print(vibrationCount); Serial.print(",");
  Serial.print(currentProgress); Serial.print(",");
  Serial.print(score); Serial.print(",");
  Serial.println(timeLeft);
  Serial.print(","); Serial.println(letterDisplayDuration);
}

void endGame() {
  inGame = false;
  waitingForStart = true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Round Over!");
  lcd.setCursor(0, 1);
  lcd.print("Score: " + String(score));

  for (int i = 0; i < 6; i++) {
    digitalWrite(ledPin, HIGH);
    delay(150);
    digitalWrite(ledPin, LOW);
    delay(150);
  }
}

String padRight(String str, int len) {
  while (str.length() < len) str += " ";
  return str;
}
