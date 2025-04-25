#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

//const uint8_t letterBitmaps[4][8] = {
//    {0x3C, 0x66, 0x60, 0x78, 0x60, 0x66, 0x3C, 0x00}, // T
//    {0x6E, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00}, // E
//    {0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00}, // A
//    {0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00}  // M
//};


const uint8_t letterBitmaps[4][8] = {
//const uint8_t IMAGES[][8] = {
{
  B00000000, // 'T'
  B01111110,
  B01011010,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00011000
},{
//  B01111110,  // Flip the 'E' vertically
//  B01100000,
//  B01100000,
//  B01111100,
//  B01100000,
//  B01100000,
//  B01111110,
//  B00000000
  B00000000, // 'E' mirrored
  B00111111,
  B00000011,
  B00000011,
  B00011111,
  B00000011,
  B00000011,
  B00111111
},{
  B00000000, // 'A' 
  B00111100,
  B01100110,
  B01100110,
  B01111110,
  B01100110,
  B01100110,
  B01100110
},{
  B00000000, // 'M'
  B01100011,
  B01110111,
  B01111111,
  B01101011,
  B01100011,
  B01100011,
  B01100011
}};

const char letters[] = {'T', 'E', 'A', 'M'};
int lastIndex = -1;  // Track the last displayed letter index

void setup() {
    matrix.begin(0x70);  // Initialize matrix at default I2C address
    matrix.setBrightness(1);
    randomSeed(analogRead(0)); // Random seed from floating input
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
    for (int i = 0; i < 8; i++) {
        matrix.displaybuffer[i] = letterBitmaps[index][i];
    }
    matrix.writeDisplay();
}
