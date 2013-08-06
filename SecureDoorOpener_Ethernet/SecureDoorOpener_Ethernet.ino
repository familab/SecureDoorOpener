#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); // RX, TX

#define NFC_IRQ (2)
#define RESET  (3)

EthernetUDP Udp;


// the media access control (ethernet hardware) address for the shield:
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x5F, 0x5F };  
//the IP address for the shield:
byte ip[] = { 10, 13, 37, 250 };    

int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

unsigned long previousTime = millis();

  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  String data;



int rLED = 9;
int gLED = 10;
int bLED = 11;



int i = 0;

char RFID_packet[48];
char NFC_packet[48];
int NFC_Incrementor = 0;
char buffer = 0;




void setup() {
  // put your setup code here, to run once:
  
Ethernet.begin(mac, ip);
  
pinMode(NFC_IRQ,INPUT);



pinMode(rLED,OUTPUT);
pinMode(gLED,OUTPUT);
pinMode(bLED,OUTPUT);

//digitalWrite(gLED,HIGH);

Serial.begin(57600);
Udp.begin(9802);
mySerial.begin(4800);

Serial.print("Begin");



}

void  loop() 
             {
             
        while (mySerial.available())
        {
         buffer = mySerial.read();
         
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
             
           }
         
           NFC_Incrementor = 0; 
         }          
          
         else if (NFC_Incrementor > 0 && NFC_Incrementor <48)
          NFC_packet[NFC_Incrementor++] = buffer;
          
         else
           NFC_Incrementor = 0;         
          }
          
             }
          
          
          

          

          
      
               
               
	
   /* Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write("@#");
    Udp.endPacket();

    uint32_t i;
    uint32_t b;
*/
  
  







  

