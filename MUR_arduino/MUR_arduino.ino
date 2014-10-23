#include "FastSPI_LED2.h"
#include <SPI.h>         
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "Config.h"

// On définie quelques valeurs.
//#define FOSC 16000000 // Fréquence de l'horloge (16MHz).
//#define BAUD 250000 // Baud rate (250kb/s).
//#define MYUBRR FOSC/16/BAUD-1 // Calcul de l'UBRR (16000000/16/250000 – 1 = 3).


//byte DNS[] = {2,0,0,1};
// On définie quelques variables.
byte remoteIp[4];
unsigned int remotePort;
char packetBuffer[550];

#define DMXshift 18

unsigned int timer;
unsigned int oldTime;


// Ci-dessous, c'est la réponse que l'on enverra aux machines présentent sur le réseau. On leur indiquera quelques paramètres de notre noeud comme par exemple l'univers, l'adresse IP, le nom de celui-ci...
unsigned char Artpollreply[217]={                       
0x41, 0x72, 0x74, 0x2d, 0x4e, 0x65, 0x74, 0x00, 
0x00, 0x21, 
ip[0], ip[1], ip[2], ip[3],     
0x36, 0x19, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x4E, 0x6F, 0x65, 0x75, 0x64, 0x20, 0x41, 0x72, 0x74, 0x2D, 0x4E, 0x65, 0x74, 0x20, 0x44, 0x4D, 0x58, 0x00, 
0x4E, 0x6F, 0x65, 0x75, 0x64, 0x20, 0x41, 0x72, 0x74, 0x2D, 0x4E, 0x65, 0x74, 0x20, 0x2F, 0x20, 
0x44, 0x4D, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

0x00, 0x01, 
0x80, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x01, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 

0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 
mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
0x02, 0x00, 0x00, 0x01,
0x00, 0x01
};

EthernetUDP Udp;


// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];
//byte smallLedsCorresp[NUM_LEDS];
//byte ledsCorresp[NUM_LEDS];
//byte ledsCorrespCarre[NUM_LEDS];
//char packetBuffer[NUM_LEDS*3];
int id = 0;
int altDim = 0;

int mode = 0;

boolean bufferUpdated = false;
boolean isTestMode = false;

// This function sets up the ledsand tells the controller about them
void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
   
  Ethernet.begin(mac, ip,dns, gateway, subnet); 
  Udp.begin(localPort);

//  pinMode(1, OUTPUT); // TX est la broche digital 1, on la place en sortie vu qu'on s'en servira.
  
  pinMode(TEST_PIN, INPUT_PULLUP);
  
  // Uncomment one of the following lines for your leds arrangement.
   FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
   
//   Serial.begin(57600);
     
     timer = millis();
     oldTime = timer;
}

void parseArtnet(){
  // On vérifie que l'on reçoit des informations via le protocole UDP.
  int packetSize = Udp.parsePacket();
   
  if (Udp.available()){
    //Udp.readPacket(packetBuffer, packetSize, ip, localPort); // On lit ces données.
    Udp.read(packetBuffer,packetSize);
    
//    for (int i=0 ; i< packetSize ; i++){
//      Serial.print ((int)packetBuffer[i]);
//      Serial.print (" ");
//    }
//    Serial.println();
//    Serial.println(packetBuffer);
    
    if( // On vérifie quec'est un ArtPoll que l'on reçoit.
      packetBuffer[0] == 'A' &&
      packetBuffer[1] == 'r' &&
      packetBuffer[2] == 't' &&
      packetBuffer[3] == '-' &&
      packetBuffer[4] == 'N' &&
      packetBuffer[5] == 'e' &&
      packetBuffer[6] == 't' &&
      packetBuffer[7] == 0x00 &&
      packetBuffer[9] == 0x20
      
      // Si oui, on envoie l'Artpollreply afin d'indiquer aux autres machines sur le réseau que l'on est la et donc de rendre le noeud pilotable.
      ) { //Udp.sendPacket (Artpollreply, 207, ip2, localPort);
      Udp.beginPacket(ip2,localPort);
      Udp.write(Artpollreply, 217);
      Udp.endPacket();
//      Serial.println("Reply");
      
    }
    
//    if (packetBuffer[9] == 0x50 && packetBuffer[14] == univers[0]) // On vérifie quec'est un ArtDMX que l'on reçoit.
    if (packetBuffer[9] == 0x50 && packetBuffer[14] == univers[0]) // On vérifie quec'est un ArtDMX que l'on reçoit.
    {
      for (int i=0 ; i<NUM_LEDS ; i++){
          leds[i].g = packetBuffer[i*3+DMXshift];
          leds[i].r = packetBuffer[i*3+1+DMXshift];
          leds[i].b = packetBuffer[i*3+2+DMXshift];
      }
      bufferUpdated = true;
//      Serial.println("packet received");
    }
  }
}

int testLed = 0;
boolean testDir = false;
int testColor = 0;

void testLeds(){
  for (int i=0 ; i<NUM_LEDS ; i++){ 
    if (i<testLed){
//      leds[i].r = 255;
//      leds[i].g = 255;
//      leds[i].b = 255;
      leds[i].r = (testColor == 0 || testColor == 1)?255:0;
      leds[i].g = (testColor == 0 || testColor == 2)?255:0;
      leds[i].b = (testColor == 0 || testColor == 3)?255:0;
    }else{
      leds[i].r = 0;
      leds[i].g = 0;
      leds[i].b = 0;
    }
  }
  if (mode){
    testLed ++;
  }
  else{
    testLed --;
  }
    
  if (mode && testLed > NUM_LEDS || !mode &&testLed <=0 ){
    mode = !mode;
    if (testLed<=0){
      testColor ++;
      if (testColor >3){
        testColor = 0;
      } 
    }
  }
}

// This function runs over and over, and is where you do the magic to light
// your leds.
void loop() {
  while(1){
    
    if (isTestMode){
      timer = millis();
      if (timer-oldTime>LOOP_TEMPO){
        oldTime = timer;
        isTestMode = !digitalRead(TEST_PIN);
        testLeds();
        FastLED.show();
      } 
    }
    else{
      parseArtnet();
      if (bufferUpdated){
        FastLED.show();
        bufferUpdated = false;
      }
      
      timer = millis();
      if (timer-oldTime>LOOP_TEMPO){
        oldTime = timer;
        isTestMode = !digitalRead(TEST_PIN);
      }
    }
  }
}
