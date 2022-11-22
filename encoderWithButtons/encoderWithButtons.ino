#include <Joystick.h>

Joystick_ Joystick;

const bool DEBUG = false;

const byte BASE_PIN = 2; 
float base_value;
const int base_range = 30;
const int base_center = 720;

const byte SEAT_PIN = 3; 
float seat_value;
const int seat_range = 50;
const int seat_center = 860;

const int NUM_BUTTONS = 4;
const int buttonPins[] = {A0, A1, A2, A3};
boolean buttonPressed[] = {false, false, false, false};
int pressedValue = 100;
int releasedValue = 200;

void setup() {
  Serial.begin(57600);
  
  // Initialize Button Pins
//  pinMode(firstButtonPin, INPUT_PULLUP); 

  //Initialize PWM Pins
  pinMode(BASE_PIN, INPUT);
  pinMode(SEAT_PIN, INPUT);

  //Initialize Analog pins for pressure sensors
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP); 
    Serial.print("Listening to button ");
    Serial.println(i);
  }

  Joystick.begin();

  Joystick.setXAxisRange(-1, 1);
  Joystick.setYAxisRange(-1, 1);
  
  Serial.println ("Initialized!");
}

void loop() {
  checkButtons();


  if (DEBUG) Serial.print ("Base value: ");
  base_value = checkEncoder(BASE_PIN, base_center, base_range);  
  Joystick.setYAxis(base_value);
  if (DEBUG) Serial.println (base_value);

  if (DEBUG) Serial.print ("Seat value: ");
  seat_value = checkEncoder(SEAT_PIN, seat_center, seat_range);
  Joystick.setXAxis(seat_value);  
  if (DEBUG) Serial.println(seat_value);
  
   if (DEBUG) delay(250);
//  Serial.println("Loooping...");

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

float checkEncoder (byte pin, int center, int range ) {
  int result = pulseIn(pin, HIGH);
  if (DEBUG) {
    Serial.print("\t (Raw: ");
    Serial.print(result);
    Serial.print(")\t");
  }
  float scaled = (result - center) / (float)range;
  if (scaled < -1)  {
    scaled = -1;
  } else if (scaled > 1) {
    scaled = 1;
  }

  return scaled;
}
