#include <Adafruit_NeoPixel.h>

#define LDR A0 // composante photorésistance sur la pin A0
#define LED 6  // composante led sur la pin 6
#define nombre_led_ruban 4 // nombre de led sur le ruban

int value; //valeur luminosité

Adafruit_NeoPixel strip = Adafruit_NeoPixel(nombre_led_ruban, 2, NEO_GRB + NEO_KHZ800); //

void setup() {
   // initialise la communication avec le PC
   Serial.begin(9600);

   // initialise les broches
   pinMode(LDR, INPUT);
   pinMode(LED, OUTPUT);

   strip.begin();
   for(int i = 0; i < nombre_led_ruban ; i++ ) { // On fait une boucle pour définir la couleur de chaque led
        // setPixelColor(n° de led, Rouge, Vert, Bleu)
        strip.setPixelColor(i, 0, 0, 0); } //on éteint les led
   strip.show(); // Initialise toute les led à 'off'
}

 
void loop() {
   // mesure la tension sur la broche A0
   value = analogRead(LDR);
   Serial.print(value);

   if (value>230) { //230 valeur dans la salle mais à modifier pour la maquette
    for(int i = 0; i < nombre_led_ruban; i++ ) { 
    strip.setPixelColor(i, 255, 255, 255);}
    strip.show(); 
    Serial.println(" Le ruban doit s'allumer");}
   else {
    for(int i = 0; i < nombre_led_ruban; i++ ) { 
    strip.setPixelColor(i, 0, 0, 0);}
    strip.show(); 
    Serial.println(" Le ruban doit s'éteindre");}

   delay(20);
}
