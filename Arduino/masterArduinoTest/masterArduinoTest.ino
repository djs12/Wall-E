#include <Wire.h>
//Master



void setup(){
  Wire.begin();
  Serial.begin(9600);
}


void loop(){
  
  delay(2000);
  int16_t bigNum;

  byte a,b;
  Wire.requestFrom(8,2);

  a = Wire.read();
  b = Wire.read();

  
  bigNum = a<< 8 | b;
  Serial.print(bigNum);
 
  
  Serial.println();
  
}

