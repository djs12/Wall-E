

/*
 * This is the main file to control the arduino slave
 */

#include <Wire.h>
#include <NewPing.h>
#define maxDistance 200

//Defining the pins for the back sensors
#define triggerPin1 11
#define echoPin1 10

#define triggerPin2 9
#define echoPin2 8

#define triggerPin3 7
#define echoPin3 6

//Defining the pins for the front sensors
#define triggerPin4 5
#define echoPin4 4

#define triggerPin5 3
#define echoPin5 2

#define triggerPin6 A0
#define echoPin6 A1


//create the distance sensor objects
NewPing DistanceSensor1(triggerPin1,echoPin1,maxDistance);
int16_t dis1 = 0;

NewPing DistanceSensor2(triggerPin2,echoPin2,maxDistance);
int16_t dis2 = 0;

NewPing DistanceSensor3(triggerPin3,echoPin3,maxDistance);
int16_t dis3 = 0;

NewPing DistanceSensor4(triggerPin4,echoPin4,maxDistance);
int16_t dis4 = 0;

NewPing DistanceSensor5(triggerPin5,echoPin5,maxDistance);
int16_t dis5 = 0;

NewPing DistanceSensor6(triggerPin6,echoPin6,maxDistance);
int16_t dis6 = 0;

unsigned long echoTime = 0;
int var = 1;

void setup() {
  Wire.begin(1);//slave at address 1
  Wire.onRequest(requestEvent);
  //Serial.begin(9600);
  
}

void loop() {

  dis1 = DistanceSensor1.ping_cm();
  dis2 = DistanceSensor2.ping_cm();
  dis3 = DistanceSensor3.ping_cm();
  dis4 = DistanceSensor4.ping_cm();
  dis5 = DistanceSensor5.ping_cm();
  dis6 = DistanceSensor6.ping_cm();
 delay(100);

//  if ( millis() > echoTime ){
//    echoTime += 10;
//
//    switch (var){
//      case 1:
//        dis1 = DistanceSensor1.ping_cm();
//        var = 2;
//        break;
//      case 2:
//        dis2 = DistanceSensor2.ping_cm();
//        var = 3;
//        break;
//      case 3:
//        dis3 = DistanceSensor3.ping_cm();
//        var = 4;
//        break;
//      case 4:
//        dis4 = DistanceSensor4.ping_cm();
//        var = 5;
//        break;
//      case 5:
//        dis5 = DistanceSensor5.ping_cm();
//        var = 6;
//        break;
//      case 6:
//        dis6 = DistanceSensor6.ping_cm();
//        var = 1;
//        break;
//        
//      
//    }
//  }
  
  

}

void requestEvent() {
//  int16_t cm1 = dis1;
//  int16_t cm2 = dis2;
//  //can only send 1 byte at a time, so break cm into two bytes
//  byte myArray[4];
//  myArray[0] = (cm1>>8 & 255);//where 255 is 11111111
//  myArray[1] = cm1 & 255;
//  myArray[2] = (cm2>>8 & 255);
//  myArray[3] = cm2 & 255;
//
//  //sending myArray[0] first which contains the higher bytes
//  Wire.write(myArray,4);




  int16_t cm1 = dis1;
  int16_t cm2 = dis2;
  int16_t cm3 = dis3;
  int16_t cm4 = dis4;
  int16_t cm5 = dis5;
  int16_t cm6 = dis6;
  //can only send 1 byte at a time, so break cm into two bytes
  byte myArray[12];
  myArray[0] = (cm1>>8 & 255);//where 255 is 11111111
  myArray[1] = cm1 & 255;
  myArray[2] = (cm2>>8 & 255);
  myArray[3] = cm2 & 255;
  myArray[4] = (cm3>>8 & 255);
  myArray[5] = cm3 & 255;
  myArray[6] = (cm4>>8 & 255);
  myArray[7] = cm4 & 255;
  myArray[8] = (cm5>>8 & 255);
  myArray[9] = cm5 & 255;
  myArray[10] = (cm6>>8 & 255);
  myArray[11] = cm6 & 255;
  Wire.write(myArray,12);
  

  
  
  
  
}

