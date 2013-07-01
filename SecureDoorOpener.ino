#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>

int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by


int dispenseTime = 700;
unsigned long previousTime = millis();
int candyDispensed = 0;

int motor = 14;
int handSensor = 5;
int NFC_IRQ = 3;
int tableDetect = 8;
int rLED = 9;
int gLED = 10;
int bLED = 11;

int buffer = 0;
int candy = 0;
int i = 0;
int handDetected = 0;
int cardDetected = 0;


Adafruit_NFCShield_I2C nfc(NFC_IRQ, 7);

void setup() {
  // put your setup code here, to run once:
  

  
pinMode(NFC_IRQ,INPUT);

pinMode(motor,OUTPUT);
pinMode(handSensor,INPUT);
pinMode(tableDetect,INPUT);
digitalWrite(tableDetect, HIGH); //Holds line high

pinMode(rLED,OUTPUT);
pinMode(gLED,OUTPUT);
pinMode(bLED,OUTPUT);

//digitalWrite(gLED,HIGH);

Serial.begin(9600);





  nfc.begin();
/*
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);
  
  // configure board to read RFID tags
  */
  nfc.SAMConfig();
    
  Serial.println("Waiting for an ISO14443A card");

}

void loop() {

} */ 

Adafruit_NFCShield_I2C nfc(NFC_IRQ, 7);



  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;

  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength))
  {

    nfc.PrintHex(uid, uidLength);

    uint32_t i;
    uint32_t b;
    
     

  }
  
  


  
  
  
  }










  
}

int dispenseCandy(int howMuchCandy)
{
int calculatedDispenseTime = howMuchCandy * dispenseTime;  
unsigned long startTime = millis();
  
 if (howMuchCandy > 0 && howMuchCandy <=5)
 {   
    digitalWrite(motor,HIGH);
    digitalWrite(rLED,LOW);
    digitalWrite(gLED,LOW);
    
    
    while ( (millis() - startTime) < calculatedDispenseTime )
    {
     digitalWrite(bLED,HIGH);
     delay(150);
     digitalWrite(bLED,LOW);
     delay(150);
    }  
    
    
    digitalWrite(motor,LOW); 
    return 1;
 }
 else
  return 0;
}




