

void getDistance(){
  Wire.requestFrom(1,12);
  dis1 = Wire.read();
  dis1 = dis1 << 8;
  dis1 |= Wire.read();
  
  
  
  dis2 = Wire.read();
  dis2 = dis2 << 8;
  dis2 |= Wire.read();
  
  
  
  dis3 = Wire.read();
  dis3 = dis3 << 8;
  dis3 |= Wire.read();
  
  
  
  dis4 = Wire.read();
  dis4 = dis4 << 8;
  dis4 |= Wire.read();
  

  
  dis5 = Wire.read();
  dis5 = dis5 << 8;
  dis5 |= Wire.read();
  

  
  dis6 = Wire.read();
  dis6 = dis6 << 8;
  dis6 |= Wire.read();
  
  
//    Serial.print("                                          First sensor: ");
//    Serial.print(dis1);
//    Serial.print(",2nd: ");
//    Serial.print(dis2);
//    Serial.print(",3rd: ");
//    Serial.print(dis3);
//    Serial.print(",4th: ");
//    Serial.print(dis4);
//    Serial.print(",5th: ");
//    Serial.print(dis5);
//    Serial.print(",6th: ");
//    Serial.println(dis6);
}

