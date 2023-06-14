#include "timer.h"
int In1 = 8 ;
int In2 = 7;
int EnA = 6;

int encoderA = 2; 

Timer timer;

const int LM393 = 2;
int counter = 0;

void setup() {
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(EnA, OUTPUT);
  pinMode(encoderA, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(LM393), count, RISING);
  Serial.begin(9600);

  
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  analogWrite(EnA, 255);
  
  timer.setInterval(1000);
  timer.setCallback(RPM);
  timer.start();
}

void count() {
  counter++;
}

void RPM() {
  Serial.println((counter/20) * 60);
  counter = 0;
}

void loop() {
  timer.update();
}
