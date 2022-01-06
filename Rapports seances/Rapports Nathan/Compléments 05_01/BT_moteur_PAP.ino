//fusion code moteur pas-à-pas et code BT

#include <Stepper.h>

#include<SoftwareSerial.h>
#define RX 4 // je change l'I/O de RX car les I/O 10 et 11 seront utilisées par le moteur
#define TX 5 //idem
SoftwareSerial BlueT(RX,TX);

int PWM=128;
char Data;

bool mode_auto=true; // par défaut, le store est en mode automatique

int i=0; //utilisé dans la boucle qui gère le nombre de tours du rotor du moteur
int nbtours=3; // on définit le nombre de tours que doit faire le rotor du moteur
bool position_haute=true; // on veut savoir si le store est enroulé ou déroulé
const int stepsPerRevolution = 2048; // 1 tour = 2048 pas 
const int pasParMinute = 15; // la plage réglable du pas à pas est de 0 à 17
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11); // initialisation des pins stepper moteur

#define LDR A0  // composante photorésistance sur la pin A0

int value; //stocke la valeur de la luminosité mesurée

void setup(){
  Serial.begin(9600);
  myStepper.setSpeed(pasParMinute);

  delay(500);
  BlueT.begin(9600);
  delay(500);

  Serial.println("Lancement des mesures de luminosité, veuillez patienter.");
  Serial.println("Mode auto activé au démarrage (par défaut).");
  delay(1000); // pour que les message précédents aient le temps d'être lus

  pinMode(LDR, INPUT);
}

void loop() {

  value = analogRead(LDR);

  if (BlueT.available()){
        Data=BlueT.read();
        if (Data=='A') { //si on envoie A (bouton jaune) à la carte Uno par Bluetooth alors le store change de mode 
          if (mode_auto==false){
                Serial.println("Mode auto activé."); // message d'information à l'attention de l'utilisateur
                delay(1000); // pour que le message précédent ait le temps d'être lu
              }
          else{
                Serial.println("Mode manuel activé."); // message d'information à l'attention de l'utilisateur
                delay(1000); // pour que le message précédent ait le temps d'être lu
              }
        mode_auto=!mode_auto;
        }

         else if (Data=='O') { //si on envoie O (bouton vert) à la carte Uno par Bluetooth, que le mode auto est désactivé et que le store est déroulé (position haute==false) alors le store s'enroule (O pour Open)  
            if (mode_auto==false){
              if(position_haute==false){
                Serial.println("Ouverture du store.");
                while(i<nbtours){
                    myStepper.step(-stepsPerRevolution); // dans une direction
                    i+=1;
                    }
              position_haute=true;
              }
              else{
                Serial.println("Le store est déjà ouvert."); // message d'information à l'attention de l'utilisateur
              }
            }
         }
 
        else if (Data=='C') { //si on envoie C (bouton rouge) à la carte Uno par Bluetooth, que le mode auto est désactivé et que le store est enroulé (position haute==true) alors le store se déroule (C pour Close)  
            if (mode_auto==false){
              if(position_haute==true){
                Serial.println("Fermeture du store.");
                while(i<nbtours){
                    myStepper.step(stepsPerRevolution); // dans l'autre direction direction
                    i+=1;
                    }
                position_haute=false;
              }
              else{
                Serial.println("Le store est déjà fermé."); // message d'information à l'attention de l'utilisateur
              }
            }
         }
  }
         i=0; // on assigne 0 à i en pour la future ouverture/fermeture du store
            
  if (mode_auto==true){ 

  // Wait a few seconds between measurements.
  delay(2000);

  Serial.print("Luminosité: ");
  Serial.println(value);

  if((value<230.0) &&(position_haute==false)){ //on fait monter le store si il est déroulé et que la luminosité est en-dessous du seuil (on rappelle que 0 correspond au plus lumineux et 1023 au plus sombre)
    Serial.println("Assez de luminosité, le store monte.");
    while(i<nbtours){
            myStepper.step(-stepsPerRevolution);
            i+=1;
    }  
    position_haute=true; // le store est ouvert désormais
    }
  else if((value>230.0) &&(position_haute==true)){ //on fait descendre le store si il est enroulé et que la luminosité est au-dessus du seuil (on rappelle que 0 correspond au plus lumineux et 1023 au plus sombre)
    Serial.println("Pas assez de luminosité, le store descend.");
    while(i<nbtours){
            myStepper.step(stepsPerRevolution); 
            i+=1;}
    position_haute=false; // le store est fermé désormais
    } 
  }
  i=0; // on assigne 0 à i en pour la future ouverture/fermeture du store
}
