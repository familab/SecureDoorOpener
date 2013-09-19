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
long SD_Record_Num = 0;
int unlockDoor = 0;


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
//int i = 0;
struct badge key;
char NFC_packet[48];
char tempMillis[8] = {0};

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

char twoByteStr[4] = {0};

void loop() {
  
  int i = 0;
  int j = 0;
  
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if(packetSize)
  {
     // read the packet into packetBufffer
    Udp.read(packetBuffer,PACKET_LENGTH);
    key = parseData(packetBuffer, packetSize);
    for(int i=0;i<100;i++) 
     packetBuffer[i] = 0;

  }
  
  
  
  
      if(key.isValid)
     {
       for (i=(16-key.ID_Length),j=0; i<16; i++)
        UID_Card[j++] = key.ID[i];
       
       millisHex(tempMillis);
       for (i=0;i<8;i++)
        key.time_2[i] = tempMillis[i];
      
      key.Attribute_1[0] = '0';
      key.Attribute_1[1] = '?';

      SD_Record_Num = compareUID_SD(UID_Card,key.ID_Length);
      if (SD_Record_Num > 0)
      {
       key.Attribute_1[0] = '2';
       
       twoByteHex(twoByteStr,(uint16_t)SD_Record_Num);
       for (i=0;i<4;i++)
        key.Attribute_2[i] = twoByteStr[i];
       
       unlockDoor = 1; 
       generatePacket(NFC_packet,key);
       for (i=0;i<48;i++)
        Serial.print(NFC_packet[i]);
       Serial.println("");
       unlockDoor = 0;
       key.isValid = 0;
       
       delay(1);
      }
      
      
     }
  
   
}











void generatePacket(char* newPacket,struct badge scan)
{
int packetIncrementor = 0;
int i = 0;
char checksum = 0;

newPacket[packetIncrementor++] = scan.MsgType;
newPacket[packetIncrementor++] = scan.badgeType;
newPacket[packetIncrementor++] = scan.Attribute_1[0];
newPacket[packetIncrementor++] = scan.Attribute_1[1];
newPacket[packetIncrementor++] = ',';
newPacket[packetIncrementor++] = scan.Attribute_2[0];
newPacket[packetIncrementor++] = scan.Attribute_2[1];
newPacket[packetIncrementor++] = scan.Attribute_2[2];
newPacket[packetIncrementor++] = scan.Attribute_2[3];
newPacket[packetIncrementor++] = ',';
newPacket[packetIncrementor++] = scan.time_1[0];
newPacket[packetIncrementor++] = scan.time_1[1];
newPacket[packetIncrementor++] = scan.time_1[2];
newPacket[packetIncrementor++] = scan.time_1[3];
newPacket[packetIncrementor++] = scan.time_1[4];
newPacket[packetIncrementor++] = scan.time_1[5];
newPacket[packetIncrementor++] = scan.time_1[6];
newPacket[packetIncrementor++] = scan.time_1[7];
newPacket[packetIncrementor++] = ',';
newPacket[packetIncrementor++] = scan.time_2[0];
newPacket[packetIncrementor++] = scan.time_2[1];
newPacket[packetIncrementor++] = scan.time_2[2];
newPacket[packetIncrementor++] = scan.time_2[3];
newPacket[packetIncrementor++] = scan.time_2[4];
newPacket[packetIncrementor++] = scan.time_2[5];
newPacket[packetIncrementor++] = scan.time_2[6];
newPacket[packetIncrementor++] = scan.time_2[7];
newPacket[packetIncrementor++] = ',';
newPacket[packetIncrementor++] = scan.ID[0];
newPacket[packetIncrementor++] = scan.ID[1];
newPacket[packetIncrementor++] = scan.ID[2];
newPacket[packetIncrementor++] = scan.ID[3];
newPacket[packetIncrementor++] = scan.ID[4];
newPacket[packetIncrementor++] = scan.ID[5];
newPacket[packetIncrementor++] = scan.ID[6];
newPacket[packetIncrementor++] = scan.ID[7];
newPacket[packetIncrementor++] = scan.ID[8];
newPacket[packetIncrementor++] = scan.ID[9];
newPacket[packetIncrementor++] = scan.ID[10];
newPacket[packetIncrementor++] = scan.ID[11];
newPacket[packetIncrementor++] = scan.ID[12];
newPacket[packetIncrementor++] = scan.ID[13];
newPacket[packetIncrementor++] = scan.ID[14];
newPacket[packetIncrementor++] = scan.ID[15];
newPacket[packetIncrementor++] = ',';

for (i = 0; i< packetIncrementor;i++)
 checksum = checksum ^ newPacket[i];

newPacket[packetIncrementor++] = upperNibbleHex(checksum);
newPacket[packetIncrementor++] = lowerNibbleHex(checksum);
newPacket[packetIncrementor++] = '*';
}



long compareUID_SD(char* UID_STR, int length)
{
  int i = 0;
int charCount = 0;  
long currentRecord = -1;
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
       //Serial.print(buffer);
       if( (buffer == 13 || buffer == 10) && charCount != 0)
        {
         if (charCount == length)
         {
           for (i=0; i<length;i++)
            if (UID_STR[i] != read_UID[i])
             i = length;
            else if (i == length-1)
            {
             NFC_File.close(); 
             return currentRecord;
            }
          }
         
         charCount = 0;
        }
       
       else if (charCount == 0 && (buffer != 13 && buffer != 10))
       {
        currentRecord++;
        read_UID[charCount++] = buffer;     
       }
       
       else if (charCount < 16 && (buffer != 13 && buffer != 10) )
        read_UID[charCount++] = buffer;
     }
  
 NFC_File.close();
 
 
 }
 else
  return -1;
  
  return 0;
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


void twoByteHex(char charArray[], uint16_t twoBytes)
 {
   int i =0;
 uint16_t currentTwoByte = twoBytes;  
 uint16_t value;

 for(i=3;i>=0;i=i-2)
 {
  value = currentTwoByte & 0x00FF;
  charArray[i-1] = upperNibbleHex(int(value)); 
  charArray[i] = lowerNibbleHex(int(value));
  currentTwoByte = currentTwoByte >> 8; 
  //Serial.println(int(value));
 }
}




void millisHex(char charArray[])
 {
   int i = 0;
 uint32_t currentMillis = millis();  
 uint32_t value;
 //Serial.println(currentMillis);
 for(i=7;i>=0;i=i-2)
 {
  value = currentMillis & 0x000000FF;
  charArray[i-1] = upperNibbleHex(int(value)); 
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


