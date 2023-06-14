
#include "timer.h"

float kp = 0.01;
float ki = 0.001 ;
float kd = 0;
 
int encoderA = 2;

 #define pi 3.1416
volatile unsigned int counter;
int Rpm, RPM_d,RPM_max=840;
float Vmax = 6;
float Vmin = -6;
float Vd = RPM_d/RPM_max*Vmax;
float e, e_prev = 0, inte, inte_prev = 0;
 
 float sign(float x) {
  if (x > 0) {
    return 1;
  } else if (x < 0) {
    return -1;
  } else {
    return 0;
  }
}

//***Motor Driver Functions*****
void TurnMotorF(){
  digitalWrite(In1, HIGH);
   digitalWrite(In2, LOW);

}

void TurnOFF(){
 digitalWrite(In1, LOW);
 digitalWrite(In2, LOW);

}
void TurnMotorB(){
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);

}

void WriteDriverVoltage(float V, float Vmax) {
  int PWMval = int(255 * abs(V) / Vmax);
  if (PWMval > 255) {
    PWMval = 255;
  }
  if (V > 0) {
    //Move Forward
    TurnMotorF();
  }
  else if (V < 0) {
    //Move Backward
    TurnMotorB();
  }
  else {
    TurnOFF();
  }
  analogWrite(EnA, PWMval);
}

void setup() {
  //Initialisation des Pins
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(EnA, OUTPUT);
  pinMode(encoderA, INPUT);
 
  digitalWrite(encoderA, HIGH);
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  analogWrite(EnA, 0);
 
  attachInterrupt(digitalPinToInterrupt(encoderA),countpulse,RISING);
  Serial.begin(9600);
  
}

 
void countpulse(){
        counter++;
}
 
void loop() {
  
  static uint32_t prevMillis1,prevMillis2;
  const unsigned long sampleTime = 10; // milliseconds
  float t=millis();
  float dt1= t - prevMillis1;
  float dt2= t - prevMillis2;
  if (dt1 >= 1000) {
    // Calcul du RPM reel
            Rpm = (counter/20)*60;          
            counter = 0;
            prevMillis1 +=1000; }
            
   if (dt2 > sampleTime) {          
    // Calcul du RPM desire               
    RPM_d = 400;
  
    
    //Calcul de  l'erreur, son integral et la variable de commande V
    e = (RPM_d - Rpm)/60;
    inte = inte_prev + e*dt2;
    deve= (e - e_prev) / dt2;
    V_com = kp * e + ki * inte + kd *deve ;
    V= Vd+V_com;
    //Limitation du signal de commande
    if (V < 0) {
      V = max(Vmin,V)
     
     }
    
    if (V > 0) {
      V = min(Vmax,V);
    
    }
    
    // Commande des moteurs
     WriteDriverVoltage(V, Vmax);
    

    Serial.print(RPM_d); Serial.print(" \t");
    Serial.print(Rpm); Serial.print(" \t ");
    Serial.print(V2); Serial.print(" \t ");
    Serial.print(V); Serial.print("\t  ");
    Serial.print(inte); Serial.print("\t  ");
    Serial.print(e); Serial.println("  ");

    inte_prev = inte;
    e_prev = e;
    prevMillis2+=10;
    count_prev = count;
   }
}


