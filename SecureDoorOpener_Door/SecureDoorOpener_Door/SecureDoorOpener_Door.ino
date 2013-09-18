/*
  UDPSendReceive.pde:
 This sketch receives UDP message strings, prints them to the serial port
 and sends an "acknowledge" string back to the sender
 
 A Processing sketch is included at the end of file that can be used to send 
 and received messages for testing with a computer.
 
 created 21 Aug 2010
 by Michael Margolis
 
 This code is in the public domain.
 */


#include <SPI.h>         // needed for Arduino versions later than 0018
#include <SD.h>
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008

#define PACKET_LENGTH 100


 struct badge
 {
 char MsgType;
 char badgeType;
 char Attribute_1[3];
 char Attribute_2[5];
 char time_1[9];  
 char time_2[9];
 char ID[17];
 int  ID_Length;
 char checksum[3];
 boolean isValid;   
 };


char UID_Card[17];
int i = 0;
struct badge key;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0xDE, 0x2D, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 230);

unsigned int localPort = 1337;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[PACKET_LENGTH]; //buffer to hold incoming packet,
char packetBuffer2[PACKET_LENGTH]; //buffer to hold incoming packet,

char  ReplyBuffer[] = "acknowledged";       // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  // start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);

  Serial.begin(57600);
  pinMode(10, OUTPUT);    
  
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
   pinMode(4, OUTPUT);
   
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
  

}

void loop() {
  
  int i = 0;
  int j = 0;
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if(packetSize)
  {
    /*
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i =0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());*/

    // read the packet into packetBufffer
    Udp.read(packetBuffer,PACKET_LENGTH);
    key = parseData(packetBuffer, packetSize);
    
    if(key.isValid)
     {

    Serial.print("UID Length: ");
    Serial.println(key.ID_Length); 
    
    Serial.print("UID: ");
    j = 0;
    for (i = (16 - key.ID_Length) ; i<16; i++)
    {
     Serial.print(key.ID[i]);
     UID_Card[j++] = key.ID[i];
    } 
    Serial.println(""); 
    
    compareUID_SD(UID_Card,key.ID_Length);
     
    Serial.print("Attribute 1: ");
    Serial.println(key.Attribute_1);
    Serial.print("Attribute 2: ");
    Serial.println(key.Attribute_2);
    Serial.print("Time 1: ");
    Serial.println(key.time_1);
    Serial.print("Time2: ");
    Serial.println(key.time_2);
    Serial.print("Checksum: ");
    Serial.println(key.checksum);
       
     }
    else
    {
     Serial.println("Error");
    }

    // send a reply, to the IP address and port that sent us the packet we received
    //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    //Udp.write(ReplyBuffer);
    //Udp.endPacket();
    for(int i=0;i<100;i++) 
     packetBuffer[i] = 0;

  }
  delay(1);
}


int compareUID_SD(char* UID_STR, int length)
{
int charCount = 0;  
int currentRecord = -1;
if (length > 16 || length < 1)
 return currentRecord;
 
char read_UID[16];
File NFC_File;
char buffer;
  
 if( SD.exists("NFC.txt") )
 {
   NFC_File = SD.open("NFC.txt", FILE_READ);
   currentRecord = 0;
   charCount = 0; 
     
     while (NFC_File.available()) 
     {
       buffer = (char)NFC_File.read();
       if( (buffer == 13 || buffer == 10) && charCount != 0)
        {
         if (charCount == length)
         {
          Serial.print("Record #");
          Serial.print(currentRecord);
          Serial.println(" matches length.");
          }
         else
         {
          /*
          Serial.print("charCount: ");
          Serial.println(charCount);
          Serial.print("Record #");
          Serial.println(currentRecord); 
          for (int i = 0; i<charCount;i++)
           Serial.print(read_UID[i]);
          Serial.println(""); */
          
         }
          
         charCount = 0;
        }
       
       else if (charCount == 0 && (buffer != 13 && buffer != 10))
       {
        currentRecord++;
        read_UID[charCount++] = buffer;     
        /*Serial.print(charCount);
        Serial.print(" ");
        Serial.println(int(buffer));   
        */
       }
       
       else if (charCount < 16 && (buffer != 13 && buffer != 10) )
       {
        read_UID[charCount++] = buffer;
        /*Serial.print(charCount);
        Serial.print(" ");
        Serial.println(int(buffer));*/
       }
       
       
       
       
       // Serial.write(); 
     }
  
 NFC_File.close();
 
 
 }
 else
  return currentRecord;
}

struct badge parseData(char* inPacket, int length)
{
struct badge scan;  
int i =0,j =0;
char buffer;
int NFC_Incrementor = 0;
char NFC_packet[48];

scan.isValid = 0;

for (i=0;i<length;i++)
 {

     buffer = inPacket[i];
     
     //Serial.print(buffer);
         
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
            /*for(i=0;i<48;i++)
             Serial.write(NFC_packet[i]); 
            Serial.write("\n");*/
           
            scan.isValid = 1;
            scan.MsgType = NFC_packet[0];
            scan.badgeType = NFC_packet[1];
            
            for (j=2;j<4;j++)
             scan.Attribute_1[j-2] = NFC_packet[j];
            scan.Attribute_1[2] = char(0); 
            
            for (j=5;j<9;j++)
             scan.Attribute_2[j-5] = NFC_packet[j];    
            scan.Attribute_2[4] = char(0);  
             
            for (j=10;j<18;j++)
             scan.time_1[j-10] = NFC_packet[j];
            scan.time_1[8] = char(0);  
             
            for (j=19;j<27;j++)
             scan.time_2[j-19] = NFC_packet[j];             
            scan.time_2[8] = char(0);  
             
            for (j=28;j<44;j++)
             scan.ID[j-28] = NFC_packet[j];
            scan.ID[16] = char(0);              
             
            for (j=45;j<47;j++)
             scan.checksum[j-45] = NFC_packet[j];
           scan.checksum[2] = char(0);   


            scan.ID_Length = 16;
            for (j=0;j<16;j++)  
            {
             if( scan.ID[j] == '_')
              scan.ID_Length -= 1;
             else
              break;
              
            }
             
             
            return scan; 
           }
         
           NFC_Incrementor = 0;

         }
          
                           
          
         else if (NFC_Incrementor > 0 && NFC_Incrementor <48)
          NFC_packet[NFC_Incrementor++] = buffer;
          
         else
           NFC_Incrementor = 0;         
         
   
   

  
  
  
 } 
  
  
return scan;  
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




 




/*
  Processing sketch to run with this example
 =====================================================
 
 // Processing UDP example to send and receive string data from Arduino 
 // press any key to send the "Hello Arduino" message
 
 
 import hypermedia.net.*;
 
 UDP udp;  // define the UDP object
 
 
 void setup() {
 udp = new UDP( this, 6000 );  // create a new datagram connection on port 6000
 //udp.log( true ); 		// <-- printout the connection activity
 udp.listen( true );           // and wait for incoming message  
 }
 
 void draw()
 {
 }
 
 void keyPressed() {
 String ip       = "192.168.1.177";	// the remote IP address
 int port        = 8888;		// the destination port
 
 udp.send("Hello World", ip, port );   // the message to send
 
 }
 
 void receive( byte[] data ) { 			// <-- default handler
 //void receive( byte[] data, String ip, int port ) {	// <-- extended handler
 
 for(int i=0; i < data.length; i++) 
 print(char(data[i]));  
 println();   
 }
 */


