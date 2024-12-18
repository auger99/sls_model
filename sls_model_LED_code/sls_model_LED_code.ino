#include <Adafruit_NeoPixel.h>

// Define button pin
#define BUTTON_PIN 10

// Define button debouncer values
#define NUM_CHECKS 10
#define CHECK_DELAY 2

// Define the pin connected to the NeoPixel strip and the number of LEDs
#define NEO_PIN 6
#define NUMPIXELS 100  // Adjust based on the number of LEDs in your strip -- Go big or go home

// Define RGB value for orange
#define RED   255
#define GREEN 35
#define BLUE  0

// Define LED intensities
typedef enum {
  OFF  = 0,
  FULL = 1,
  HALF,
  THREE_QUARTER,
} intensity_t;

// Define state machine values
typedef enum {
  STATE_OFF = 0,
  STATE_ON  = 1,
  STATE_LIFTOFF,
} state_t;

// Define liftoff routine values
#define LIFTOFF_BUTTON_WAIT 500   //milliseconds
#define LIFTOFF_ITR_MIN 25
#define LIFTOFF_ITR_MAX 50
#define LIFTOFF_OFF_MIN 10
#define LIFTOFF_OFF_MAX 50
#define LIFTOFF_ON_MIN 50
#define LIFTOFF_ON_MAX 200



// Create an instance of the NeoPixel library
Adafruit_NeoPixel pixels(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800); 



void setup() {
  pinMode(BUTTON_PIN, INPUT);

  pixels.begin(); // Initialize the NeoPixel strip
  pixels.show();  // Initialize all LEDs as off
}

void loop() {
  static bool button_press = 0;
  static state_t state = STATE_OFF;

  if (digitalRead(BUTTON_PIN) == 1) {
    button_press = debounce(BUTTON_PIN);
  }

  if (button_press == 1) {          // Change state if the button is pressed
    switch (state) {                // State machine
      case STATE_OFF:
        state = state_off();
        break;
      case STATE_ON:
        state = state_on();
        break;
      case STATE_LIFTOFF:
        state = state_liftoff();
        break;
      default:
        state = state_on();         // If broken assume was on, change state to off
        break;
    }
  }

  while(debounce(BUTTON_PIN));      // Wait until button is unpressed
  button_press = 0;
}



// Debouncer function
bool debounce(int button) {
  int reading   = 0;
  bool debounce = 0;

  for (int i = 0; i < NUM_CHECKS; i++) {
    reading += digitalRead(button);
    delay(CHECK_DELAY);
  }

  return debounce = (reading == NUM_CHECKS) ? 1 : 0;
}

// LED on/off function
void LED_set(intensity_t intensity) {
  char red;
  char green;
  char blue;

  switch (intensity) {
    case OFF:
      red   = 0;
      green = 0;
      blue  = 0;
      break;
    case FULL:
      red   = RED;
      green = GREEN;
      blue  = BLUE;
      break;
    case HALF:
      red   = RED/2;
      green = GREEN/2;
      blue  = BLUE/2;
      break;
    case THREE_QUARTER:
      red   = RED*0.75;
      green = GREEN*0.75;
      blue  = BLUE*0.75;
      break;
    default:
      red   = 0;
      green = 0;
      blue  = 0;
      break;
  }

  for (int i = 0; i < NUMPIXELS; i++) {       // Set LEDs
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
  }
  pixels.show();
}



// State functions
state_t state_off(void) {
  state_t next_state = STATE_OFF;
  bool button_press = 0;

  // Change state to either STATE_ON or STATE_LIFTOFF
  LED_set(FULL);                                 // Turn on LEDs
  
  // Check if button is long-pressed
  delay(LIFTOFF_BUTTON_WAIT);
  button_press = debounce(BUTTON_PIN);

  if (button_press == 0) {                    // If button was regular-pressed
    next_state = STATE_ON;
  }
  else {                                      // If button was long-pressed
    liftoff_routine();
    next_state = STATE_LIFTOFF;
  }

  return next_state;
}

state_t state_on(void) {
  state_t next_state = STATE_ON;

  // Change state to STATE_OFF
  LED_set(OFF);
  next_state = STATE_OFF;
  return next_state;
}

state_t state_liftoff(void) {
  state_t next_state = STATE_LIFTOFF;

  liftoff_routine();
  LED_set(OFF);

  // Change state to STATE_OFF
  next_state = STATE_OFF;
  return next_state;
}



// Liftoff routine
void liftoff_routine(void) {
  for (int j = 0; j < random(LIFTOFF_ITR_MIN, LIFTOFF_ITR_MAX); j++) {
    // LED_set(HALF);
    LED_set(THREE_QUARTER);
    delay(random(LIFTOFF_OFF_MIN, LIFTOFF_OFF_MAX));

    LED_set(FULL);
    delay(random(LIFTOFF_ON_MIN, LIFTOFF_ON_MAX));
  }
}