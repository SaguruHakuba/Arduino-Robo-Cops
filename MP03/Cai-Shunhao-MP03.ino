/*
 *MP03 for ECE160 Fall2017 Miniproject 03
 *State Machine
 *@author: Scott Cai
 *9/21/2017
 *
 *
 *DC motor 2
 *servo motor 11
 *LED 8
 *buzzer 9
 *obastacle avoidance sensor A5
 *flex sensor A0
 *soft pot A1
 *button 4
 *
 *function includes():
 *void setup()
 *void loop()
 *void run_machine1()
 *void run_machine2()
 *void read_sensors()
 *void read_soft()
 *void read_flex()
 *void read_obs()
 *
 */

#include <Servo.h>

Servo servo1;

#define DCMotor 2
#define ServoMotor 11
#define LED 8
#define buzzer 9
#define OBS A5
#define flex A0
#define softPot A1
#define Btn 4

int flexVal;
int oaVal;
int softVal;

#define FLASH_LED 0
#define MOTOR_SPIN 1
#define BUZZER_PLAY 2
#define SERVO_MOVE 3

//sensor status: true or false, HIGH or LOW
//set by read_sensors();
int flex_status, pot_status, object_detect_status;

//servo low (0 degree) and high (180 degree) bounds
//the values need to be calibrated to certain servo
#define SERVOLOW 650
#define SERVOHIGH 2700

static unsigned char state=FLASH_LED;
   

const int songLength = 18;
char notes[songLength] = {'c', 'd', 'f', 'd', 'a', ' ', 'a', 'g', ' ', 'c', 'd', 'f', 'd', 'g', ' ', 'g', 'f', ' '};
int beats[songLength] = {1,1,1,1,1,1,4,4,2,1,1,1,1,1,1,4,4,2};
int tempo = 113;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(DCMotor, OUTPUT);
  servo1.attach(ServoMotor, SERVOLOW, SERVOHIGH);
  pinMode(LED, OUTPUT);
  pinMode(flex, INPUT);
  pinMode(softPot, INPUT);
  pinMode(OBS, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(Btn, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  //run_machine1();
  //read_sensors();
  run_machine2();
  //my_design();

}

void run_machine1() {
  //put code here to run the timer state machine
  static unsigned char state = FLASH_LED;
  switch(state) {
    case FLASH_LED:
      flashLED();
      //
      state = MOTOR_SPIN;
      Serial.println("FLASH LED");
      delay(200);
      break;
      
     case MOTOR_SPIN:
      spinDC();
      //
      state = BUZZER_PLAY;
      Serial.println("Spin Motor");
      delay(200);
      break;
      
     case BUZZER_PLAY:
      buzzerSong();
      //
      state = SERVO_MOVE;
      Serial.println("Play on Buzzer");
      delay(200);
      break;

     case SERVO_MOVE:
      moveServo();
      //
      state = FLASH_LED;
      Serial.println("Move Servo");
      Serial.println(" ");
      delay(200);
      break;
      
  }
}

//thresholds to be adjusted
#define FLESHTHRESHOLD 1
#define POTTHRESHOLD 2
#define OBJECTTHRESHOLD 0
//read sensors and set their status values: true or false, HIGH or LOW
void read_sensors() {
  //set status based on reading to be true or false, HIGH or LOW
  //int flex_Status, pot_status, object_detect_status;
  //use Serial.print() to help you debug
  pot_status = read_soft();
  object_detect_status = read_obs();
  flex_status = read_flex();

  Serial.print("soft:");
  Serial.println(analogRead(A1));

  Serial.print("obs:");
  Serial.println(analogRead(A5));

  Serial.print("obsStatus:");
  Serial.println(object_detect_status);

  Serial.print("flex:");
  Serial.println(analogRead(A0));
}

int read_soft() {
  //put code here here to read soft potentiometer
  if(analogRead(A1) >= 800) {
    return 1;
  }else {
    return 0;
  }
}

int read_obs() {
  //put ocde here to read object detection sensor
  if(analogRead(A5) < 500) { // we have detected sth.
    return 1;
  }else {
    return 0;
  }
}

int read_flex() {
  //put code here to read flex sensor
  if(analogRead(A0) >= 850) {
    return 1;
  }else {
    return 0;
  }
}

void run_machine2() {
  //put code here to run the input state machine
  //state machine block
   switch (state) {
    case FLASH_LED:
      flashLED();  
      read_sensors(); 
      if(flex_status==1) state = FLASH_LED;
      else if (pot_status==1) state =  MOTOR_SPIN;
      else if (object_detect_status==0) state =  SERVO_MOVE;
      else state = FLASH_LED;
      Serial.println("Flash LED");
      Serial.print("NextState:");
      Serial.println(state);
      break;
    case MOTOR_SPIN:
      spinDC();   
      read_sensors(); 
      if(flex_status==1) state = FLASH_LED;
      else if (pot_status==1) state =  BUZZER_PLAY;
      else if (object_detect_status==0) state =  FLASH_LED;
      else state = MOTOR_SPIN;
      Serial.println("Spin Motor");
      break;
    case BUZZER_PLAY:
      buzzerSong(); 
      read_sensors(); 
      if(flex_status==1) state = FLASH_LED;
      else if (pot_status==1) state =  SERVO_MOVE;
      else if (object_detect_status==0) state =  MOTOR_SPIN;
      else state = BUZZER_PLAY;
      Serial.println("Play on Buzzer");
      break;
    case SERVO_MOVE: 
      moveServo();  
      read_sensors(); 
      if(flex_status==1) state = FLASH_LED;
      else if (pot_status==1) state =  FLASH_LED;
      else if (object_detect_status==0) state =  BUZZER_PLAY;
      else state = SERVO_MOVE;
      Serial.println("Move Servo"); 
      Serial.println(" ");
      break;
  }
}

void my_design() {
  //put code here to run your state machine design
}

void buzzerSong() {
  //Play a song
  int i, duration;
  for(i = 0; i < songLength; i++) {
    duration = beats[i]*tempo;
    if(notes[i] == ' ') {
      delay(duration);
    }else {
      tone(buzzer, frequency(notes[i]), duration);
      delay(duration);
    }
    delay(tempo/10);
  }
}

void flashLED() {
  //flash LED 5 times
  for(int i = 0; i<5; i++) {
    digitalWrite(LED, HIGH);
    delay(300);
    digitalWrite(LED, LOW);
    delay(300);
  }
}

void moveServo() {
  //move servo up and down
  servo1.write(0);
  delay(500);
  servo1.write(180);
  delay(500);
  servo1.write(0);
  delay(500);
  servo1.write(180);
  delay(500);
  servo1.write(0);
}

void spinDC() {
  //spin DC motor for 3 seconds 
  analogWrite(DCMotor, 255);
  delay(3000);
  analogWrite(DCMotor, 0);
  delay(100);
}

int frequency(char note) {
  const int numNotes = 9;
  char names[numNotes] = {'c','d','e','f','g','a','b','C'};
  int frequencies[numNotes] = {262,294,330,349,392,440,494,523};

  for(int i = 0; i < numNotes; i++) {
    if(names[i] == note) {
      return(frequencies[i]);
    }
  }
  return(0);
}

