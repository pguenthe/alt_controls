const int NUM_BUTTONS = 4;
const int buttonPins[] = {A0, A1, A2, A3};
boolean buttonPressed[] = {false, false, false, false};
int pressedValue = 20;
int releasedValue = 50;


int hapticMotor1 = 3; //PWM pins 3, 5, 6, 9-12
int hapticMotor2 = 5;
byte outputLevel = 255;


unsigned long loopCount = 0;

void setup() {
  Serial.begin(57600);
  delay(2000);

  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP); 
    Serial.print("Listening to button ");
    Serial.println(i);
  }
  
  Serial.println("Program initializing...");
}

void loop() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    int sensorValue = analogRead(buttonPins[i]);

    if (!buttonPressed[i] && sensorValue <= pressedValue) {
      Serial.print("Button ");
      Serial.print(i);
      Serial.println(" pressed!");
      Serial.println(loopCount);
//      Serial.print("Value: ");
//      Serial.println(sensorValue);
      buttonPressed[i] = true;
    }

    if (buttonPressed[0] || buttonPressed[1]) {
      analogWrite(hapticMotor1, outputLevel);
    } else {
      analogWrite(hapticMotor1, 0);
    }

    if (buttonPressed[2] || buttonPressed[3]) {
      analogWrite(hapticMotor2, outputLevel);
    }else {
      analogWrite(hapticMotor2, 0);
    }
  
    if (buttonPressed[i] && sensorValue >= releasedValue) {
      Serial.print("Button ");
      Serial.print(i);
      Serial.println(" released!");
      Serial.println(loopCount);
//      Serial.print("Value: ");
//      Serial.println(sensorValue);
      buttonPressed[i] = false;
    }
  }
  
  loopCount++;
}
