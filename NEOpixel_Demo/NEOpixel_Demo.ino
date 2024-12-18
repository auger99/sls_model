#include <Adafruit_NeoPixel.h>

#define VANILLA_LED_pin 9

// Define the pin connected to the NeoPixel strip and the number of LEDs
#define NEO_pin 6
#define NUMPIXELS 5  // Adjust based on the number of LEDs in your strip

#define delay_time 500

// Create an instance of the NeoPixel library
Adafruit_NeoPixel pixels(NUMPIXELS, NEO_pin, NEO_GRB + NEO_KHZ800); 

char rgb   = 0b000;
char red   = 0;
char green = 0;
char blue  = 0;

void setup() {
  pixels.begin(); // Initialize the NeoPixel strip
  pixels.show();  // Turn off all LEDs by default
}

void loop() {
  // Austin's obfuscated way of changing color
  rgb++;
  rgb   = (rgb > 0b111) ? 0b001 : rgb;
  red   = ((rgb & 0b100) == 0b100) ? 255 : 0;
  green = ((rgb & 0b010) == 0b010) ? 255 : 0;
  blue  = ((rgb & 0b001) == 0b001) ? 255 : 0;

  // Change the LEDs forward
  for (int i = 0; i < NUMPIXELS; i++) { 
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
    pixels.show();  // Send the updated data to the strip

    delay(delay_time);      // Wait delay_time ms before moving to the next pixel

    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }

  // Change the LEDs backward
  for (int i = NUMPIXELS-2; i > 0; i--) { 
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
    pixels.show();  // Send the updated data to the strip
    
    delay(delay_time);      // Wait delay_time ms before moving to the next pixel

    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }
}
