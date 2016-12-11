#include <Wire.h>
//Slave

int count = 0;

void setup(){
  Wire.begin(8);
  Wire.onRequest(requestEvent);
  
}

void loop(){
  delay(100);
  
}


void requestEvent() {
  //Wire.write("hello ");
  int16_t x = 1234;
  //byte myArray[2];
  //myArray[0] = (x>>8 & 255);
  //myArray[1] = x & 255;
  //Wire.write(myArray,2);
  Wire.write((x >> 8 ) & 255);
  Wire.write(x & 255);
  
}

