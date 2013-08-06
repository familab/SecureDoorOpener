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
  char packet[48];
  uint8_t uidLength;
  uint8_t packetIncrementor = 0;
  char runningTime[8];
  char checksum = 0;
  String checksumStr;
  char checksumStr_buf[3];

 




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
    checksum = char(00);

   
    packetIncrementor = 0;     
    packet[packetIncrementor++] = '@'; 
    packet[packetIncrementor++] = '#'; 
    packet[packetIncrementor++] = '_';
    packet[packetIncrementor++] = '_';  
    packet[packetIncrementor++] = ',';    
    packet[packetIncrementor++] = '-'; 
    packet[packetIncrementor++] = '-'; 
    packet[packetIncrementor++] = '-'; 
    packet[packetIncrementor++] = '-'; 
    packet[packetIncrementor++] = ',';
    
    //runningTime = String(millis(), HEX);
    
    
   //Adds millis() to packet
    millisHex(runningTime);
    for(i=7 ;i>=0 ; i--)
     packet[packetIncrementor++] = runningTime[i];
     

    //Placeholder for 2nd millis()
    packet[packetIncrementor++] = ',';
    for(i = 0; i<8 ;i++)
     packet[packetIncrementor++] = '-';
    packet[packetIncrementor++] = ','; 
     
    
    for (i=0; i < (8- uidLength); i++) 
    {
          packet[packetIncrementor++] = '_'; 
          packet[packetIncrementor++] = '_'; 
    }
    
    for (i=0; i < uidLength; i++) 
    {
      packet[packetIncrementor++] = upperNibbleHex(uid[i]);
      packet[packetIncrementor++] = lowerNibbleHex(uid[i]);
    }

    
    packet[packetIncrementor++] = ',';
    
    for(i=0;i<packetIncrementor;i++)
     checksum = checksum ^ packet[i];
         
    packet[packetIncrementor++] = upperNibbleHex(int(checksum));
    packet[packetIncrementor++] = lowerNibbleHex(int(checksum));
    packet[packetIncrementor++] = '*';

    
   for(i=0;i< packetIncrementor;i++)
    {
     Serial.print(packet[i]);
     mySerial.print(packet[i]);      
    }
    
    Serial.println("");
    mySerial.println("");   
    
    digitalWrite(13,HIGH);
    delay(1000);
    digitalWrite(13,LOW);
  }
  
 }
 
 
 void millisHex(char charArray[])
 {
 uint32_t currentMillis = millis();  
 uint32_t value;
 //Serial.println(currentMillis);
 for(i=0;i<8;i=i+2)
 {
  value = currentMillis & 0x000000FF;
  charArray[i+1] = upperNibbleHex(int(value)); 
  charArray[i] = lowerNibbleHex(int(value));
  currentMillis = currentMillis >> 8; 
  //Serial.println(int(value));
 }
}
 
 char upperNibbleHex(int byteValue)
 {
 byte value = byte(byteValue);  
 value = value & 0xF0;
 value = value >> 4;  
 return returnNibbleHex( int(value) );  
 }
 
 
 char lowerNibbleHex(int byteValue)
 {
  return returnNibbleHex(byteValue);  
 }
 
 char returnNibbleHex(int byteValue)
 {
 byte value = byte(byteValue);  
 value = value & 0x0F;
 
 char hexChar = '!';
 //Serial.print(value);  
 switch(value)
  {
   case 0:
    hexChar = '0'; 
    break;    
   case 1:
    hexChar = '1';
    break;    
   case 2:
    hexChar = '2';
    break;    
   case 3:
    hexChar = '3';
    break;    
   case 4:
    hexChar = '4';
    break;    
   case 5:
    hexChar = '5';
    break;    
   case 6:
    hexChar = '6';
    break;
   case 7:
    hexChar = '7';
    break;
   case 8:
    hexChar = '8';
    break;
   case 9:
    hexChar = '9';
    break;    
   case 10:
    hexChar = 'A';
    break;    
   case 11:
    hexChar = 'B';
    break;    
   case 12:
    hexChar = 'C';
    break;    
   case 13:
    hexChar = 'D';
    break;    
   case 14:
    hexChar = 'E';
    break;    
   case 15:
    hexChar = 'F';
    break;    
  }
  //Serial.println(hexChar);   
 return hexChar;  
 } 
 
 
 
