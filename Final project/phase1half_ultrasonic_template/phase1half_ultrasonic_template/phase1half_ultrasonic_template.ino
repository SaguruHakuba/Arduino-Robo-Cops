//This is a template for Phase 0.5 
//You are free to revise this template for your robot
//Ultrasonic controlled
//Robot actions are taken in a subroutine RobotAction()
//RobotAction() takes two parameters: an ASCII word 'r', time duration in us
//Function RobotAction() is incomplete. You need to complete it.
//phase 0.5 of term project
//Revised by Jianjian Song
//Date: October 5, 2017

//Function to drive the robot forward and backward
//Function to spin the robot left and right
//Function to turn the robot left and right
//Function to open and close the gripper
//Stop function that works by command to stop the robot. 
//Function to stop the robot if a sensor detects an object 
//or obstacle in front of the robot.

#include <Servo.h>      // Include servo library
#include <NewPing.h>
Servo LeftWheel;        // Declare left servo signal
Servo RightWheel;       // Declare right servo signal
Servo Grabber;

#define GrabberControl 11
#define LeftWheelControl 12
#define RightWheelControl 13
#define STOP 1500  //microseconds
#define COUNTERCLOCKWISE  1700
#define CLOCKWISE  1300
#define GRABBING 180 //degrees to close grabber
 #define RELEASE  0   //degree to open grabber
 
#define SonarPin 5
#define MAX_DISTANCE 200
NewPing mySonar(SonarPin,SonarPin,MAX_DISTANCE);
#define STOPDISTANCE 30 // in cm

#define BaudRate 9600
int sonar_distance_cm;
void setup()
{ 
  
  Serial.begin(BaudRate);
 //define pins and attach servos
 
  sonar_distance_cm = mySonar.ping_cm(); 
  Serial.print("sonar distance=");
  Serial.println(sonar_distance_cm);
  RobotAction('r', 1000);
  RobotAction('l', 1000);
  RobotAction('s', 1000);
  RobotAction('c', 1000);
  RobotAction('o', 1000);
  Serial.println("Press a key and click"); // User prompt
  Serial.println("Send to start servo...");
 }  

void loop()   
{ 
 int Receivedkey;
 int distance;

    while(Serial.available() != 0) {
    Receivedkey = Serial.read(); // read stored ASCII keyword
    Serial.print("you typed "); Serial.write(Receivedkey);
    Serial.println(" ");
    }
   distance = mySonar.ping_cm();
   delay(100);
   Serial.print("sonar in cm "); Serial.println(distance);
   if(distance<STOPDISTANCE&&distance!=0)      
        RobotAction('s', 100);
   else RobotAction(Receivedkey, 100);
 
}
void RobotAction(int key, int duration) {

  switch(key) {
 case 'b': { 

  delay(duration);
  break;
  }
 case 'f': {  //forward movement
    LeftWheel.writeMicroseconds(CLOCKWISE);
    RightWheel.writeMicroseconds(COUNTERCLOCKWISE);
    delay(duration);
    break;
  }
 case 'l': { //left turn

    delay(duration);
    break;
  }
 case 'r': {  //right turn

    delay(duration);
    break;
    }
 case 's': {  //stop servos
    LeftWheel.writeMicroseconds(STOP);

    delay(duration);  
    break;
    }
 case 'o': { //open grabber

       delay(duration);  
      break;
    }
case 'c': { //close grabber
       Grabber.write(GRABBING);
       delay(duration);  
      break;
    }
 }  //end switch()
} //RobotAction()


