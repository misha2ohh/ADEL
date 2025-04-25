#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Try 0x27 first. If it doesn't work, try 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);  // (address, columns, rows)

void setup() {
  Serial.begin(115200);
  Wire.begin(23, 22);  // SDA = GPIO 23, SCL = GPIO 22 for HUZZAH32
  delay(500);

  lcd.init();          // Initialize LCD
  lcd.backlight();     // Turn on the backlight

  lcd.setCursor(0, 0); // Column 0, Row 0
  lcd.print("Hello TEAM!");

  lcd.setCursor(0, 1); // Column 0, Row 1
  lcd.print("AI is watching");

  Serial.println("LCD initialized and text sent.");
}

void loop() {
  // Nothing in the loop
}
