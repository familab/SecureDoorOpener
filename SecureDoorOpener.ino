#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>
#include <SPI.h>
#include <Ethernet.h>

#define NFC_IRQ (2)
#define RESET  (3)

// the media access control (ethernet hardware) address for the shield:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
//the IP address for the shield:
byte ip[] = { 10, 13, 37, 250 };    

int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

unsigned long previousTime = millis();





int rLED = 9;
int gLED = 10;
int bLED = 11;

int buffer = 0;

int i = 0;




Adafruit_NFCShield_I2C nfc(NFC_IRQ, RESET);

void setup() {
  // put your setup code here, to run once:
  
Ethernet.begin(mac, ip);
  
pinMode(NFC_IRQ,INPUT);



pinMode(rLED,OUTPUT);
pinMode(gLED,OUTPUT);
pinMode(bLED,OUTPUT);

//digitalWrite(gLED,HIGH);

Serial.begin(57600);


  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  char data[];


  nfc.begin();

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



Adafruit_NFCShield_I2C nfc(NFC_IRQ, 7);





  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength))
  {

    nfc.PrintHex(uid, uidLength);
	
	data = "@#"; //Header for RFID: Bytes 1-2
	
	

	
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.write("@#");

    Udp.endPacket();
	
	
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




