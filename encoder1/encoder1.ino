#include <Joystick.h>

Joystick_ Joystick;

const bool DEBUG = false;

const byte firstButtonPin = 9;
byte lastButtonState = 0;

const byte BASE_PIN = 2; 
float base_value;
const int base_range = 30;
const int base_center = 720;

const byte SEAT_PIN = 3; 
float seat_value;
const int seat_range = 50;
const int seat_center = 860;

void setup() {
  Serial.begin(57600);
  
  // Initialize Button Pins
  pinMode(firstButtonPin, INPUT_PULLUP); 

  //Initialize PWM Pins
  pinMode(BASE_PIN, INPUT);
  pinMode(SEAT_PIN, INPUT);

  Joystick.begin();

  Joystick.setXAxisRange(-1, 1);
  Joystick.setYAxisRange(-1, 1);
  
  Serial.println ("Initialized!");
}

void loop() {
  checkButton();


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

void checkButton() {
  int currentButtonState = !digitalRead(firstButtonPin);
  
  if (currentButtonState != lastButtonState)
  {
    if (DEBUG) {
      Serial.print ("Button mode changed -- button now ");
      if (currentButtonState == 1) {
        Serial.println ("pressed");
      } else {
        Serial.println ("released");
      }
    }
    Joystick.setButton(0, currentButtonState);
    lastButtonState = currentButtonState;
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
