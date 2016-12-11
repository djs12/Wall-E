

/*
 * This is the main file to run Wall-E
 * 
 * 
 * Pins controlled by motor shield
 * Function         Channel A   Channel B
  Direction         Digital 12  Digital 13
  Speed (PWM)       Digital 3   Digital 11
  Brake             Digital 9   Digital 8
  Current Sensing   Analog 0    Analog 1


  MotorA is the right motor when looking from the back(side w/o battery)
  MotorB is the left
 */


void setup() {
  pinMode(12,OUTPUT);
  pinMode(9,OUTPUT);

  //Setup MotorB left motor
  pinMode(13,OUTPUT);
  pinMode(8,OUTPUT);
  
}

void loop() {
  digitalWrite(12,HIGH);
  digitalWrite(9,LOW);
  analogWrite(3,230);

  digitalWrite(13,HIGH);
  digitalWrite(8,LOW);
  analogWrite(11,255);

  delay(1000);

}
