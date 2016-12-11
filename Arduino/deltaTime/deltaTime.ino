unsigned long printTime = 0;


void setup() {
  Serial.begin(9600);
  

}

void loop() {
  if ( millis() > printTime){
    printTime += 1000;
    Serial.println("every second");
  }
   

}
