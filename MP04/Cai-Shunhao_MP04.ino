/*
 *MP04 for ECE160 Fall2017 Miniproject 04
 *Wireless Communication
 *Receiver
 *@author: Scott Cai
 *10/1/2017
 *
 *
 *DC motor 5
 *servo motor 9
 *IR sensor A3
 *Object detection sensor 6
 *
 *Wireless Transimitter NRF24LO1
 *
 *
 *function includes():
 *void setup()
 *void loop()
 *
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#include <math.h>

Servo servo1;

#define datapin 2   //SER
#define clockpin 3  //SCK
#define latchpin 4  //RCK

#define joyZ A0
#define joyY A1
#define joyX A2

#define CE 7
#define CSN 8
#define MOSI 11
#define MISO 12
#define SCK 13

#define DC 5
#define IR A3
#define ServoMotor 9
#define OBS 6

#define SERVOLOW 650
#define SERVOHIGH 2700
#define BAUDRATE 9600

int xAxis, yAxis, zAxis;

#define Reset 0
#define LeftLED 1
#define RightLED 2
#define ForwardServo 3
#define BackwardServo 4
#define MiddleServo 5
#define turnDC 6

int IRStatus = 0; //0 for nothing in the distance, 1 for sth and disable the joystick
int xposition, yposition, swstatus;

static unsigned char state = Reset;

const uint64_t pipe = 0xE8E8F0F0E1LL; //transimission pipe
RF24 radio(CE, CSN);  //Creating a radio object
int joystick [3]; //three element array for joystick coordinates
 
void setup() {
  // put your setup code here, to run once:  
  pinMode(datapin, OUTPUT);
  pinMode(clockpin, OUTPUT);
  pinMode(latchpin, OUTPUT);
  pinMode(joyZ, INPUT_PULLUP);
  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);

  radio.begin();
  radio.setChannel(22);
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  
  Serial.begin(BAUDRATE);
  Serial.println("nRF24L01 Transmitter Starting");

  pinMode(DC, OUTPUT);
  pinMode(OBS, INPUT);
  servo1.attach(ServoMotor, SERVOLOW, SERVOHIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
   //wired_test();
  wireless_test();
}

void shiftWrite(byte data) {
 //Send "data" to the shift register1
 Serial.print("Shift data  "); 
 Serial.println(data,BIN);
  digitalWrite(latchpin, LOW); 
  shiftOut(datapin, clockpin, MSBFIRST, data); 
 //Positive edge on latchPin to store data
  digitalWrite(latchpin, HIGH); 
}

void wired_test() {
  
  //switch(state) {
  //  case Reset:
  //  shiftWrite(0b11111111);
  //  readSensors();
  //  takeActions(xposition, yposition, swstatus, IRStatus);
  //  break;
  //
  //  case LeftLED:
  //  shiftWrite(0b11110000);
  //  readSensors();
  //  takeActions(xposition, yposition, swstatus, IRStatus);
  //  break;
  //
  //  case RightLED:
  //  shiftWrite(0b00001111);
  //  readSensors();
  //  takeActions(xposition, yposition, swstatus, IRStatus);
  //  break;
  //
  //  case ForwardServo:
  //  servo1.write(180);
  //  readSensors();
  //  takeActions(xposition, yposition, swstatus, IRStatus);
  //  break;
  //
  //  case MiddleServo:
  //  servo1.write(90);
  //  readSensors();
  //  takeActions(xposition, yposition, swstatus, IRStatus);
  //  break;
  //
  //  case BackwardServo:
  //  servo1.write(0);
  //  readSensors();
  //  takeActions(xposition, yposition, swstatus, IRStatus);
  //  break;
  //
  //  case turnDC:
  //  analogWrite(DC, 255);
  //  delay(1000);
  //  analogWrite(DC, 0);
  //  delay(100);
  //  readSensors();
  //  takeActions(xposition, yposition, swstatus, IRStatus);
  //  break;
  //}

  readSensors();
  if(xposition >= 800) {
      shiftWrite(0b11110000);
  }
  if(xposition <= 200) {
      shiftWrite(0b00001111);
  }
  if(yposition >= 800) {
      servo1.write(180);
  }
  if(yposition <= 200) {
      servo1.write(0);
  }
  if(yposition >= 200 & yposition <= 800) {
      servo1.write(90);
  }
  if(swstatus == 0) {
      analogWrite(DC, 255);
      delay(1000);
      analogWrite(DC, 0);
      delay(100);
  }
}

void wireless_test() {
  if(radio.available()) {
    while (radio.available()) {
      radio.read(joystick, sizeof(joystick));
      Serial.print("VRX=");
      Serial.println(joystick[0]);
      Serial.print("VRY=");
      Serial.println(joystick[1]);
      Serial.print("SW=");
      Serial.println(joystick[2]);
      Serial.println();

      if(joystick[0] >= 800) {
        shiftWrite(0b11110000);
      }
      if(joystick[0] <= 200) {
        shiftWrite(0b00001111);
      }
      if(joystick[1] >= 800) {
        servo1.write(180);
      }
      if(joystick[1] <= 200) {
        servo1.write(0);
      }
      if(joystick[1] >= 200 & joystick[1] <= 800) {
        servo1.write(90);
      }
      if(joystick[2] == 0) {
        analogWrite(DC, 255);
        delay(1000);
      }else if(joystick[2] == 1) {
        analogWrite(DC, 0);
        delay(10);
      }
      delay(10);
    }
  }else {
    Serial.println("No radio available.");
  }
}

void readSensors() {
  readJoy();
  readIR();
}

void readJoy() {
  xposition = analogRead(joyX);
  yposition = analogRead(joyY);
  swstatus = digitalRead(joyZ);
  delay(100);

  Serial.print("VRX=");
  Serial.println(xposition);
  Serial.print("VRY=");
  Serial.println(yposition);
  Serial.print("SW=");
  Serial.println(swstatus);
  Serial.println();
  delay(100);
    
}

void readIR() {
  unsigned int IRValue;
  float distance;
  IRValue = analogRead(IR);
  distance = 80*exp(-(IRValue*5.0/1024-0.2));
  Serial.print("Distance in cm = ");
  Serial.println(distance);
  Serial.print("IRValue = ");
  Serial.println(IRValue);
  delay(100);

  //decide the IRStatus
  if(distance <= 1) {
    IRStatus = 1;
  }else {
    IRStatus = 0;
  }
  Serial.print("IRStatus = ");
  Serial.println(IRStatus);
  Serial.println();
}

void takeActions(int x, int y, int z, int ir) {
  //x = xposition;
  //y = yposition;
  //z = swstatus;
  //ir = IRStatus;

  while(ir == 0){
    if(x >= 800) {
      state = LeftLED;
    }else if(x <= 200) {
      state = RightLED;
    }else if(y >= 800) {
      state = ForwardServo;
    }else if(y <= 200) {
      state = BackwardServo;
    }else if (y >= 200 & y <= 800) {
      state = MiddleServo;
    }else if(z == 0) {
      state = turnDC;
    }
  }
  
}

