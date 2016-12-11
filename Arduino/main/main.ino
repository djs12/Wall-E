


/*I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg*/


#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 mpu;

#define OUTPUT_READABLE_YAWPITCHROLL

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector


volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}







#include "pitches.h"
#include <Wire.h>

/*
 * This is the main file to run Wall-E
 * 
 * 
 * Pins controlled by motor shield
 * Function         Channel A   Channel B
  Direction         Digital 12  Digital 13
  Speed (PWM)       Digital 3   Digital 11
  Brake             Digital 9   Digital 8
  Current Sensing   Analog 0    Analog 1


  MotorA is the right motor when looking from the back(side w/o battery)
  MotorB is the left
 */

//Creating the states for the car
enum State{
  stopped,         //0
  backwards,       //1
  forwards,        //2
  turningRight,    //3
  turningLeft,     //4
//  dodgingRight,      //5
//  dodgingLeft,       //6
  
}

//initialize the first state as stopped
state = stopped;

//initialize the distance readings
int16_t dis1;//middle back sensor
int16_t dis2;//right back sensor
int16_t dis3;//left back sensor
int16_t dis4;//middle front sensor
int16_t dis5;//right front sensor
int16_t dis6;//left front sensor

//create global variable to hold angle readings
float angle;

//condition
bool obstacle = 0;
bool dodgingRight = 0;
bool dodgingLeft = 0;
unsigned long accumulator;

//initialize the pin for pwm of the speaker
const int speakerPin = 5;

unsigned long initialWaitTime = 5000;

bool waiting = 1;
const int waitSignal = 6;
unsigned long printTime = 0;

void setup() {
  
  
  Wire.begin();//master

  //Setup MotorA right motor
  pinMode(12,OUTPUT);
  pinMode(9,OUTPUT);

  //Setup MotorB left motor
  pinMode(13,OUTPUT);
  pinMode(8,OUTPUT);

  //tone(speakerPin,NOTE_C4,10000);
  pinMode(waitSignal,OUTPUT);
  digitalWrite(waitSignal,HIGH);

  

// join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
      TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif

  Serial.begin(115200);
  
  // initialize device
  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
  //verify connection
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  // load and configure the DMP
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();
  
  // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(86);
    mpu.setYGyroOffset(-35);
    mpu.setZGyroOffset(19);
    mpu.setZAccelOffset(4897);

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

}

void loop() {
  
  // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
        // other program behavior stuff here
        // .
        // .
        // .

        //get the sensor readings and store them
        getDistance();
        
        if( millis() > initialWaitTime ){
        
        
        digitalWrite(waitSignal,LOW);
        switch(state){
  
          case stopped:
            //action
            brake();

            

            if( (1 < dis1 && dis1 < 30) || (1<dis2&&dis2<30) || (1<dis3&&dis3<30) ) {
              //if true, then there is a foot close to one of the back sensors

              //set state to forwards
              state = forwards;
              
            }else {
              //No feet have been detected, stop!
              //remain stopped until a foot comes closer
              state = stopped;
            }

            

            break;
      
          case backwards:
            
            
            break;
      
          case forwards:
            //action
            if ( dodgingRight || dodgingLeft ){
              if ( dodgingRight ){
                dodgeRight();
                if ( millis() - accumulator > 5000 ){
                  obstacle = 0;//we will have passed the obstacle
                  dodgingRight = 0;//finished dodging after 5 seconds
                  state = turningLeft;//get set to even out
                }
              }

              if ( dodgingLeft ){
                dodgeLeft();
                if ( millis() - accumulator > 5000 ){
                  obstacle = 0;//we will have passed the obstacle
                  dodgingLeft = 0;//finished dodging after 5 seconds
                  state = turningRight;//get set to even out
                }
              }
              
            }else {
              goStraight();
            }
            
            if( (1 < dis1 && dis1 < 30) || (1<dis2&&dis2<30) || (1<dis3&&dis3<30) ) {
              //if true, then there is a foot reasonably close by in the back

              //if there is an object close enough to the left, turn right
              if( 1 < dis6 < 30){
                obstacle = 1;
                state = turningRight;
                dodgingRight = 1;
              
              
              //if there is an object close enough to the right, turn left
              }else if( 1 < dis5 < 30 ){
                obstacle = 1;
                state = turningLeft;
                dodgingLeft = 1;
                
              }else {
                //else there are no objects ahead, keep going in the same direction
                state = forwards;
              }



              
            }else {
              //No feet have been detected, stop!
              //remain stopped until a foot comes closer
              state = stopped;
            }
      
             
             break;
      
          case turningRight:
              if(obstacle){
                if(angle<-45){
                  state=forwards;
                  accumulator = millis();  
                }
                turnRight();
              }
              else{
                if(angle<0){
                  state=forwards;
                }
                turnRight();
              }


          
//            //if the angle is past 45 degrees, then the car has completed the turn
//            if( angle < -45 ){
//              //if we are dodging an obstacle 
//              if ( obstacle ) {
//                state = forwards;
//                accumulator = millis();//we are about to start dodging an obstacle, set the timer
//              }else {
//                //else we have straightened out, set the state back to forwards!
//                state = forwards;
//              }
//              
//            }else {
//              //we have not completed enough rotation yet, keep turning right
//              turnRight();
//              
//            }
            
            break;
      
          case turningLeft:
          if(obstacle){
            if(angle>45){
              state=forwards;
              accumulator = millis();  
            }
            turnLeft();
          }
          else{
            if(angle>0){
              state=forwards;
            }
            turnLeft();
          }
//            //if the angle is past -45 degrees, then the car has completed the turn
//            if( angle >45 ){
//              //if we are dodging an obstacle
//              if ( obstacle ) {
//                state = forwards;
//                //we are about to start dodging an obstacle, set the timer
//              }else {
//                //else we have straigthened out, set the state back to forwards!
//                state = forwards;
//              }
//            }else{
//              turnLeft();
//              
//            }
            
            break;


      
          
        }//end of switch statement

        
        }else{
          Serial.println("              Waiting");
        }


    }//end of where main code is supposed to go

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        

        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
//            Serial.print("ypr\t");
//            Serial.print(ypr[0] * 180/M_PI);
//            Serial.print("\t");
//            Serial.print(ypr[1] * 180/M_PI);
//            Serial.print("\t");
//            Serial.println(ypr[2] * 180/M_PI);

            //my Edit to store the yaw angle value
            angle = ypr[0] * 180/M_PI;
        #endif

    }

}


void getDistance() {
//  Wire.requestFrom(1,4);
//  dis1 = Wire.read();//get higher byte
//  dis1 = dis1 << 8;//shift to appropriate spot
//  dis1 |= Wire.read();//combine
//  Serial.print("                                          First sensor: ");
//  Serial.print(dis1);
//  Serial.print("  ");
//  dis2 = Wire.read();
//  dis2 = dis2 << 8;
//  dis2 |= Wire.read();
//  Serial.print("Second sensor: ");
//  Serial.println(dis2);



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

  if( millis() > printTime) {
    printTime += 1000;
    Serial.print("ypr\t");
    Serial.print(ypr[0] * 180/M_PI);
    Serial.print("\t");
    Serial.print(ypr[1] * 180/M_PI);
    Serial.print("\t");
    Serial.println(ypr[2] * 180/M_PI);

    Serial.print("                                          First sensor: ");
    Serial.print(dis1);
    Serial.print(",2nd: ");
    Serial.print(dis2);
    Serial.print(",3rd: ");
    Serial.print(dis3);
    Serial.print(",4th: ");
    Serial.print(dis4);
    Serial.print(",5th: ");
    Serial.print(dis5);
    Serial.print(",6th: ");
    Serial.println(dis6);
  }
  
  


  
}








