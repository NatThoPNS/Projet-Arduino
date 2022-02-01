// la LED verte s'allume brièvement si le badge est scanné, la LED rouge s'allume brièvement si tout autre carte/badge est scanné(e)
#include <SPI.h>
#include <MFRC522.h>
const int pinRST = 9; // pin RST du module RC522
const int pinSDA = 10; // pin SDA du module RC522
String value;
MFRC522 rfid(pinSDA, pinRST);

const int led_rouge=3;
const int led_verte=2;

void setup(){
            pinMode(led_rouge,OUTPUT);
            pinMode(led_verte,OUTPUT);
            SPI.begin();
            rfid.PCD_Init();
            Serial.begin(9600);
            }


void loop(){
      Serial.println("En l'attente d'une carte ou d'un badge...");
      if (rfid.PICC_IsNewCardPresent()){ // on a détecté un tag
            if (rfid.PICC_ReadCardSerial()){ // on a lu avec succès son contenu
                Serial.print("Voici l'UID de ce tag en decimal :");
                for (byte i = 0; i < rfid.uid.size; i++){
                    value+= String(rfid.uid.uidByte[i]);
                    }   
                Serial.println(value);
                if (value=="715422996"){ // on compare l'IUD scanné à celui du badge 
                    Serial.println("Accès accordé.");
                    digitalWrite(led_verte,HIGH); // la LED verte s'allume si l'IUD scanné correspond bien à celui du badge
                    delay(3000);
                    }
                else{
                    Serial.println("Accès refusé.");
                    digitalWrite(led_rouge,HIGH); // la LED rouge s'allume si l'IUD scanné ne correspond pas à celui du badge
                    delay(3000);
                  }

                Serial.println(); // pour aérer l'affichage du moniteur série
                value="";  
                digitalWrite(led_rouge,LOW); // la LED verte s'éteint après chaque scan  
                digitalWrite(led_verte,LOW); // la LED rouge s'éteint après chaque scan
                delay (1000); // on patiente un instant avant un nouveau test
                }
          }
      }
