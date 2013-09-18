#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
//#include <SoftwareSerial.h>
#include <Wiegand.h>

//SoftwareSerial mySerial(4, 5); // RX, TX

EthernetUDP Udp;
WIEGAND wg;



// the media access control (ethernet hardware) address for the shield:
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x5F, 0x5F };  
//the IP address for the shield:
byte ip[] = { 192, 168, 1, 233 }; 
byte sendToip[] = { 192, 168, 1, 230 };
byte server[] = { 192,168,1,230 };


int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

unsigned long previousTime = millis();

  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  String data;


int SITE_CODE = 128;
int MIN_CARD = 3000;
int MAX_CARD = 13000;

char runningTime[8];
char RFID_HexArray[8];




int i = 0;

char packet[48];
char NFC_packet[48];
int NFC_Incrementor = 0;
int RFID_packetIncrementor;
char buffer = 0;
 char checksum = 0;




void setup() {
  // put your setup code here, to run once:
  
Ethernet.begin(mac, ip);
  
pinMode(4,INPUT);
pinMode(5,INPUT);




Serial.begin(57600);
Udp.begin(1337);
//mySerial.begin(4800);
wg.begin();
Serial.print("Begin");




}

void  loop() 
             {
      
     checkNFCserial();
   //  checkRFID();
          
             }


boolean checkRFID()
{
  
    	if(wg.available())
	{  
  
   checksum = char(00);

   
    RFID_packetIncrementor = 0;     
    packet[RFID_packetIncrementor++] = '@'; 
    packet[RFID_packetIncrementor++] = '$'; 
    packet[RFID_packetIncrementor++] = '_';
    packet[RFID_packetIncrementor++] = '_';  
    packet[RFID_packetIncrementor++] = ',';    
    packet[RFID_packetIncrementor++] = '-'; 
    packet[RFID_packetIncrementor++] = '-'; 
    packet[RFID_packetIncrementor++] = '-'; 
    packet[RFID_packetIncrementor++] = '-'; 
    packet[RFID_packetIncrementor++] = ',';
    
    //runningTime = String(millis(), HEX);
    
    
   //Adds millis() to packet
    millisHex(runningTime);
    for(i=7 ;i>=0 ; i--)
     packet[RFID_packetIncrementor++] = runningTime[i];
     

    //Placeholder for 2nd millis()
    packet[RFID_packetIncrementor++] = ',';
    for(i = 0; i<8 ;i++)
     packet[RFID_packetIncrementor++] = '-';
    packet[RFID_packetIncrementor++] = ','; 
     
    
    for (i=0; i < (8- 3); i++) 
    {
          packet[RFID_packetIncrementor++] = '_'; 
          packet[RFID_packetIncrementor++] = '_'; 
    }

   RFID_Hex(RFID_HexArray, wg.getCode());    
    for(i=5 ;i>=0 ; i--)
     packet[RFID_packetIncrementor++] = RFID_HexArray[i];

    
    packet[RFID_packetIncrementor++] = ',';
    
    for(i=0;i<RFID_packetIncrementor;i++)
     checksum = checksum ^ packet[i];
         
    packet[RFID_packetIncrementor++] = upperNibbleHex(int(checksum));
    packet[RFID_packetIncrementor++] = lowerNibbleHex(int(checksum));
    packet[RFID_packetIncrementor++] = '*';

    

    Udp.beginPacket(sendToip, 1337);
    Udp.print(packet);
    Udp.endPacket();
    
       for(i=0;i< RFID_packetIncrementor;i++)
    {
     Serial.print(packet[i]);
     packet[i] = char(0);
    }
    
    Serial.println("");
//mySerial.println("");
    
    
    digitalWrite(13,HIGH);
    delay(1000);
    digitalWrite(13,LOW);
  }
  
}
  
   

   boolean checkNFCserial()
{   
      
         while (Serial.available())
        {
         buffer = Serial.read();
         
         if (buffer == '@')
         {
           NFC_Incrementor = 0;
           NFC_packet[NFC_Incrementor++] = buffer;
         }
          
         else if (buffer == '*')
         {
          
           if(NFC_Incrementor == 47)
           {
            NFC_packet[NFC_Incrementor++] = buffer;  
            for(i=0;i<48;i++)
             Serial.write(NFC_packet[i]); 
            Serial.write("\n");
           
            Udp.beginPacket(sendToip, 1337);
            Udp.print(NFC_packet);
            Udp.endPacket();
            
             
           }
         
           NFC_Incrementor = 0;
           digitalWrite(13,HIGH);
           delay(1000);
           digitalWrite(13,LOW);
         }
          
           
                   
          
         else if (NFC_Incrementor > 0 && NFC_Incrementor <48)
          NFC_packet[NFC_Incrementor++] = buffer;
          
         else
           NFC_Incrementor = 0;         
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


 void RFID_Hex(char charArray[],unsigned long RFID_DEC)
 {
 uint32_t RFID = RFID_DEC;  
 uint32_t value;
 //Serial.println(currentMillis);
 for(i=0;i<8;i=i+2)
 {
  value = RFID & 0x000000FF;
  charArray[i+1] = upperNibbleHex(int(value)); 
  charArray[i] = lowerNibbleHex(int(value));
  RFID = RFID >> 8; 
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





  

