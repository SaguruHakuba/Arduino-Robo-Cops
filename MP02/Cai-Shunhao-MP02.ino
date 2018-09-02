/*
 * MP02 for ECE160 Fall2017 Miniproject 02
 * Motors and Alarms
 * @author: Scott Cai
 * 9/14/2017
 * 
 * In this project, with the help of photo sensor and temperature sensor, we can read the ambient value of temperature and light through the analog. 
 * Servo motor can adjust its angle, while DC motor its speed.
 * Also, the lcd is using 4-bit transmission. Buzzer will alarm when the temperature is too high or the room is too bright.
 * 
 * servo motor 9
 * DC motor 10
 * buzzer 6
 * button 7
 * LED 8
 * Photo Resistor A1
 * Temperature Sensor A0
 * 
 * LCD:
 * RS-12
 * E-11
 * DB4-5
 * DB5-4
 * DB6-3
 * DB7-2
 * 
 * function includes:
 * void setup()
 * void loop()
 * void data_display()
 * void analog_thermometer()
 * void light_tracker()
 * void alarm_system()
 * void my_design()
 * void lcd_initial()
 * float getTempC()
 * float getTempF()
 * int getLight()
 */
#include <LiquidCrystal.h>
#include <Servo.h>


LiquidCrystal lcd(12,11,5,4,3,2);  // RS, E, DB4, DB5. DB6, DB7
Servo servo1;
const int tempPin = A0;
const int photoPin = A1;
unsigned int lightLevel, calibratedlightLevel;
const int servo = 9;
const int motor = 10;
const int buzzer = 6;
const int LED = 8;
const int btn = 7;

const int songLength = 16;
int tempo = 139;
char notes[songLength] = {'c','c','g','g','a','a','g',' ','f','f','e','e','d','d','c',' '};
int beats[songLength] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

unsigned int alarmStatus = 0; //0 off, 1 on
unsigned int servoposition;
unsigned int motorspeed;
unsigned int buzzerStatus = 0;
unsigned int degreesCStatus = 0;
unsigned int degreesFStatus = 1;


void setup() {
  // put your setup code here, to run once:
  lcd_initial();
  pinMode(tempPin, INPUT);
  pinMode(photoPin, INPUT);
  servo1.attach(servo, 900, 2100); //pulse width min 900us, max 2100us
  pinMode(motor, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // data_display();
  // analog_thermometer();
  // light_tracker();
  // alarm_system();
   my_design();
}



void data_display() {
  //put code to test outputs here
  lcd.clear();
  lcd.setCursor(0, 0);
  //lcd.print(millis()/1000); //print the number of seconds since the Arduino last reset
  lcd.write("temp:");
  lcd.print(getTempF());
  lcd.write(" F");
  lcd.setCursor(0,1);
  lcd.write("light:");
  lcd.print(getLight());
  delay(200);
}

void analog_thermometer() {
  //put code to test inputs here
  servoposition = map(getTempF(), 70, 80, 0, 180);
  servo1.write(servoposition);
  delay(200);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write("temp:");
  lcd.print(getTempF());
  lcd.write(" F");
  lcd.setCursor(0,1);
  lcd.write("angle:");
  lcd.print(servoposition);
}

void light_tracker() {
  //put code to test system here
  motorspeed = map(getLight(), 0, 255, 0, 255);
  Serial.println("Light:");
  Serial.print(getLight());
  Serial.println("Speed:");
  Serial.print(motorspeed);
  analogWrite(motor, motorspeed);
  delay(20);
}

void alarm_system() {
  //put ocde for my design here
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("alarm off");
  alarmStatus = 0;
  if(getTempC() >= 24 || getLight() >= 150) {
    alarmStatus = 1;
    digitalWrite(LED, HIGH);
  }
  if(alarmStatus == 1) {
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("alarm on");
    buzzerStatus = 1;
  }
  if(buzzerStatus == 1) {
    tone(buzzer, 440);
  }
  if(digitalRead(btn) == 0) {
     alarmStatus = 0;
     buzzerStatus = 0;
    //digitalWrite(LED, LOW);
  }
  if(buzzerStatus == 0) {
    noTone(buzzer);
  }
  lcd.setCursor(0,1);
  lcd.write("tp:");
  lcd.print(getTempC());
  lcd.write("C");
  lcd.write(" lt:");
  lcd.print(getLight());
  delay(200);
}

void my_design() {
  //put code for my design here
  lcd.setCursor(0, 0);
  lcd.print("alarm off!");
  servo1.write(0);
  if(digitalRead(btn) == 0) {
    alarmStatus = 1; // waiting for alarm
  }
  if(alarmStatus == 1) {
    lcd.setCursor(0, 1);
    lcd.print("alarm awake");
  }else {
    lcd.setCursor(0, 1);
    lcd.print("alarm sleep");
  }
  if(getLight() <= 150 && alarmStatus == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("alarm ONON");
    servo1.write(getLight());
    Serial.print(getLight());
    playbuzzer();  
  }
  delay(300);
}

void lcd_initial() {
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,1);
  //lcd.print("hello, world!");
}

float getTempC() {
  float voltage, degreesC;
  voltage = analogRead(tempPin)*5.0/1024;
  degreesC = (voltage-0.5)*100.0; //convert the voltage to degrees Celsius
  return degreesC;
}

float getTempF() {
  float voltage, degreesC, degreesF;
  voltage = analogRead(tempPin)*5.0/1024;
  degreesC = (voltage-0.5)*100.0;
  degreesF = degreesC*(9.0/5.0)+32.0; //Convert degrees Celsius to Fahrenheit
  return degreesF;
}

int getLight() {
  lightLevel = analogRead(photoPin); //read the voltage on the photo sensor pin
  //Serial.println(lightLevel);
  calibratedlightLevel = map(lightLevel, 0, 1023, 0, 255); //map(inputValue, fromMin, fromMax, toMin, to Max);
  //Serial.print("\t");
  //Serial.print(calibratedlightLevel);
  return calibratedlightLevel;
}

void playbuzzer() {
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
