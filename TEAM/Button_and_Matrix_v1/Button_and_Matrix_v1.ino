#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

const int buttonPin = 32;
const int ledPin = 14;

char letters[] = {'T', 'E', 'A', 'M'};
char currentLetter = ' ';
bool lastButtonState = HIGH;

void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLUP);  // Use internal pull-up
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);         // LED off initially

  matrix.begin(0x70);                // Default I2C address
  randomSeed(analogRead(0));         // Seed random number generator

  showRandomLetter();                // Show first letter
}

void loop() {
  bool buttonState = digitalRead(buttonPin);

  if (lastButtonState == HIGH && buttonState == LOW) {
    // Button just pressed
    digitalWrite(ledPin, HIGH);       // Turn on LED
    showRandomLetter();               // Change matrix letter

    Serial.print("Button pressed! New letter: ");
    Serial.println(currentLetter);
  }

  if (lastButtonState == LOW && buttonState == HIGH) {
    // Button just released
    digitalWrite(ledPin, LOW);        // Turn off LED
  }

  lastButtonState = buttonState;
}

void showRandomLetter() {
  int index = random(0, 4);
  currentLetter = letters[index];

  matrix.clear();
  matrix.setCursor(0, 0);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  matrix.print(currentLetter);
  matrix.writeDisplay();
}
