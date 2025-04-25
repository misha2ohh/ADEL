#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

char letters[] = {'T', 'E', 'A', 'M'};
int letterIndex = 0;
unsigned long lastChange = 0;
const long interval = 1000; // Change every 1000ms

void setup() {
  Serial.begin(115200);
  matrix.begin(0x70);  // Default I2C address for HT16K33
  randomSeed(analogRead(0));  // Seed randomness

  shuffleLetters();  // Start with a random order
}

void loop() {
  unsigned long now = millis();

  if (now - lastChange >= interval) {
    lastChange = now;

    char currentLetter = letters[letterIndex];
    displayLetter(currentLetter);

    Serial.print("Showing letter: ");
    Serial.println(currentLetter);

    letterIndex++;
    if (letterIndex >= 4) {
      shuffleLetters();  // Shuffle again after 4 letters
      letterIndex = 0;
    }
  }
}

void displayLetter(char c) {
  matrix.clear();
  matrix.setCursor(0, 0);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  matrix.print(c);
  matrix.writeDisplay();
}

void shuffleLetters() {
  for (int i = 3; i > 0; i--) {
    int j = random(0, i + 1);
    char temp = letters[i];
    letters[i] = letters[j];
    letters[j] = temp;
  }

  Serial.print("Shuffled: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(letters[i]);
    Serial.print(" ");
  }
  Serial.println();
}
