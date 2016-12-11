//make the car dodge right


//make the car go straight


void dodgeRight() {
  
  //if the car is mostly on track 
  if( -55 <= angle && angle <= -35  ){
    //keep spinning both wheels at full speed

    //spin motorA forwards at full speed
    digitalWrite(12,HIGH);//set direction forwards
    digitalWrite(9,LOW);//turn brake off
    analogWrite(3,255);
  
    //spin motorB forwards at full speed
    digitalWrite(13,HIGH);
    digitalWrite(8,LOW);
    analogWrite(11,255);
    
    
  }else if( angle > -35 ) {
    //if true, then the car is angling too far to the left.
    //adjust by turning right slightly

    //spin motorA forwards at a slightly slower pace than full speed
    digitalWrite(12,HIGH);//set direction forwards
    digitalWrite(9,LOW);//turn brake off
    analogWrite(3,230);
  
    //spin motorB forwards at full speed
    digitalWrite(13,HIGH);
    digitalWrite(8,LOW);
    analogWrite(11,255);

    
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
    analogWrite(11,230);
    
  }



  
}

