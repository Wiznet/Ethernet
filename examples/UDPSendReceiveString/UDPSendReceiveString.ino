/*
 UDPSendReceiveString:
 This sketch receives UDP message strings, prints them to the serial port
 and sends an "acknowledge" string back to the sender

 A Processing sketch is included at the end of file that can be used to send
 and received messages for testing with a computer.

 created 21 Aug 2010
 by Michael Margolis

 This code is in the public domain.
 */


#include <Ethernet.h>
#include <EthernetUdp.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
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

byte server_gua[] = {
0x20,0x01,0x02,0xb8,
0x00,0x10,0xFF,0xFE,
0x11,0x8b,0x4d,0x1c,
0x05,0x15,0x49,0x1a
};

IP6Address ip(192, 168, 0, 4);
IP6Address myDns(192, 168, 0, 1);
IP6Address gateway(192, 168, 0, 1);
IP6Address subnet(255, 255, 0, 0);

IP6Address lla(ip6_lla, 16);
IP6Address gua(ip6_gua, 16);
IP6Address sn6(ip6_sn6, 16);
IP6Address gw6(ip6_gw6, 16);

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged";        // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

  // start the Ethernet
  #if 1
  Ethernet.begin(mac);
  #else
  Ethernet.begin(mac, ip, myDns, gateway, subnet, lla, gua, sn6, gw6);
  #endif

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
  
  Serial.println("UDPSendReceiveString address:");

  Serial.print("My IPv4 address: ");
  Serial.println(Ethernet.localIP());

  Serial.print("My IPv6 LLA: ");
  Serial.println(Ethernet.linklocalAddress());
  
  Serial.print("My IPv6 GUA: ");
  Serial.println(Ethernet.globalunicastAddress());
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IP6Address remote = Udp.remoteIP();
    Serial.print(remote);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    // send a reply to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
  delay(10);
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


