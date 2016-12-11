
//make the car dodge left


void dodgeLeft() {
  
  //if the car is mostly on track 
  if(-35 <= myAngle && myAngle <= -15){
    //keep spinning both wheels at full speed

    //spin motorA forwards at full speed
    digitalWrite(12,HIGH);//set direction forwards
    digitalWrite(9,LOW);//turn brake off
    analogWrite(3,255);
  
    //spin motorB forwards at full speed
    digitalWrite(13,HIGH);
    digitalWrite(8,LOW);
    analogWrite(11,255);
    digitalWrite(6,LOW);//feedback
    
  }else if( myAngle <-35 ) {
    //if true, then the car is angling too far to the left.
    //adjust by turning right slightly

    //spin motorA forwards at a slightly slower pace than full speed
    digitalWrite(12,HIGH);//set direction forwards
    digitalWrite(9,LOW);//turn brake off
    analogWrite(3,127);
  
    //spin motorB forwards at full speed
    digitalWrite(13,HIGH);
    digitalWrite(8,LOW);
    analogWrite(11,255);
    digitalWrite(6,HIGH);//feedback
    
  }else {
    //if true, then the car is angling too far to the right
    //adjust by turning left slightly

    //spin motorA forwards at full speed
    digitalWrite(12,HIGH);//set direction forwards
    digitalWrite(9,LOW);//turn brake off
    analogWrite(3,255);
  
    //spin motorB forwards at a slightly slower pace than full speed
    digitalWrite(13,HIGH);
    digitalWrite(8,LOW);
    analogWrite(11,127);
    digitalWrite(6,HIGH);//feedback
    
  }



  
}

