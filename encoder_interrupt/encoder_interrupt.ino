//incorporates code from https://www.benripley.com/diy/arduino/three-ways-to-read-a-pwm-signal-with-arduino/
#include <Joystick.h>

Joystick_ Joystick;

const bool DEBUG = true;

const byte firstButtonPin = 5;
const byte lastButtonPin = 9;

const byte BASE_PIN = 2;
const byte BASE_INTERRUPT = 1; //note the order is switched on interrupts per leonardo pinout!
const int base_range = 30;
const int base_center = 733;

const byte SEAT_PIN = 3;
const byte SEAT_INTERRUPT = 0; //note the order is switched on interrupts per leonardo pinout!
const int seat_range = 50;
const int seat_center = 708;

//volatile variables for interrupt changes
volatile int base_value = base_center;
volatile int base_prev_time = 0;
volatile int seat_value = seat_center;
volatile int seat_prev_time = 0;

void setup() {
  if (DEBUG) Serial.begin(57600);
  
  // Initialize Button Pins
  for (int i = firstButtonPin; i <= lastButtonPin; i++) {
    pinMode(i, INPUT_PULLUP); 
  }
  
  attachInterrupt(BASE_INTERRUPT, rising_base, RISING);
  attachInterrupt(SEAT_INTERRUPT, rising_seat, RISING);

  Joystick.begin();
//  Joystick.setXAxisRange(-1, 1);
//  Joystick.setYAxisRange(-1, 1);
 Joystick.setXAxisRange(seat_center - seat_range, seat_center + seat_range);
  Joystick.setYAxisRange(base_center - base_range, base_center + base_range);
  
  if (DEBUG) Serial.println ("Initialized!");
}

void loop() {
  for (int i = firstButtonPin; i <= lastButtonPin; i++) {
    bool state = checkButton(i);
    Joystick.setButton(i - firstButtonPin, state);
    if (DEBUG && state) {
      Serial.print("Button ");
      Serial.print(i - firstButtonPin);
      Serial.println(" pressed");
    }
  }

//  float base_scaled = (base_value - base_center) / (float)base_range;
//  if (base_scaled < -1)  {
//    base_scaled = -1;
//  } else if (base_scaled > 1) {
//    base_scaled = 1;
//  }
  Joystick.setYAxis(base_value);

//  float seat_scaled = (seat_value - seat_center) / (float)seat_range;
//  if (seat_scaled < -1)  {
//    seat_scaled = -1;
//  } else if (seat_scaled > 1) {
//    seat_scaled = 1;
//  }
  Joystick.setXAxis(seat_value);  
  
  if (DEBUG) {
    Serial.print ("Base value: ");
    Serial.println (base_value);
    Serial.print ("Seat value: ");
    Serial.println(seat_value);
    delay(250);
  }
  
}

bool checkButton(byte pin) {
  int buttonState = !digitalRead(pin);
  return buttonState;
}

void rising_seat() {
  attachInterrupt(SEAT_INTERRUPT, falling_seat, FALLING);
  seat_prev_time = micros();
}
 
void falling_seat() {
  attachInterrupt(SEAT_INTERRUPT, rising_seat, RISING);
  seat_value = micros()-seat_prev_time;

//  if (DEBUG) {
//    Serial.print("Seat on interrupt: ");
//    Serial.println(seat_value);
//  }
}

void rising_base() {
  attachInterrupt(BASE_INTERRUPT, falling_base, FALLING);
  base_prev_time = micros();
}
 
void falling_base() {
  attachInterrupt(BASE_INTERRUPT, rising_base, RISING);
  base_value = micros()-base_prev_time;

//  if (DEBUG) {
//    Serial.print("Base on interrupt: ");
//    Serial.println(base_value);
//  }
}
