// Simple ESP32 touch sensor test using pins 36 and 39

const int touchPin1 = 36;  // T0
const int touchPin2 = 39;  // T3

void setup() {
  pinMode(touchPin1, INPUT_PULLUP);
  pinMode(touchPin2, INPUT_PULLUP);
  Serial.begin(115200);
  delay(1000);
  Serial.println("Touch test start. Touch the pads connected to GPIO 36 or 39.");
}

void loop() {
  int value1 = touchRead(touchPin1);
  int value2 = touchRead(touchPin2);

  Serial.print("Touch 36: ");
  Serial.print(value1);
  Serial.print(" | Touch 39: ");
  Serial.println(value2);

  delay(200);
}
 
