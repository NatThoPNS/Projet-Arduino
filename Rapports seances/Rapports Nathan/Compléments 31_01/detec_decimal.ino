// pour obtenir l'identité du badge/de la carte en décimal
#include <SPI.h>
#include <MFRC522.h>
const int pinRST = 9; // pin RST du module RC522
const int pinSDA = 10; // pin SDA du module RC522
String value;
MFRC522 rfid(pinSDA, pinRST);
void setup(){
            SPI.begin();
            rfid.PCD_Init();
            Serial.begin(9600);
            }


void loop(){
      if (rfid.PICC_IsNewCardPresent()){ // on a détecté un tag
            if (rfid.PICC_ReadCardSerial()){ // on a lu avec succès son contenu
                Serial.println("Voici l'UID de ce tag en decimal :");
                for (byte i = 0; i < rfid.uid.size; i++){
                    value+= String(rfid.uid.uidByte[i]);
                    }
                Serial.print("Value: ");    
                Serial.println(value);
                value="";   
                delay(3000); //on laisse du temps d'affichage 
                }
          }
      }
