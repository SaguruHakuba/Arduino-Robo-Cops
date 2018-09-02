/*
 * MP01 for ECE160 Fall2017 Miniproject 01
 * Digital and Analog Inputs and Outputs
 * @author: Scott Cai
 * 9/6/2017
 * 
 * In the system fucntion, by turning the potentiometer clockwise, LEDs will illuminate from left to right and note player bu the buzzer increases.
 * In the design function, by turning the potentiometer, LEDs will be lit in different combination as well as the notes from the buzzer.
 * 
 * function includes:
 * void setup()
 * void loop()
 * test_outputs()
 * test_inputs()
 * test_system()
 * my_design()
 */

int LEDPins[] = {10,11,12,13}; //LED pins, Red 10, Green 11, Yellow 12, Blue 13
int BtnPins[] = {2,4,7,8}; //Button pins, Red 2, Green 4, Yellow 7, Blue 8
int BtnStatus[] = {1,1,1,1}; //Button status, 1 for unpressed, 0 for pressed
const int buzzerPin = 3; //connect buzzer to digital pin 3
int RGBPins[] = {5,6,9}; //RGB pins, Blue 5, Green 6, Red 9
int sensorPin = A0; //LED is connected to analog pin 0
int sensorValue; //store the value of potentiometer

const int songLength = 16;
char notes[songLength] = {'c','c','g','g','a','a','g',' ','f','f','e','e','d','d','c',' '};
int beats[songLength] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int tempo = 113; //how fast to play the song

char startSystem = 0;
char startDesign = 0; // system status variables

void setup() {
  // variables registered
  int index;
  int delayTime = 100;
  int longerDelay = 1000;

  //initialize pins
  for(index = 0; index <= 3; index = ++index) {
    pinMode(LEDPins[index], OUTPUT);
    pinMode(BtnPins[index], INPUT);
  }

  for(index = 0; index <= 2; index = ++index) {
    pinMode(RGBPins[index], OUTPUT);
  }

  pinMode(buzzerPin, OUTPUT);

  
}

void loop() {
  //
  //test_outputs();
  //test_inputs();

  //refresh the button status in the loop
  if(digitalRead(BtnPins[0]) == LOW) {
    BtnStatus[0] = 0;
    BtnStatus[1] = 1;
    BtnStatus[2] = 1;
    BtnStatus[3] = 1;
  }

  if(digitalRead(BtnPins[1]) == LOW) {
    BtnStatus[0] = 1;
    BtnStatus[1] = 0;
    BtnStatus[2] = 1;
    BtnStatus[3] = 1;
  }

  if(digitalRead(BtnPins[2]) == LOW) {
    BtnStatus[0] = 1;
    BtnStatus[1] = 1;
    BtnStatus[2] = 0;
    BtnStatus[3] = 1;
  }

  if(digitalRead(BtnPins[3]) == LOW) {
    BtnStatus[0] = 1;
    BtnStatus[1] = 1;
    BtnStatus[2] = 1;
    BtnStatus[3] = 0;
  }
  
  if(BtnStatus[0] == 0) { //red button pressed, start test_system()
    startSystem = 1;
  }

  if(BtnStatus[3] == 0) { //blue button pressed, start my_design()
    startDesign = 1;
  }

  if(BtnStatus[1] == 0 || BtnStatus[2] == 0) { //green or yellow button pressed, stop
    digitalWrite(LEDPins[0], LOW);
    digitalWrite(LEDPins[1], LOW);
    digitalWrite(LEDPins[2], LOW);
    digitalWrite(LEDPins[3], LOW);
    noTone(buzzerPin);
    startSystem = 0;
    startDesign = 0;
  }

  if(startSystem == 1) {
    test_system();
  } 
  
  if(startDesign == 1) {
    my_design(); 
  }
  

}


void test_outputs() {
  //put code to test outputs here

  //all LEDs off
  digitalWrite(LEDPins[0], LOW);
  digitalWrite(LEDPins[1], LOW);
  digitalWrite(LEDPins[2], LOW);
  digitalWrite(LEDPins[3], LOW);
  digitalWrite(RGBPins[0], LOW);
  digitalWrite(RGBPins[1], LOW);
  digitalWrite(RGBPins[2], LOW);

  //Blink the blue, yellow, green, and red LEDs
  //blinkLEDs();

  //Use PWM to vary the brightness of the green and red LEDs
  //PWMvary();

  //Cycle through the RGB LED colors
  //cycleRGB();

  //Play a series of notes on the buzzer
  //playbuzzer();
  
}

void test_inputs() {
  //put ocde to test inputs here

  //all LEDs off
  digitalWrite(LEDPins[0], LOW);
  digitalWrite(LEDPins[1], LOW);
  digitalWrite(LEDPins[2], LOW);
  digitalWrite(LEDPins[3], LOW);
  digitalWrite(RGBPins[0], LOW);
  digitalWrite(RGBPins[1], LOW);
  digitalWrite(RGBPins[2], LOW);

  //button pressed, light up
  while(digitalRead(BtnPins[0]) == LOW) {
    digitalWrite(LEDPins[0], HIGH);
  }

  while(digitalRead(BtnPins[1]) == LOW) {
    digitalWrite(LEDPins[1], HIGH);
  }

  while(digitalRead(BtnPins[2]) == LOW) {
    digitalWrite(LEDPins[2], HIGH);
  }

  while(digitalRead(BtnPins[3]) == LOW) {
    digitalWrite(LEDPins[3], HIGH);
  }

  //When PMW is too low or too high, red LED blinks at a frequency set by the potentiometer value
  while(analogRead(sensorPin) <=300 || analogRead(sensorPin) >= 700) {
    sensorValue = analogRead(sensorPin);
    digitalWrite(LEDPins[0], HIGH);
    while(sensorValue == analogRead(sensorPin)) {
      delay(analogRead(sensorPin));
      digitalWrite(LEDPins[0], LOW);
      delay(analogRead(sensorPin));
      digitalWrite(LEDPins[0], HIGH);
    }
  }
  
}

void test_system() {
  //put code to test sytstem here

  sensorValue = analogRead(sensorPin); //read PWM value from 0 to 1023
  if(sensorValue <= 145) {
    digitalWrite(LEDPins[0], HIGH);
    digitalWrite(LEDPins[1], LOW);
    digitalWrite(LEDPins[2], LOW);
    digitalWrite(LEDPins[3], LOW);
  }

  if(sensorValue <= 511 && sensorValue > 145) {
    digitalWrite(LEDPins[0], LOW);
    digitalWrite(LEDPins[1], HIGH);
    digitalWrite(LEDPins[2], LOW);
    digitalWrite(LEDPins[3], LOW);
  }

  if(sensorValue <= 767  && sensorValue > 511) {
    digitalWrite(LEDPins[0], LOW);
    digitalWrite(LEDPins[1], LOW);
    digitalWrite(LEDPins[2], HIGH);
    digitalWrite(LEDPins[3], LOW);
  }

  if(sensorValue <= 1023 && sensorValue > 767) {
    digitalWrite(LEDPins[0], LOW);
    digitalWrite(LEDPins[1], LOW);
    digitalWrite(LEDPins[2], LOW);
    digitalWrite(LEDPins[3], HIGH);
  }

  //divide the 0-1023 area into seven parts for the notes
  if(sensorValue <= 145) {
    tone(buzzerPin, frequency('c'));
  }

  if(sensorValue <= 291 && sensorValue > 145){
    tone(buzzerPin, frequency('d'));
  }

  if(sensorValue <= 437 && sensorValue > 291){
    tone(buzzerPin, frequency('e'));
  }

  if(sensorValue <= 583 && sensorValue > 437){
    tone(buzzerPin, frequency('f'));
  }

  if(sensorValue <= 729 && sensorValue > 583){
    tone(buzzerPin, frequency('g'));
  }

  if(sensorValue <= 875 && sensorValue > 729){
    tone(buzzerPin, frequency('a'));
  }

  if(sensorValue <= 1023 && sensorValue > 875){
    tone(buzzerPin, frequency('b'));
  }
  
}

void my_design() {
  //put code for my design here
  sensorValue = analogRead(sensorPin); //read PWM value from 0 to 1023
  if(sensorValue <= 145) {
    for(int index = 3; index >= 0; index = --index) {
      for(int x = 0; x < 5; x = ++x) {
        digitalWrite(LEDPins[index], HIGH);
        delay(sensorValue);
        digitalWrite(LEDPins[index], LOW);
        delay(sensorValue);
      }
      delay(sensorValue);
    }
  }

  if(sensorValue <= 511 && sensorValue > 145) {
    digitalWrite(LEDPins[0], HIGH);
    digitalWrite(LEDPins[1], LOW);
    digitalWrite(LEDPins[2], LOW);
    digitalWrite(LEDPins[3], LOW);
    delay(sensorValue);
    digitalWrite(LEDPins[0], HIGH);
    digitalWrite(LEDPins[1], HIGH);
    digitalWrite(LEDPins[2], LOW);
    digitalWrite(LEDPins[3], LOW);
    delay(sensorValue);
    digitalWrite(LEDPins[0], HIGH);
    digitalWrite(LEDPins[1], HIGH);
    digitalWrite(LEDPins[2], HIGH);
    digitalWrite(LEDPins[3], LOW);
    delay(sensorValue);
    digitalWrite(LEDPins[0], HIGH);
    digitalWrite(LEDPins[1], HIGH);
    digitalWrite(LEDPins[2], HIGH);
    digitalWrite(LEDPins[3], HIGH);
    delay(sensorValue);
    digitalWrite(LEDPins[0], LOW);
    digitalWrite(LEDPins[1], LOW);
    digitalWrite(LEDPins[2], LOW);
    digitalWrite(LEDPins[3], LOW);
    delay(sensorValue);
  }

  if(sensorValue <= 767  && sensorValue > 511) {
    cycleRGB();
  }

  if(sensorValue <= 1023 && sensorValue > 767) {
    playbuzzer();
  }
  
}


void blinkLEDs() {
  const int delayTime = 100;
  const int longerDelay = 1000;
  for(int index = 3; index >= 0; index = --index) {
    for(int x = 0; x < 5; x = ++x) {
      digitalWrite(LEDPins[index], HIGH);
      delay(delayTime);
      digitalWrite(LEDPins[index], LOW);
      delay(delayTime);
    }
    delay(longerDelay);
  }
}

void cycleRGB() {
  const int delayTime = 10;
  const int longerDelay = 1000;
  for(int x = 0; x <= 767; x++) {
    int redIntensity, greenIntensity, blueIntensity;
    x = constrain(x, 0, 767);

    if(x <= 255) { //red to green
      redIntensity = 255 - x;
      greenIntensity = x;
      blueIntensity = 0;
    }else if(x <= 511) { //green to blue
      redIntensity = 0;
      greenIntensity = 511 - x;
      blueIntensity = x - 256;
    }else { //blue to red
      redIntensity = x - 512;
      greenIntensity = 0;
      blueIntensity = 767 - x;
    }

    analogWrite(RGBPins[0], blueIntensity);
    analogWrite(RGBPins[1], greenIntensity);
    analogWrite(RGBPins[2], redIntensity);
    delay(delayTime);
  }
  delay(longerDelay);
}

void playbuzzer() {
  int i, duration;
  for(i = 0; i < songLength; i++) {
    duration = beats[i]*tempo;
    if(notes[i] == ' ') {
      delay(duration);
    }else {
      tone(buzzerPin, frequency(notes[i]), duration);
      delay(duration);
    }
    delay(tempo/10);
  }

  while(true) {
    //only play it once
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

void PWMvary() {
  sensorValue = analogRead(sensorPin);
  int greenIntensity, redIntensity;
  int delayTime = 100;
  
  analogWrite(LEDPins[0],sensorValue/4);
  analogWrite(LEDPins[1],sensorValue/4);
  delay(delayTime);
}

