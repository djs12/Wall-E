
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
  
  Note: calibration code is adapted from Joop Brooking's code which can be found on his website http://www.brokking.net/imu.html
  
 */

#include <Wire.h>
#include "pitches.h"
//Declaring some global variables
int gyro_x, gyro_y, gyro_z;
long acc_x, acc_y, acc_z, acc_total_vector;
int temperature;
long gyro_x_cal, gyro_y_cal, gyro_z_cal;
long loop_timer;
int lcd_loop_counter;
float angle_pitch, angle_roll,angle_yaw;
int angle_pitch_buffer, angle_roll_buffer;
boolean set_gyro_angles;
float angle_roll_acc, angle_pitch_acc;
float angle_pitch_output, angle_roll_output, angle_yaw_output;
unsigned long printTimer = 10000;
float myAngle;

////initialize the distance readings
//int16_t dis1;//middle back sensor
//int16_t dis2;//right back sensor
//int16_t dis3;//left back sensor
//int16_t dis4;//middle front sensor
//int16_t dis5;//right front sensor
//int16_t dis6;//left front sensor

//initialize the distance readings
int16_t dis1;//middle back sensor
int16_t dis3;//right back sensor
int16_t dis2;//left back sensor
int16_t dis4;//middle front sensor
int16_t dis5;//right front sensor
int16_t dis6;//left front sensor

const int speakerPin = 5;
const int d=30;
float turn_angle=0;
boolean feet;

const int statusPin = 10;
unsigned long statusPrint = 6000;
bool statusLight = 1;

unsigned long beepTime = 6000;
bool beeping = 0;

unsigned long sampleRate = 0;
int count=0;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  setup_mpu_6050_registers();    //Setup the registers of the MPU-6050 (500dfs / +/-8g) and start the gyro
  pinMode(6,OUTPUT);
  digitalWrite(6,HIGH);
  

  delay(1500);

  Serial.println("Calibrating gyro");
  for (int cal_int = 0; cal_int < 2000 ; cal_int ++){                  //Run this code 2000 times
    //if(cal_int % 125 == 0)Serial.println(".");                              //Print a dot on the LCD every 125 readings
    read_mpu_6050_data();                                              //Read the raw acc and gyro data from the MPU-6050
    gyro_x_cal += gyro_x;                                              //Add the gyro x-axis offset to the gyro_x_cal variable
    gyro_y_cal += gyro_y;                                              //Add the gyro y-axis offset to the gyro_y_cal variable
    gyro_z_cal += gyro_z;                                              //Add the gyro z-axis offset to the gyro_z_cal variable
    delay(3);                                                          //Delay 3us to simulate the 250Hz program loop
    //Serial.println(gyro_x_cal);
  }

  gyro_x_cal /= 2000;                                                  //Divide the gyro_x_cal variable by 2000 to get the avarage offset
  gyro_y_cal /= 2000;                                                  //Divide the gyro_y_cal variable by 2000 to get the avarage offset
  gyro_z_cal /= 2000;                                                  //Divide the gyro_z_cal variable by 2000 to get the avarage offset

  digitalWrite(6,LOW);
  Serial.println("made it here");

  loop_timer = micros();                                               //Reset the loop timer

  //Setup MotorA right motor
  pinMode(12,OUTPUT);
  pinMode(9,OUTPUT);

  //Setup MotorB left motor
  pinMode(13,OUTPUT);
  pinMode(8,OUTPUT);

  //turn brakes on 
  digitalWrite(9,HIGH);
  digitalWrite(8,HIGH);
  digitalWrite(9,HIGH);

  sampleRate = micros();
  
}


void loop() {
  //sampling at 250Hz
  if ( micros() > sampleRate ){
   sampleRate += 4000;
  
  read_mpu_6050_data();                                                //Read the raw acc and gyro data from the MPU-6050

  gyro_x -= gyro_x_cal;                                                //Subtract the offset calibration value from the raw gyro_x value
  gyro_y -= gyro_y_cal;                                                //Subtract the offset calibration value from the raw gyro_y value
  gyro_z -= gyro_z_cal;                                                //Subtract the offset calibration value from the raw gyro_z value

  //Gyro angle calculations
  //0.0000611 = 1 / (250Hz / 65.5)
  angle_pitch += gyro_x * 0.0000611;                                   //Calculate the traveled pitch angle and add this to the angle_pitch variable
  angle_roll += gyro_y * 0.0000611;                                    //Calculate the traveled roll angle and add this to the angle_roll variable
  angle_yaw += gyro_z * 0.0000611; 

  //To dampen the pitch and roll angles a complementary filter is used
  angle_pitch_output = angle_pitch_output * 0.9 + angle_pitch * 0.1;   //Take 90% of the output pitch value and add 10% of the raw pitch value
  angle_roll_output = angle_roll_output * 0.9 + angle_roll * 0.1;      //Take 90% of the output roll value and add 10% of the raw roll value
  angle_yaw_output = angle_yaw_output * 0.9 + angle_yaw * 0.1;      //Take 90% of the output roll value and add 10% of the raw roll value




  //angle and distance
  myAngle = angle_yaw_output/2.0;
  getDistance();
  
  //Main code
  if( (dis1 > d || dis1==0) && (dis2 > d || dis2==0) && (dis3 > d || dis3==0) ){
    //feet are too far away, brake!
    brake();
   
  }else {
    //feet are in the back, we can do stuff
   
  //no obstacle in the front
    if (      (dis6 == 0 || dis6 > d) &&  (dis4 == 0 || dis4 > d) &&   (dis5 == 0 || dis5 > d)      ){
      
      if(count==0){
        //move straight after turning
        turn_angle=myAngle;
        goStraight();
        count+=1;
        noTone(speakerPin);
      }
      else{
        
        //the car is facing right
        if(myAngle>5){
          turnLeft();
        }
        //the car is facing left
        else if(myAngle<-5){
          turnRight();
        }
        //if the angle is between -5 and 5, go straight
        else{
          turn_angle=0;
          goStraight();
        }
      }
     
    }
     //end no obstacle

     //obstacle appear
     else{
        //the obstacle is on the left
        if(dis6<d && (dis5>dis6 || dis5==0)){
          turnRight();
          tone(speakerPin,NOTE_B0);
          count=0;
        }
        //the obstacle is on the right
        else if(dis5<d && (dis6>dis5 || dis6==0)){
          turnLeft();
          tone(speakerPin,NOTE_DS8);
          count=0;
          
        }
        //long wall
        else if(dis4<dis5 && dis4<dis6 && dis4<d){
          //closer to left
          if(dis5<dis6){
            turnRight();
            count=0;
          }
          //closer to right
          else{
            turnLeft();
            count=0;
          }
        }
        //small obstacle ( small as 5cm )
        else if(dis4<d && dis5==0 && dis6==0){
          //default set to turn right
          turnRight();
          count=0;
        }
     }
     //end obstacle appear
  }
  
  
  }//end of sample loop




   //use the speaker to constantly make a short noise every 3 seconds
   if ( millis() > beepTime ){
      //If we just made a noise, knock it off!
      if ( beeping ){
        beepTime += 2700;   //no noise for a period of 2.7 seconds
        noTone(speakerPin);
        beeping = 0;  
      }else {
        //Make some noise!
        beepTime += 300;    //sound for a period of .3 seconds
        tone(speakerPin,NOTE_D4);
        beeping = 1;
      }
   }





  
   //Continually flash the status LED every second
   if( millis() > statusPrint ){
    statusPrint += 1000;
    digitalWrite(statusPin,statusLight);
    statusLight = !statusLight;
   }

   
} //end of void loop

