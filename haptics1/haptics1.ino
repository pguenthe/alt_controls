int hapticMotor1 = 3; //PWM pins 3, 5, 6, 9-12
int hapticMotor2 = 5;
byte outputLevel = 255;
byte increment = 32;

void setup()
{
  pinMode(hapticMotor1, OUTPUT);
  pinMode(hapticMotor2, OUTPUT);
}

void loop()
{
  analogWrite(hapticMotor1, outputLevel);
  delay(300); 
  analogWrite(hapticMotor1, 0); 
  analogWrite(hapticMotor2, outputLevel);
  delay(300);
  analogWrite(hapticMotor2, 0); 
  delay(300);

  outputLevel -= increment;
  if(outputLevel < 64) {
    outputLevel = 255;
  }
}
