/*
  WiFi UDP Send and Receive String

  This sketch waits for a UDP packet on localPort using the WiFi module.
  When a packet is received an Acknowledge packet is sent to the client on port remotePort

  created 30 December 2012
  by dlf (Metodo2 srl)

  Find the full UNO R4 WiFi Network documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples#wi-fi-udp-send-receive-string
 */


#include <WiFiS3.h>
#include <OSCBundle.h>
#include <OSCBoards.h>

int sensors[] = {A3, A4, A5};
int buttons[] = {10, 9, 8};

int status = WL_IDLE_STATUS;
#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key index number (needed only for WEP)
int packets = 0;

unsigned int localPort = 2390;      // local port to listen on

char packetBuffer[256]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged\n";       // a string to send back
OSCMessage msg("");

WiFiUDP Udp;

void setup() {
  //Initialize serial and wait for port to open:
  // Serial.begin(9600);
  // while (!Serial) {
  //   ; // wait for serial port to connect. Needed for native USB port only
  // }

  for (int buttonPin : buttons) {
    pinMode(buttonPin, INPUT_PULLUP);
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to WiFi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);

  msg.set(0, 1);
}

void loop() {

  // if there's data available, read a packet


  int packetSize = Udp.parsePacket();
  if (packetSize) {
    packets++;
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBuffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    int value = 0;
    for (int sensor : sensors) {
      int sensorValue = analogRead(sensor);
      if (sensorValue > 90) {
        value++;
        Serial.print("yes ");
        Serial.println(sensor);
      }
      value = value << 1;
    }

    for (int button : buttons) {
      byte buttonState = digitalRead(button);

    if (buttonState == LOW) {
      value++;
      Serial.print("yes ");
      Serial.println(button);
    }
      value = value << 1;
    }

    Serial.println(value);

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), 7400);
    msg.set(0, value);
    msg.send(Udp);
    Udp.endPacket();
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
