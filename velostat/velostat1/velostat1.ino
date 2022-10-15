const int button1pin = A0;
int pressedValue = 100;
int releasedValue = 200;
boolean button1pressed = false;

unsigned long loopCount = 0;

void setup() {
  Serial.begin(57600);

  pinMode(A0, INPUT_PULLUP); 
  delay(100);
  
  Serial.println("Program initializing...");
  Serial.println("Listening to button 1");
}

void loop() {
  int sensorValue = analogRead(button1pin);

  if (!button1pressed && sensorValue <= pressedValue) {
    Serial.println("Button press!");
    Serial.println(loopCount);
    Serial.print("Value: ");
    Serial.println(sensorValue);
    button1pressed = true;
  }
  
  if (button1pressed && sensorValue >= releasedValue) {
    Serial.println("Button release!");
    Serial.println(loopCount);
    Serial.print("Value: ");
    Serial.println(sensorValue);
    button1pressed = false;
  }

  loopCount++;
//  Serial.print("Button 1:");
//  Serial.println(sensorValue);
}
