#include <Stepper.h>

int nbtour=0;
bool position_haute=false;
const int stepsPerRevolution = 2048; // 1 tour = 2048 pas 
const int pasParMinute = 17; // La plage réglable du pas à pas est de 0 à 17
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11); // initialisation des pins stepper moteur

void setup(){
  Serial.begin(9600);
  myStepper.setSpeed(pasParMinute);
}

void loop(){
  if (position_haute==false && nbtour<4){
    Serial.println("tourne");
    myStepper.step(stepsPerRevolution); // dans une direction
    delay(500);
    nbtour+=1; }
    //myStepper.step(-stepsPerRevolution); // dans l’autre direction
    //delay(500);
}
