#include <Servo.h>       // Include servo library
#include <Timer.h>
#include <PS2X_lib.h>  //for v1.6

Servo servoLeft;         // Declare left servo signal
Servo servoRight;        // Declare right servo signal
Servo servoGrabber;      // Declare rear servo signal
PS2X ps2x;               // create PS2 Controller Class
long starttime;          // Used for timer start
long endtime;            // Used to determine end time
int timedYes = 0;
boolean isGrabberOpen; //determines whether grabber is open
boolean isObjectInside = false; // determines whether an object is in the grabber
boolean isAutonomous; // determines whether the robot is in autonomous mode
int error = 0; // error code for ps2 controller
byte type = 0;//data type for ps2 controller
byte vibrate = 0;//whether vibration motor are on
int count = 0; // counter 

int rowCount = 0; // counts rows for autonomous mode
int forwardObsSensor = A5; // front wall detection obsticle sensor connected to pin A5
int grabberSensor = A0; // grabber detection obsticle sensor connected to pin A5

#define pressures   true
#define rumble      false
#define PS2_DAT        12  //ps2 data pin 12 
#define PS2_CMD        11  //ps2 command pin 11
#define PS2_SEL        10  //ps2 attention pin 10
#define PS2_CLK        13  //ps2 clock pin 13

void setup() 
{
  Serial.begin(9600);

 error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
 if(error == 0)
    Serial.print("Found Controller, configured successful ");
    
 else if(error == 1)
   Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
 else if(error == 2)
   Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

 else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
  
  type = ps2x.readType(); 
  switch(type) 
  {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
  case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break; 
   }
   isAutonomous = false; //robot starts in non autonmous mode
   pinMode(forwardObsSensor, INPUT);//sets pin A5 as input
   pinMode(grabberSensor,INPUT);//sets pin to A0 as inputs
   servoLeft.attach(5); // Attach left signal to P5 
   servoRight.attach(3);// Attach right signal to P3 
   servoGrabber.attach(2);// Attach left signal to P2

    delay(4000);
    starttime = millis();//sets starttime at 0 and as milliseconds
    endtime = starttime;
    while ((endtime - starttime) <= 1000 * 30) // do this loop for up to 30 sec
    {
      Autonomous();//runs the autonomous mode
      endtime = millis();
    }
}

void loop() 
{
  if(error == 1) //skip loop if no controller found
    return; 
  
  if(type == 1)//PS2 Controller
  {   
       ps2x.read_gamepad();          //read controller 

       int xAxis = ps2x.Analog(PSS_LY); // reads left joystick
       int yAxis = ps2x.Analog(PSS_LX); // reads left joystick
       int zAxis = ps2x.Analog(PSS_RY); // reads right joystick
       Serial.print("x=");
       Serial.print(xAxis);
       Serial.print("           y=");
       Serial.println(yAxis);
              if(xAxis > 220) // move forward
              {
                 int xRAxis = map(xAxis,128,255,1500,1700); // maps values for variable speeds
                 int xLAxis = map(xAxis,128,255,1500,1300);
                 forward(xRAxis, xLAxis);//writes the mapped values
              }
              if(xAxis < 80) // move backwards
              {
                int xRAxis = map(xAxis,128,0,1500,1300);// maps values for variable speeds
                int xLAxis = map(xAxis,128,0,1500,1700);
                reverse(xRAxis, xLAxis);//writes the mapped values
              }
             if(yAxis > 200) // turn left
              {
                int yRAxis = map(yAxis,128,255,1500,1700);// maps values for variable speeds
                int yLAxis = map(yAxis,128,255,1500,1700);
                turnRight(yRAxis, yLAxis);//writes the mapped values
              }
            if(yAxis < 100) //turn right
              {
                int yRAxis = map(yAxis,128,0,1500,1300);// maps values for variable speeds
                int yLAxis = map(yAxis,128,0,1500,1300);
                turnLeft(yRAxis, yLAxis);//writes the mapped values
              }
            if((xAxis < 200) && (xAxis > 100) && (yAxis < 200) && (yAxis > 100)) // stops robot
                stopServo();
            
                if(zAxis > 200) // closed grabber
                  grabberClose();
                if(zAxis < 100)// opens grabbers
                  grabberOpen();
              
               ps2x.read_gamepad(); // reads ps2 controller
            
               
  }
}
   

void forward() // move forward full speed
{
  servoLeft.write(1700);//writes the autonomous values to servos
  servoRight.write(1300);
}
void forward(int r, int l) // move forward variable speed
{
  servoLeft.write(l);//writes the mapped values
  servoRight.write(r);
}

void reverse() // reverse backwards full speed
{
  servoLeft.write(1300);//writes the autonomous values to servos
  servoRight.write(1700);
}
void reverse(int r, int l) // reverse backwards variable speed
{
  servoLeft.write(l);//writes the mapped values
  servoRight.write(r);
}

void turnRight() // turn right full speed
{
  servoLeft.write(1700);//writes the autonomous values to servos
  servoRight.write(1700);
}
void turnRight(int r, int l) // turn right variable speed
{
  servoLeft.write(l);//writes the mapped values
  servoRight.write(r);
}
void turnLeft() // turn left full speed
{
  servoLeft.write(1300);//writes the autonomous values to servos
  servoRight.write(1300);
}
void turnLeft(int r, int l) //turn left variable speed
{
  servoLeft.write(l);//writes the mapped values
  servoRight.write(r);
}
void grabberOpen() // opens grabber
{
  servoGrabber.write(90);
  isGrabberOpen = true;
}
void grabberClose() //close grabber
{
  servoGrabber.write(0);
  servoGrabber.write(180);
  isGrabberOpen = false;
}

void stopServo() // stop all servos
{
  servoLeft.write(1500);
  servoRight.write(1500);
}

void objectInside()
{
  int forwardObsValue = analogRead(forwardObsSensor); // reads wall sensor
  while((forwardObsValue > 300)&& (isObjectInside)) // while wall sensor detects no wall and while there is an object in the grabber run this code
  {
   forward();
   forwardObsValue = analogRead(forwardObsSensor); // reads wall sensor
   if((forwardObsValue <= 50)) // if there is a wall
          {
            if(count == 1)// && (rowCount < 2)) // if this is the second wall and first row use this code
            {
              stopServo();
              delay(1000);
              grabberOpen();
              delay(1000);
              count = 0;
              rowCount ++;
              if(rowCount >= 2) // if this is the second row run this code
              {
                reverse();
                delay(3600);
                turnLeft();
                delay(720);
                count = 0;
                isObjectInside =  false;
              }
              else // if only first wall is detected do this
              {
                reverse();
                delay(5000);
                turnLeft();
                delay(720);
                isObjectInside = false;
              }
            }
            else // if nothing detected by sensor do this
            {
              stopServo();
              delay(1000);
              turnLeft();
              delay(520);
              count++;
              forwardObsValue = analogRead(forwardObsSensor); // read front wall sensor
            }
           }
  }      
}

void Autonomous()
{
        int forwardObsValue = analogRead(forwardObsSensor);// reads wall sensors
        int grabberSensorValue = analogRead(grabberSensor);// reads grabber sensor
        ps2x.read_gamepad();          //read controller 

        
        if((forwardObsValue > 300)) // move forward if everything is clear
        {
          forward();
          forwardObsValue = analogRead(forwardObsSensor);
          grabberSensorValue = analogRead(grabberSensor);
          //Serial.println(lineSensorValue);
        }
        
        if((grabberSensorValue <=50)) //if something is in the grabber
        {
           forward();
           delay(100);
           grabberSensorValue = analogRead(grabberSensor);
           if(grabberSensor<=50) // if there is still an object in grabber then close grabber 
           {
             stopServo();
             delay(1000);
             grabberClose(); // close grabber
             delay(1000);
             turnRight(); // turn around
             delay(1100);
             stopServo();
             isObjectInside = true; // there is an object there
             objectInside(); // runs code on what to do with the object
           }  
         }
        
           forwardObsValue = analogRead(forwardObsSensor); //reads wall sensor
           ps2x.read_gamepad();          //read controller 
            

}



