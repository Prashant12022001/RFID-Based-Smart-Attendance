#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

 
MFRC522 rfid(SS_PIN, RST_PIN); 
MFRC522::MIFARE_Key key; 

int f1=0,f2=0;
   
byte nuidPICC[4];

void setup() { 
  Serial.begin(115200);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}
 
void loop() {

  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  if ( ! rfid.PICC_ReadCardSerial())
    return;


  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);


  if(rfid.uid.uidByte[0] != nuidPICC[0]|| 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) 
    {

    for (byte i = 0; i < 4; i++) 
     { nuidPICC[i] = rfid.uid.uidByte[i];
    }
   
    int x = int(nuidPICC[0])+int(nuidPICC[1])+int(nuidPICC[2])+int(nuidPICC[3]);

    
    
    if(x==587)
    {if(f1==0)
      {Serial.print("1");
       digitalWrite(3,HIGH);
       delay(1000);
       digitalWrite(3,LOW);
      //Serial.println("______________________________________");
      f1=1;
    }
    else {
      digitalWrite(4,LOW);
      Serial.println(F("Card read previously."));
    }
   
    }
  
  if (x==287)
 { if(f2==0)
      {Serial.print("2");
      digitalWrite(4,HIGH);
      delay(1000);
      digitalWrite(4,LOW);
      //Serial.println("______________________________________");
    f2=1;
  }
  
    else {
      digitalWrite(4,LOW);
      Serial.println(F("Card read previously."));
    }
    }}
  else Serial.println(F("Card read previously."));

  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}
