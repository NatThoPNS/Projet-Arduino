//ajout BT (android) + capteur température/hygrométrie (AM2302) avec LED de seuil

#include "DHT.h"
#define DHTPIN 2     // I/O de data de l'AM2302
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321 (relaif au modèle du composant)

#include<SoftwareSerial.h>
#define RX 10
#define TX 11
SoftwareSerial BlueT(RX,TX);

DHT dht(DHTPIN, DHTTYPE);
const int led_verte=3;

int PWM=128;
char Data;

bool mode_auto=true; // par défaut, le chauffage est en mode automatique


void setup() {
  Serial.begin(9600);
  delay(500);
  BlueT.begin(9600);
  delay(500);
  Serial.println("Lancement du thermostat, veuillez patienter.");
  Serial.println("Mode auto activé au démarrage (par défaut).");
  delay(1000); // pour que les message précédents ait le temps d'être lu
  pinMode(led_verte,OUTPUT);
  dht.begin();
}

void loop() {

  if (BlueT.available()){
        Data=BlueT.read();
        if (Data=='A') { //si on envoie A (bouton jaune) à la carte Uno par Bluetooth alors le chauffage change de mode 
           if (mode_auto==false){
                Serial.println("Mode auto activé.");
                delay(1000); // pour que le message précédent ait le temps d'être lu
              }
              else{
                Serial.println("Mode manuel activé."); // message d'information à l'attention de l'utilisateur
                delay(1000); // pour que le message précédent ait le temps d'être lu
              }
        mode_auto=!mode_auto;
        }
        else if (Data=='O') { //si on envoie O (bouton vert) à la carte Uno par Bluetooth et que le mode auto est désactivé alors le chauffage s'allume (O pour Open)  
            if (mode_auto==false){
              Serial.println("Chauffage ON");
              analogWrite(led_verte, 255); // la LED s'allume
            }
            else{
              Serial.println("Le mode auto est activé. Désactivez-le pour gérer manuellement le chauffage."); // message d'information à l'attention de l'utilisateur
              delay(1000); // pour que le message précédent ait le temps d'être lu
            }
        }
        else if (Data=='C') { //si on envoie C (bouton rouge) à la carte Uno par Bluetooth et que le mode auto est désactivé alors le chauffage s'éteint (C pour Close) 
            if (mode_auto==false){
               Serial.println("Chauffage OFF");
               analogWrite(led_verte, 0); // la LED s'éteint
            }
            else{
              Serial.println("Le mode auto est activé. Désactivez-le pour gérer manuellement le chauffage."); // message d'information à l'attention de l'utilisateur
              delay(1000); // pour que le message précédent ait le temps d'être lu
            }  
        }
    }

  if (mode_auto==true){  // si le mode auto est activé, alors le module AM2302 prend des mesures et nous les affiche dans le moniteur série. 
                         //cela nous permet de déterminer si le chauffage doit s'allumer ou non selon la valeur recueillie
  
  delay(2000);// il faut attendre quelques instants avant le début des mesures

  Serial.println("===========================");
  Serial.print("Température: ");
  Serial.print(getTemp("c"));
  Serial.println(" *C ");
   
  Serial.print("Humidity: ");
  Serial.print(getTemp("h"));
  Serial.println(" % ");

  // par rapport au code de la séance précédente, j'ai supprimé les relevés d'indice de chaleur (heat index), de la température en K et en °F qui ne nous intéressent pas

  if(getTemp("h")>70.0){
    digitalWrite(led_verte,HIGH); //on allume la LED
    Serial.println("Chauffage ON");
    }
  else{
    digitalWrite(led_verte,LOW); //on éteint la LED
    Serial.println("Chauffage OFF");
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
