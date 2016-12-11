//turn the car right

void turnRight() {
  
  //spin motorA backwards at half speed
  digitalWrite(12,LOW);//set direction backwards
  digitalWrite(9,LOW);//turn brake off
  analogWrite(3,127);//at half speed

  //spin motorB forwards at full speed
  digitalWrite(13,HIGH);
  digitalWrite(8,LOW);
  analogWrite(11,255);
}

