/*
 Chat Server

 A simple server that distributes any incoming messages to all
 connected clients.  To use, telnet to your device's IP address and type.
 You can see the client's input in the serial monitor as well.
 Using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 */

#include <SPI.h>
#include <Ethernet.h>
#include "IP6Address.h"

//#define DEBUG_CHATSERVER_INO_ETHERNET_BEGIN

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:

byte mac[] = {
0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

byte ip6_lla[] = {
0xfe,0x80, 0x00,0x00,
0x00,0x00, 0x00,0x00,
0x02,0x00, 0xdc,0xff,
0xfe,0x57, 0x57,0x61
};

byte ip6_gua[] = {
0x20,0x01,0x02,0xb8,
0x00,0x10,0xFF,0xFE,
0x00,0x00,0xdc,0xff,
0xfe,0x57,0x57,0x61
};

byte ip6_sn6[] = {
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0x00,0x00,0x00, 0x00,
0x00,0x00,0x00,0x00
};

byte ip6_gw6[] = {
0xfe, 0x80, 0x00,0x00,
0x00,0x00,0x00,0x00,
0x02,0x00, 0x87,0xff,
0xfe,0x08, 0x4c,0x81
};

IP6Address ip(192, 168, 0, 4);
IP6Address myDns(192, 168, 0, 1);
IP6Address gateway(192, 168, 0, 1);
IP6Address subnet(255, 255, 0, 0);

IP6Address lla(ip6_lla, 16);
IP6Address gua(ip6_gua, 16);
IP6Address sn6(ip6_sn6, 16);
IP6Address gw6(ip6_gw6, 16);

// telnet defaults to port 23
EthernetServer server(23);
bool alreadyConnected = false; // whether or not the client was connected previously

void setup() {
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

  // initialize the ethernet device
  Ethernet.begin(mac, ip, myDns, gateway, subnet, lla, gua, sn6, gw6);

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

  // start listening for clients
  server.begin(1);

  Serial.println("Chat server address:");

  Serial.print("My IPv4 address: ");
  Serial.println(Ethernet.localIP());

  Serial.print("My IPv6 LLA: ");
  Serial.println(Ethernet.linklocalAddress());
  
  Serial.print("My IPv6 GUA: ");
  Serial.println(Ethernet.globalunicastAddress());
}

void loop() {
  // wait for a new client:
  EthernetClient client = server.available();

  // when the client sends the first byte, say hello:
  if (client) {
    if (!alreadyConnected) {
      // clear out the input buffer:
      client.flush();
      Serial.println("We have a new client");
      client.println("Hello, client!");
      alreadyConnected = true;
    }

    if (client.available() > 0) {
      // read the bytes incoming from the client:
      char thisChar = client.read();
      // echo the bytes back to the client:
      server.write(thisChar);
      // echo the bytes to the server as well:
      Serial.write(thisChar);
    }
  }
}



