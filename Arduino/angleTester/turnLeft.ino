//turn the car left

void turnLeft() {
  
  //spin motorA forwards at full speed
  digitalWrite(12,HIGH);//set direction forwards
  digitalWrite(9,LOW);//turn brake off
  analogWrite(3,255);//at full speed

  //spin motorB backwards at half speed
  digitalWrite(13,LOW);
  digitalWrite(8,LOW);
  analogWrite(11,127);

  //tone(speakerPin, NOTE_C4);
  
}

