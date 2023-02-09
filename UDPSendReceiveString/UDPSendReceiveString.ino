/*
 UDPSendReceiveString

 This sketch receives UDP message strings, prints them to the serial port
 and sends an "acknowledge" string back to the sender

 A Processing sketch is included at the end of file that can be used to send
 and receive messages for testing with a computer.

 created 21 Aug 2010
 by Michael Margolis

 This code is in the public domain.
 */


#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#define I2C_ADDRESS 0x3C

#define UDP_TX_PACKET_MAX_SIZE 64
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);

unsigned int localPort = 49154;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged";        // a string to send back

String Received[UDP_TX_PACKET_MAX_SIZE];
String Message[32];
String teststring;
short WertDez[32];
int counter = 0;

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

SSD1306AsciiWire oled;

void setup() {
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH Shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit FeatherWing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit FeatherWing Ethernet

  // start the Ethernet
  Ethernet.begin(mac, ip);

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start UDP
  Udp.begin(localPort);

  Wire.begin();
  Wire.setClock(400000L);
  oled.begin(&Adafruit128x64, I2C_ADDRESS);

}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i=0; i < 4; i++) {
      Serial.print(remote[i], DEC);
      if (i < 3) {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBuffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    //Serial.println(packetBuffer);

    oled.setFont(System5x7); // Auswahl der Schriftart
    oled.clear(); //Löschen der aktuellen Displayanzeige
    oled.setContrast(100);

    /*for(int i = 0; i< 64; i++)
    {
      Serial.print((byte)packetBuffer[i]);
      Serial.print(" ");

      oled.print((int)packetBuffer[i]);
    }

    Serial.println("");*/

      
    for(int i = 0; i < UDP_TX_PACKET_MAX_SIZE; i++)
    {
      int temp[8];
      for(int j = 7; j >= 0; j--)
      {
        temp[j] =  bitRead((byte)packetBuffer[i], j);
        teststring = String(temp[7])  + String(temp[6]) + String(temp[5]) + String(temp[4]) + String(temp[3]) + String(temp[2]) + String(temp[1]) + String(temp[0]);
        
      }

      Received[i] = teststring;
     
    }

    
    unsigned int temp2 = 0;
    unsigned int temp4 = 0;
    String temp3 = "";
    
    for(int j = 0; j < 32; j++)
    {
      Message[j] = Received[counter + 1] + Received[counter];
      counter = counter + 2;
      temp3 = Message[j];
      //Serial.println(temp3); // Da nochmal schöneren Code überlegen


      int exp = 15;
      for(int t = 0; t < 16; t++)
      {
        temp2 = temp2 + temp3.substring(t, t+1).toInt() * round(pow(2, exp)) ; // Warum zum Teufel muss da die + 1 hin?!!?!?!?!
        exp--;
        WertDez[j] = temp2;
      }
      temp2 = 0;
    }

    /*for(int l = 0; l < 32; l++)
    {
      Serial.println(WertDez[l]);
    }
    */

    double d = (double(WertDez[4]) / 32767) * (double)200;
    Serial.println(d);

    Serial.println("");
    


    
  }
  delay(0);
}
