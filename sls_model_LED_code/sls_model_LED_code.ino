#include <Adafruit_NeoPixel.h>

// Define button pin
#define BUTTON_PIN 7

// Define button debouncer values
#define NUM_CHECKS 10
#define CHECK_DELAY 2
#define DEBOUNCE_DELAY 500

// Define the pin connected to the NeoPixel strip and the number of LEDs
#define NEO_PIN 5
#define NUMPIXELS 100  // Adjust based on the number of LEDs in your strip -- Go big or go home

// Define RGB value for orange
#define RED   255
#define GREEN 35
#define BLUE  0

// Define LED intensities
typedef enum {
  OFF  = 0,
  FULL = 1,
  QUARTER,
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
  static state_t state = STATE_OFF;

  // State machine
  switch (state) {
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



// Debouncer function
bool debounce(int button) {
  int reading   = 0;
  bool debounce = 0;

  for (int i = 0; i < NUM_CHECKS; i++) {
    reading += !digitalRead(button);      // Active low button so NOT it to add up
    delay(CHECK_DELAY);
  }

  delay(DEBOUNCE_DELAY);

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
    case QUARTER:
      red   = RED*0.25;
      green = GREEN*0.25;
      blue  = BLUE*0.25;
      break;      
    case HALF:
      red   = RED*0.5;
      green = GREEN*0.5;
      blue  = BLUE*0.5;
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

  pixels.fill(pixels.Color(red, green, blue));
  pixels.show();
}



// State functions
state_t state_off(void) {
  state_t next_state = STATE_OFF;
  bool button_press = 0;

  LED_set(OFF);

  button_press = debounce(BUTTON_PIN);

  // Change state to either STATE_ON or STATE_LIFTOFF
  if (button_press) {
    LED_set(FULL);                                 // Turn on LEDs
  
    // Check if button is long-pressed
    // delay(LIFTOFF_BUTTON_WAIT);      // TEST
    button_press = debounce(BUTTON_PIN);

    if (button_press == 0) {                    // If button was regular-pressed
      next_state = STATE_ON;
    }
    else {                                      // If button was long-pressed
      next_state = STATE_LIFTOFF;
      while (digitalRead(BUTTON_PIN) == 0);    // TEST // Wait until button is unpressed (active low button)
    }
  }

  return next_state;
}

state_t state_on(void) {
  state_t next_state = STATE_ON;
  bool button_press = 0;

  LED_set(FULL);

  button_press = debounce(BUTTON_PIN);

  // Change state to STATE_OFF
  if (button_press) {
    LED_set(OFF);
    next_state = STATE_OFF;
  }

  return next_state;
}

state_t state_liftoff(void) {
  state_t next_state = STATE_LIFTOFF;
  bool button_press = 0;

  liftoff_routine();

  button_press = debounce(BUTTON_PIN);

  // Change state to STATE_OFF
  if (button_press) {
    LED_set(OFF);
    next_state = STATE_OFF;
  }

  return next_state;
}



// Liftoff routine
void liftoff_routine(void) {
  // LED_set(HALF);
  LED_set(THREE_QUARTER);
  delay(random(LIFTOFF_OFF_MIN, LIFTOFF_OFF_MAX));

  LED_set(FULL);
  delay(random(LIFTOFF_ON_MIN, LIFTOFF_ON_MAX));
}