//reverse the car

void reverse(){

  //spin motorA backwards
  digitalWrite(12,LOW);//set direction backwards
  digitalWrite(9,LOW);//turn brake off
  analogWrite(3,255);

  //spin motorB backwards
  digitalWrite(13,LOW);
  digitalWrite(8,LOW);
  analogWrite(11,255);
}

