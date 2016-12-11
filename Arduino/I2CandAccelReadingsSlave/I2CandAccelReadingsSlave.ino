
#include <Wire.h>

int i = 0;

void setup() {
  Wire.begin(1);
  Wire.onRequest(requestEvent);

}

void loop() {
  

}

void requestEvent(){
  String c = "kickingAss";
  
  Wire.write(c[i]);
  i += 1;
}

