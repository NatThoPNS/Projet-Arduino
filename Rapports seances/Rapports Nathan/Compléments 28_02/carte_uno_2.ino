// Programme carte Uno 2

//Partie RFID
#include <SPI.h>
#include <MFRC522.h>
const int pinRST = 9; // pin RST du module RC522
const int pinSDA = 10; // pin SDA du module RC522
String value;
MFRC522 rfid(pinSDA, pinRST);


//Partie Servo
#include <Servo.h> // include servo library to use its related functions
#define Servo_PWM 6 // A descriptive name for D6 pin of Arduino to provide PWM signal
bool porte_ouverte = false; //on part du principe que la porte est fermée
Servo MG995_Servo;  // Define an instance of of Servo with the name of "MG995_Servo"


void setup(){
            Serial.begin(9600);
              
            //Partie RFID
            SPI.begin();
            rfid.PCD_Init();
          
            //Partie Servo
             MG995_Servo.attach(Servo_PWM);  // Connect D6 of Arduino with PWM signal pin of servo motor
            }


void loop(){
      //Partie RFID
      Serial.println("En l'attente d'une carte ou d'un badge...");
      if (rfid.PICC_IsNewCardPresent()){ // on a dédecté un tag
            if (rfid.PICC_ReadCardSerial()){ // on a lu avec succès son contenu
                Serial.println("Voici l'UID de ce tag en decimal :");
                Serial.print("const byte bonUID[");
                Serial.print(rfid.uid.size);
                Serial.print("] = {");
                for (byte i = 0; i < rfid.uid.size; i++){
                    Serial.print(rfid.uid.uidByte[i]);
                    value+= String(rfid.uid.uidByte[i]);
                    if (i < rfid.uid.size - 1){
                        Serial.print(", ");
                        }
                    else{
                        Serial.println("};");
                        }
                    }
                Serial.print("Value: ");    
                Serial.println(value);
                if (value=="715422996"){ // correspond à l'identifiant du badge bleu que nous utilisons 
                    Serial.println("Accès accordé.");
                    if (porte_ouverte==false){
                          MG995_Servo.write(8); //tourne dans le sens horaire
                          Serial.println("La porte s'ouvre");
                          delay(1000);
                          porte_ouverte = true;                                    
                          }
                    else{ //la porte est ouverte
                       MG995_Servo.write(84); // tourne dans le sens anti-horaire
                       Serial.println("La porte se ferme");
                       delay(1000);
                       porte_ouverte = false;
                       }
                    delay(3000);
                    }            
                else{
                    Serial.println("Accès refusé.");
                    delay(3000);
                  }

                Serial.println(); // pour aérer l'affichage du moniteur série
                value="";  
                }
          }
      }
