//incorporates code from https://www.benripley.com/diy/arduino/three-ways-to-read-a-pwm-signal-with-arduino/
#include <Joystick.h>

Joystick_ Joystick;

const bool DEBUG = false;

const byte firstButtonPin = 5;
const byte lastButtonPin = 9;

const byte BASE_PIN = 2;
const byte BASE_INTERRUPT = 1; //note the order is switched on interrupts per leonardo pinout!
const int base_range = 50; //unit 1: 35 / unit 2: 50
const int base_center = 724; //unit 1:733  / unit 2: 728

const byte SEAT_PIN = 3;
const byte SEAT_INTERRUPT = 0; //note the order is switched on interrupts per leonardo pinout!
const int seat_range = 60;
const int seat_center = 685; //unit 1:708  / unit 2: 660

//volatile variables for interrupt changes
volatile int base_value = base_center;
volatile int base_prev_time = 0;
volatile int seat_value = seat_center;
volatile int seat_prev_time = 0;

const int NUM_BUTTONS = 3;
const int buttonPins[] = {9, 8, 7};
const int buttonMappings[] = {9, 6, 4}; //start, dash left, activate
boolean buttonPressed[] = {false, false, false, false};

void setup() {
  Serial.begin(57600);
  
  attachInterrupt(BASE_INTERRUPT, rising_base, RISING);
  attachInterrupt(SEAT_INTERRUPT, rising_seat, RISING);

  Joystick.begin();
//  Joystick.setXAxisRange(-1, 1);
//  Joystick.setYAxisRange(-1, 1);
 Joystick.setXAxisRange(seat_center - seat_range, seat_center + seat_range);
  Joystick.setYAxisRange(base_center - base_range, base_center + base_range);

  initializeButtons();
  
  if (DEBUG) Serial.println ("Initialized!");
}

void loop() {
  checkButtons();
  Joystick.setYAxis(base_value);
  Joystick.setXAxis(seat_value);  
  
  if (DEBUG) {
    Serial.print ("Base value: ");
    Serial.println (base_value);
    Serial.print ("Seat value: ");
    Serial.println(seat_value);
    delay(250);
  }
  
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
//
//  if (DEBUG) {
//    Serial.print("Base on interrupt: ");
//    Serial.println(base_value);
//  }
}

void initializeButtons() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP); 
    Serial.print("Listening to button ");
    Serial.println(i);
  }
}

void checkButtons() {
    for (int i = 0; i < NUM_BUTTONS; i++) {
    int sensorValue = digitalRead(buttonPins[i]);

    if (!buttonPressed[i] && sensorValue == LOW) {
      if (DEBUG) {
      Serial.print("Button ");
      Serial.print(i);
      Serial.println(" pressed!");

      Serial.print("Button ");
      Serial.print(buttonMappings[i]);
      Serial.println(" sent!");
      }
      buttonPressed[i] = true;
    }

    if (buttonPressed[i] && sensorValue == HIGH) {
      buttonPressed[i] = false;
    }

    Joystick.setButton(buttonMappings[i] - 1, buttonPressed[i]); 
  }
//    if (DEBUG) {
//      Serial.println();
//    }
}
