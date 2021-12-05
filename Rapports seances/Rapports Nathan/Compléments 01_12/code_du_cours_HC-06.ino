//Par Nathan PERDOUX 
//Le code suivant provient du diaporama du cours "Eléments de robotique avec Arduino"  (page 26 à 28) http://users.polytech.unice.fr/~pmasson/Enseignement/Elements%20de%20robotique%20avec%20arduino%20-%20Communications%20RF%20-%20Projection%20-%20MASSON.pdf

#include<SoftwareSerial.h>
#define RX 10
#define TX 11
SoftwareSerial BlueT(RX,TX);


void setup(){
Serial.begin(9600);
delay(500);
Serial.println("Bonjour -Pret pour les commandes AT");
BlueT.begin(9600);
delay(500);}

void loop(){
while (BlueT.available()) {
Serial.print(char(BlueT.read())); }
while (Serial.available()) {
BlueT.write(char(Serial.read())); }
}
