//Code fusion store + BT + ruban + AM2302 présentation 1

//PARTIE STORE

#include <Stepper.h>
bool mode_auto_store=true; // par défaut, le store est en mode automatique
int i=0; //utilisé dans la boucle qui gère le nombre de tours du rotor du moteur
int nbtours=4; // on définit le nombre de tours que doit faire le rotor du moteur
bool position_haute=true; // on veut savoir si le store est enroulé ou déroulé. Au départ, on suppose qu'il est enroulé
const int stepsPerRevolution = 2048; // 1 tour = 2048 pas 
const int pasParMinute = 15; // la plage réglable du pas à pas est de 0 à 17
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11); // initialisation des pins stepper moteur
#define LDR A0  // composante photorésistance sur la pin A0
int value; //stocke la valeur de la luminosité mesurée

//PARTIE BT
#include<SoftwareSerial.h>
#define RX 4 // à relier à la broche TX du HC-06
#define TX 5 // à relier à la broche RX du HC-06
SoftwareSerial BlueT(RX,TX);
char Data;


//PARTIE CHAUFFAGE
bool mode_auto_chauffage=true;
#include "DHT.h"
#define DHTPIN 2     // I/O de data de l'AM2302
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321 (relaif au modèle du composant)
DHT dht(DHTPIN, DHTTYPE);
const int LED_chauffage=3;
int PWM=128;


//PARTIE RUBAN
bool mode_auto_ruban=true;
#include <Adafruit_NeoPixel.h>
#define ruban_LED 6  // composante led sur la pin 6
#define nombre_led_ruban 8 // nombre de led sur le ruban
Adafruit_NeoPixel strip = Adafruit_NeoPixel(nombre_led_ruban, ruban_LED, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(9600);
  Serial.println("Lancement des mesures de luminosité, du thermostat et du système d'éclairage.Veuillez patienter.");
  Serial.println("Mode auto activé au démarrage pour le store, le chauffage et le système d'éclairage (par défaut).");
  delay(1000); // pour que les message précédents aient le temps d'être lus

  //PARTIE STORE
  myStepper.setSpeed(pasParMinute);
  pinMode(LDR, INPUT);

  //PARTIE BT
  delay(500);
  BlueT.begin(9600);
  delay(500);

  //PARTIE CHAUFFAGE
  pinMode(LED_chauffage,OUTPUT);
  dht.begin();

  //PARTIE RUBAN
  strip.begin();  
  for(int i = 0; i < nombre_led_ruban ; i++ ) { // on utilise une boucle pour définir la couleur de chaque LED
                strip.setPixelColor(i, 0, 0, 0); // setPixelColor(n° de led, Rouge, Vert, Bleu)
                } //on éteint les led
  strip.show(); // initialise toute les led à 'off'
  }

void loop() {
  
  //PARTIE Bluetooth (BT)
  //PARTIE STORE BT
  if (BlueT.available()){
        Data=BlueT.read();
        if (Data=='F') { //si on envoie F (bouton jaune) à la carte Uno par Bluetooth alors la gestion du store change de mode de fonctionnement
           if (mode_auto_store==false){
                Serial.println("Mode auto du store activé."); // message d'information à l'attention de l'utilisateur
                delay(1000); // pour que le message précédent ait le temps d'être lu
                }
           else{
                Serial.println("Mode manuel du store activé."); // message d'information à l'attention de l'utilisateur
                delay(1000); // pour que le message précédent ait le temps d'être lu
                }
           mode_auto_store=!mode_auto_store; // le mode de fonctionnement passe de automatique à manuel ou l'inverse
           }

        else if (Data=='D') { //si on envoie D (bouton vert) à la carte Uno par Bluetooth, que le mode auto est désactivé et que le store est déroulé (position_haute==false) alors le store s'enroule 
            if (mode_auto_store==false){
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
 
        else if (Data=='E') { //si on envoie E (bouton rouge) à la carte Uno par Bluetooth, que le mode auto est désactivé et que le store est enroulé (position_haute==true) alors le store se déroule 
            if (mode_auto_store==false){
              if(position_haute==true){
                Serial.println("Fermeture du store.");
                while(i<nbtours){
                    myStepper.step(stepsPerRevolution); // dans l'autre direction
                    i+=1;
                    }
                position_haute=false;
                }
              else{
                Serial.println("Le store est déjà fermé."); // message d'information à l'attention de l'utilisateur
                }
              }
            }
  

  //PARTIE CHAUFFAGE BT
        if (Data=='C') { //si on envoie C (bouton jaune) à la carte Uno par Bluetooth alors le chauffage change de mode de fonctionnement
           if (mode_auto_chauffage==false){
                Serial.println("Mode auto du chauffage activé.");
                delay(1000); // pour que le message précédent ait le temps d'être lu
                }
              else{
                Serial.println("Mode manuel du chauffage activé."); // message d'information à l'attention de l'utilisateur
                delay(1000); // pour que le message précédent ait le temps d'être lu
                }
              mode_auto_chauffage=!mode_auto_chauffage;
              }
        else if (Data=='A') { //si on envoie A (bouton vert) à la carte Uno par Bluetooth et que le mode auto est désactivé alors le chauffage s'allume 
            if (mode_auto_chauffage==false){
              Serial.println("Chauffage ON");
              analogWrite(LED_chauffage, 255); // la LED de chauffage s'allume
              }
            else{
              Serial.println("Le mode auto du chauffage est activé. Veuillez le désactiver pour gérer manuellement le chauffage."); // message d'information à l'attention de l'utilisateur
              delay(1000); // pour que le message précédent ait le temps d'être lu
              }
            }
        else if (Data=='B') { //si on envoie B (bouton rouge) à la carte Uno par Bluetooth et que le mode auto est désactivé alors le chauffage s'éteint 
            if (mode_auto_chauffage==false){
               Serial.println("Chauffage OFF");
               analogWrite(LED_chauffage, 0); // la LED de chauffage s'éteint
               }
            else{
              Serial.println("Le mode auto est activé. Désactivez-le pour gérer manuellement le chauffage."); // message d'information à l'attention de l'utilisateur
              delay(1000); // pour que le message précédent ait le temps d'être lu
              }  
            }


  //PARTIE RUBAN BT
  
  if (Data=='I') { //si on envoie I (bouton jaune) à la carte Uno par Bluetooth alors le système d'éclairage change de mode de fonctionnement
          if (mode_auto_ruban==false){
                Serial.println("Mode auto sys. d'éclairage activé."); // message d'information à l'attention de l'utilisateur
                delay(1000); // pour que le message précédent ait le temps d'être lu
                }
          else{
               Serial.println("Mode manuel sys. d'éclairage activé."); // message d'information à l'attention de l'utilisateur
               delay(1000); // pour que le message précédent ait le temps d'être lu
               }
          mode_auto_ruban=!mode_auto_ruban;
          }

        else if ((Data=='G')&&(mode_auto_ruban==false)) { //si on envoie G (bouton vert) à la carte Uno par Bluetooth et que le mode auto est désactivé alors le ruban s'allume  
              for(int i = 0; i < nombre_led_ruban; i++ ) { 
                    strip.setPixelColor(i, 255, 255, 255);
                    }
              strip.show(); 
              Serial.println("Eclairage ON");
              }
                             
                 
 
        else if ((Data=='H')&&(mode_auto_ruban==false)) { //si on envoie H (bouton rouge) à la carte Uno par Bluetooth et que le mode auto est désactivé alors le ruban s'éteint  
             
              for(int i = 0; i < nombre_led_ruban; i++ ){ 
                  strip.setPixelColor(i, 0, 0, 0);
                  }
              strip.show();     
              Serial.println("Eclairage OFF");
              }
  }

  
   //PARTIE STORE NON-BT
   
  if (mode_auto_store==true){ 
  // Patentier quelques instants avant le début des mesures
  //delay(2000); on peut instaurer ce délai mais il réduit la fluidité des informations affichées sur le moniteur série 

  value = analogRead(LDR);
       
  Serial.println("===========================");
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
    
  else if((value>230.0)&&(position_haute==true)){ //on fait descendre le store si il est enroulé et que la luminosité est au-dessus du seuil (on rappelle que 0 correspond au plus lumineux et 1023 au plus sombre)
    Serial.println("Pas assez de luminosité, le store descend.");
    while(i<nbtours){
            myStepper.step(stepsPerRevolution); 
            i+=1;
            }
    position_haute=false; // le store est fermé désormais
    }

  if ((mode_auto_chauffage==false)&&(mode_auto_ruban==false)){ //si le mode auto du chauffage et du ruban sont désactivés, on instaure un délai pour que la luminosité affichée dans le moniteur série ne défile pas à toute allure
                 delay(500);
                 }
  }
  i=0; // on assigne 0 à i pour la future ouverture/fermeture du store



  //PARTIE CHAUFFAGE NON-BT

  if (mode_auto_chauffage==true){  // si le mode auto est activé, alors le module AM2302 prend des mesures et nous les affiche dans le moniteur série. 
                                  // cela nous permet de déterminer si le chauffage doit s'allumer ou non selon la valeur recueillie
                                  
            delay(2000);// il faut attendre quelques instants avant le début des mesures
     
            Serial.print("Température: ");
            Serial.print(getTemp("c"));
            Serial.println(" *C ");
             
            Serial.print("Humidity: ");
            Serial.print(getTemp("h"));
            Serial.println(" % ");
          
            if(getTemp("h")>70.0){
                digitalWrite(LED_chauffage,HIGH); //on allume la LED de chauffage 
                Serial.println("Chauffage ON");
                }
            else{
                digitalWrite(LED_chauffage,LOW); //on éteint la LED de chauffage 
                Serial.println("Chauffage OFF");
                }
            }



  //PARTIE RUBAN NON-BT 
        
  if (mode_auto_ruban==true){ 
           if (mode_auto_store==false){ //si le mode auto du store est désactivé, on veut que la luminosité nous soit tout de même affichée dans le moniteur série
                  value = analogRead(LDR);
                  Serial.print("Luminosité: ");
                  Serial.println(value);
                  if (mode_auto_chauffage==false){ //si le mode auto du store et du chauffage sont désactivés, on instaure un délai pour que la luminosité affichée dans le moniteur série ne défile pas à toute allure
                          delay(1000); // pour que le message précédent ait le temps d'être lu
                          }
                  
                  }
          
           if(value<230.0){ //on éteint la lumière si la luminosité est en-dessous du seuil (on rappelle que 0 correspond au plus lumineux et 1023 au plus sombre)
               Serial.print("Assez de luminosité, la lumière est éteinte. ");  
                        for(int i=0; i < nombre_led_ruban; i++ ) { 
                        strip.setPixelColor(i, 0, 0, 0);
                        }
                        strip.show(); 
                        Serial.println("Eclairage OFF");
              }
           else if(value>230.0){ //on allume la lumière si la luminosité est au-dessus du seuil (on rappelle que 0 correspond au plus lumineux et 1023 au plus sombre)
              Serial.print("Pas assez de luminosité, la lumière est allumée. ");
              for(int i = 0; i < nombre_led_ruban; i++ ) { 
                              strip.setPixelColor(i, 255, 255, 255);}
                              strip.show(); 
                              //Serial.println(" Le ruban doit s'allumer");
                              
                                  //digitalWrite(ruban_LED,HIGH);
                                  Serial.println("Eclairage ON");
                              }

           
           if((mode_auto_store==false)&&(mode_auto_chauffage==false)&&(mode_auto_ruban==false)) {
                Serial.println("Veuillez activer le mode automatique d'un des appareils de votre maison pour voir apparaître les relevés température/humidité/luminosité."); 
                delay(1000); // pour que le message précédent ait le temps d'être lu 
                }
           Serial.println("==========================="); 
           }
  }


float getTemp(String req) // fonction pour mesurer température, hygrométrie et indice de chaleur (heat index)
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // Compute heat index in Kelvin 
  float k = t + 273.15;
  if(req =="c"){
    return t;//return Cilsus
  }else if(req =="f"){
    return f;// return Fahrenheit
  }else if(req =="h"){
    return h;// return humidity
  }else if(req =="hif"){
    return hif;// return heat index in Fahrenheit
  }else if(req =="hic"){
    return hic;// return heat index in Cilsus
  }else if(req =="k"){
    return k;// return temperature in Kelvin
  }else{
    return 0.000;// if no request found, retun 0.000
  }
}
