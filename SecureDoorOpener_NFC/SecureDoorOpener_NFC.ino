#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>
#include <SPI.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); // RX, TX



#define NFC_IRQ (2)
#define RESET  (3)

int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

unsigned long previousTime = millis();

  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  String data = String(millis());




int rLED = 9;
int gLED = 10;
int bLED = 11;

int buffer = 0;

int i = 0;




Adafruit_NFCShield_I2C nfc(NFC_IRQ, RESET);

void setup(void) 
 {
  // put your setup code here, to run once:
 //pinMode(NFC_IRQ,INPUT);
 pinMode(5,OUTPUT);
 pinMode(13,OUTPUT);
 

 Serial.begin(57600);

 // set the data rate for the SoftwareSerial port
 //  mySerial.begin(9600);
 nfc.begin();
 uint32_t versiondata = nfc.getFirmwareVersion();
 mySerial.begin(4800);
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
  
  nfc.SAMConfig();
  Serial.println("Waiting for an ISO14443A card");

 }

void  loop(void) 
 {
   //  String data = "";
 //Adafruit_NFCShield_I2C nfc(NFC_IRQ, RESET);
 
 success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
   
  if (success)
   {

    Serial.print("@#");
    mySerial.print("@#");
    
    //data = String(data + "--------------"); //+14 = 16 byte
    data = String(millis(), HEX);
    
    for(i = 0;i<(8 - data.length() ); i++)
     {
       Serial.print("0");
       mySerial.print("0");
     }

    Serial.print(data);
    mySerial.print(data);
    
    
      Serial.print("------");
    mySerial.print("------");
    
    for (i=0; i < (8- uidLength); i++) 
    {
     //data = String(data + "--"); 
     Serial.print("--");
     mySerial.print("--");
    }
    
    for (i=0; i < uidLength; i++) 
    {
      if( uid[i] < 0x10)
      {
       //data = String(data + "0"); 
       Serial.print("0");
       mySerial.print("0");
      }
       
      //data = String(data + String(uid[i], HEX) ); 
      Serial.print(String(uid[i], HEX));
      mySerial.print(String(uid[i], HEX));
    }
    
    Serial.println("");
    mySerial.println("");    
    //mySerial.println(data);


    //Serial.println("ddd" );
    //data = String("@#"); //Header for RFID: Bytes 1-2
    
    digitalWrite(13,HIGH);
    delay(1000);
    digitalWrite(13,LOW);
  }
  
 
  
 }










  

