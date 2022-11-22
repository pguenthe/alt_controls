//incorporates code from https://www.benripley.com/diy/arduino/three-ways-to-read-a-pwm-signal-with-arduino/
#include <Joystick.h>

Joystick_ Joystick;

const bool DEBUG = false;

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

const int NUM_BUTTONS = 4;
const int buttonPins[] = {A0, A1, A2, A3};
boolean buttonPressed[] = {false, false, false, false};
int pressedValue = 100;
int releasedValue = 350;

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

//  if (DEBUG) {
//    Serial.print("Base on interrupt: ");
//    Serial.println(base_value);
//  }
}

void initializeButtons() {
    //Initialize Analog pins for pressure sensors
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP); 
    Serial.print("Listening to button ");
    Serial.println(i);
  }
}

void checkButtons() {
    for (int i = 0; i < NUM_BUTTONS; i++) {
    int sensorValue = analogRead(buttonPins[i]);

    if (!buttonPressed[i] && sensorValue <= pressedValue) {
      Serial.print("Button ");
      Serial.print(i);
      Serial.println(" pressed!");
      buttonPressed[i] = true;
    }
  
    if (buttonPressed[i] && sensorValue >= releasedValue) {
      Serial.print("Button ");
      Serial.print(i);
      Serial.println(" released!");
      buttonPressed[i] = false;
    }

    Joystick.setButton(i, buttonPressed[i]);
  }    
}
