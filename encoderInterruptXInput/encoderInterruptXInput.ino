//incorporates code from https://www.benripley.com/diy/arduino/three-ways-to-read-a-pwm-signal-with-arduino/
//and XInput example code
#include <XInput.h>

// XBox ================================================================================== 
// Config Settings
const int SafetyPin = 0;  // Ground this pin to prevent inputs

// Triggers
const int TriggerMax = 255;  // uint8_t max

// Joystick Setup
const int JoyMax = 32767;  // int16_t max
// =======================================================================================

const byte BASE_PIN = 2;
const byte BASE_INTERRUPT = 1; //note the order is switched on interrupts per leonardo pinout!
const int base_range = 50;
const int base_center = 728;

const byte SEAT_PIN = 3;
const byte SEAT_INTERRUPT = 0; //note the order is switched on interrupts per leonardo pinout!
const int seat_range = 50;
const int seat_center = 660;

//volatile variables for interrupt changes
volatile int base_value = base_center;
volatile int base_prev_time = 0;
volatile float base_axis = .5;
volatile int seat_value = seat_center;
volatile int seat_prev_time = 0;
volatile float seat_axis = .5;

const bool DEBUG = false;

void setup() {
  attachInterrupt(BASE_INTERRUPT, rising_base, RISING);
  attachInterrupt(SEAT_INTERRUPT, rising_seat, RISING);

  pinMode(SafetyPin, INPUT_PULLUP);
  XInput.setAutoSend(false);  // Wait for all controls before sending

  XInput.begin();
  
  if (DEBUG) {
    Serial.begin(57600);
    Serial.println ("Initialized!");
  }
}

void loop() {
    if (digitalRead(SafetyPin) == LOW) {
    return;
  }
  
  int axis_x = JoyMax * seat_axis; 
  int axis_y = JoyMax * base_axis;
  XInput.setJoystick(JOY_LEFT, axis_x, axis_y); 
  
  XInput.send();
  
  if (DEBUG) {
    Serial.print ("Base value: ");
    Serial.println (base_value);
    Serial.print ("Seat value: ");
    Serial.println(seat_value);
  Serial.print("Base axis:");
  Serial.println(base_axis);

  Serial.print("Seat axis:");
  Serial.println(seat_axis);
  
    delay(400);
  }
  
}

void rising_seat() {
  attachInterrupt(SEAT_INTERRUPT, falling_seat, FALLING);
  seat_prev_time = micros();
}
 
void falling_seat() {
  attachInterrupt(SEAT_INTERRUPT, rising_seat, RISING);
  seat_value = micros()-seat_prev_time;

  seat_axis = (seat_value - seat_center) / (2.0 * seat_range) + 0.5;
  if (seat_axis > 1.0) {
    seat_axis = 1.0;
  } else if (seat_axis < 0) {
    seat_axis = 0;
  }

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
  
  base_axis = (base_value - base_center) / (2.0 * base_range) + 0.5;
  if (base_axis > 1.0) {
    base_axis = 1.0;
  } else if (base_axis < 0) {
    base_axis = 0;
  }

//  if (DEBUG) {
//    Serial.print("Base on interrupt: ");
//    Serial.println(base_value);
//  }
}
