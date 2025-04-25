#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

void setup() {
  Serial.begin(115200);
  Wire.begin(23, 22);  // SDA, SCL for Adafruit HUZZAH32
  delay(100);

  matrix.begin(0x70);  // Default I2C address for LED Matrix
  matrix.setRotation(0); // Adjust if upside down

  Serial.println("Matrix initialized. Displaying letter T...");

  matrix.clear();
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  matrix.setCursor(0, 0);
  matrix.print("T");
  matrix.writeDisplay();
}

void loop() {
  // Nothing needed here
}
