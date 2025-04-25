#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

const uint64_t letterBitmaps[4] = {
    0x181818181818ffff,
  0xffff031f1f03ffff,
  0xc3c3ffffc3c37e3c,
  0xc3c3c3dbdbffffe7
};

const char letters[] = {'T', 'E', 'A', 'M'};
int lastIndex = -1;  // Track last displayed letter index

void setup() {
    matrix.begin(0x70);  // Initialize matrix
    matrix.setBrightness(3);
    randomSeed(analogRead(0));  // Seed randomness
}

void loop() {
    int newIndex;

    // Ensure a new letter is chosen that is not the same as the previous one
    do {
        newIndex = random(0, 4);
    } while (newIndex == lastIndex);

    lastIndex = newIndex;
    displayLetter(newIndex);
    
    delay(3000); // Wait 3 seconds before updating
}

void displayLetter(int index) {
    matrix.clear();
    
    uint64_t letter = letterBitmaps[index];

    for (int i = 0; i < 8; i++) {
        matrix.displaybuffer[i] = (letter >> (8 * i)) & 0xFF;
    }
    
    matrix.writeDisplay();
}
