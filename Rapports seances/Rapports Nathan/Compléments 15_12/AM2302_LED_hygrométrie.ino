//allume selon humidité

#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);
const int led_rouge=3;

void setup() {
  Serial.begin(9600);
  Serial.println("Début des mesures, veuillez patienter.");
  Serial.println("===========================");
  pinMode(led_rouge,OUTPUT);

  dht.begin();
}

void loop() {
  delay(2000);
  
  Serial.print("Temperature: ");
  
  Serial.print(getTemp("c"));
  
  Serial.print(" *C ");
  Serial.print(getTemp("f"));
  Serial.println (" *F");
  Serial.println("-----------------");  
  Serial.print("Heat index: ");
  Serial.print(getTemp("hic"));
  Serial.print(" *C ");
  Serial.print(getTemp("hif"));
  Serial.println(" *F");
  Serial.print(getTemp("k"));
  Serial.println(" *K");
  Serial.println("-----------------");    
  Serial.print("Humidity: ");
  Serial.print(getTemp("h"));
  Serial.println(" % ");

  if(getTemp("h")>50.0){
    digitalWrite(led_rouge,HIGH); //on allume la LED si le seuil est dépassé
    Serial.println("LED allumée (seuil dépassé)"); // on s'assure que tout fonctionne comme prévu
    }
  else{
    digitalWrite(led_rouge,LOW); //on éteint la LED si le seuil n'est dépassé
    Serial.println("LED éteinte (seuil non atteint)"); // on s'assure que tout fonctionne comme prévu
  }

  Serial.println("===========================");
}

float getTemp(String req) // fonction getTemp 
{
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
    return k;// return temprature in Kelvin
  }else{
    return 0.000;// if no reqest found, retun 0.000
  }
 
}
