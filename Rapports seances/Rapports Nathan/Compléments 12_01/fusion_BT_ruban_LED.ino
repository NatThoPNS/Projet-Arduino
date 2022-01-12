//fusion code ruban LED et code BT

#include <Adafruit_NeoPixel.h>
#define ruban_LED 6  // composante led sur la pin 6
#define nombre_led_ruban 4 // nombre de led sur le ruban
Adafruit_NeoPixel strip = Adafruit_NeoPixel(nombre_led_ruban, 2, NEO_GRB + NEO_KHZ800);

#include<SoftwareSerial.h>
#define RX 4 // je change l'I/O de RX car les I/O 10 et 11 seront utilisées par le moteur
#define TX 5 //idem
SoftwareSerial BlueT(RX,TX);

#define LDR A0  // composante photorésistance sur la pin A0
int value; //stocke la valeur de la luminosité mesurée

char Data;

bool mode_auto=true; // par défaut, le chauffage est en mode automatique


void setup(){
  
  Serial.begin(9600);

  delay(500);
  BlueT.begin(9600);
  delay(500);

  Serial.println("Lancement du système d'éclairage.");
  Serial.println("Mode auto activé au démarrage (par défaut).");
  delay(1000); // pour que les message précédents aient le temps d'être lus

  pinMode(LDR, INPUT);
  pinMode(ruban_LED, OUTPUT);
  strip.begin();

  
   /* TEST POUR VOIR SI LE RUBAN EST FONCTIONNEL
    * 
    * for(int i = 0; i < nombre_led_ruban ; i++ ) { // On fait une boucle pour définir la couleur de chaque led
        // setPixelColor(n° de led, Rouge, Vert, Bleu)
        strip.setPixelColor(i, 0, 0, 0); } //on éteint les led
   strip.show(); // Initialise toute les led à 'off'*/


}

void loop() {

  value = analogRead(LDR);

  if (BlueT.available()){
        Data=BlueT.read();
        if (Data=='I') { //si on envoie I (bouton jaune) à la carte Uno par Bluetooth alors la gestion de l'éclairage change de mode 
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

         else if (Data=='G') { //si on envoie G (bouton vert) à la carte Uno par Bluetooth, que le mode auto est désactivé alors le ruban s'allume  
             /* for(int i = 0; i < nombre_led_ruban; i++ ) { 
                    strip.setPixelColor(i, 255, 255, 255);}
                    strip.show(); 
                    Serial.println(" Le ruban doit s'allumer");}*/
                     digitalWrite(ruban_LED,HIGH);
         }
 
        else if (Data=='H') { //si on envoie H (bouton rouge) à la carte Uno par Bluetooth, que le mode auto est désactivé alors le ruban s'éteint  
            if (mode_auto==false){
             
              /*for(int i = 0; i < nombre_led_ruban; i++ ) { 
              strip.setPixelColor(i, 0, 0, 0);}
              strip.show(); 
              Serial.println(" Le ruban doit s'éteindre");
            }*/

             digitalWrite(ruban_LED,LOW);
         }
        }
         
  }  
            
  if (mode_auto==true){ 

  Serial.print("Luminosité: ");
  Serial.println(value);

  if(value<230.0){ //on éteint la lumière si la luminosité est en-dessous du seuil (on rappelle que 0 correspond au plus lumineux et 1023 au plus sombre)
     Serial.println("Assez de luminosité, la lumière s'éteint.");  
     /*if (mode_auto==false){
              for(int i = 0; i < nombre_led_ruban; i++ ) { 
              strip.setPixelColor(i, 0, 0, 0);}
              strip.show(); 
              Serial.println(" Le ruban doit s'éteindre");
            }*/
            digitalWrite(ruban_LED,LOW);
    }
  else if(value>230.0){ //on allume la lumière si la luminosité est au-dessus du seuil (on rappelle que 0 correspond au plus lumineux et 1023 au plus sombre)
    Serial.println("Pas assez de luminosité, la lumière s'allume.");
    /*for(int i = 0; i < nombre_led_ruban; i++ ) { 
                    strip.setPixelColor(i, 255, 255, 255);}
                    strip.show(); 
                    Serial.println(" Le ruban doit s'allumer");}*/
                        digitalWrite(ruban_LED,HIGH);
                    }
  }

}
