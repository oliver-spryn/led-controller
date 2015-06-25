#include <IRremote.h>
#include <IRremoteInt.h>
#include <SPI.h>
#include <Ethernet.h>
#include <String.h>

//Configure the IR library
IRsend irsend;

//Configure the ethernet server
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 80);
EthernetServer server(80);

void toggleLEDBar();
void toggleLEDStrip();

void setup() {
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop() {
  EthernetClient client = server.available();
  String header;

  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;

        if (c == '\n') {
          String msg;

          //Respond to various commands
          if (header.indexOf("/led-strip-on") > 0) {
            msg = "LED strip is on";
            toggleLEDStrip();
          } else if (header.indexOf("/led-strip-off") > 0) {
            msg = "LED strip is off";
            toggleLEDStrip();
          } else if (header.indexOf("/led-bar-on") > 0) {
            msg = "LED bar is on";
            toggleLEDBar();
          } else if (header.indexOf("/led-bar-off") > 0) {
            msg = "LED bar is off";
            toggleLEDBar();
          } else if (header.indexOf("/all-on") > 0) {
            msg = "All lights are on";
            toggleLEDBar();
            delay(500);
            toggleLEDStrip();
          } else if (header.indexOf("/all-off") > 0) {
            msg = "All lights are off";
            toggleLEDBar();
            delay(500);
            toggleLEDStrip();
          } else {
            msg = "No action has occurred";
          }

          //Print the message in a web page
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println(msg);
          client.println("</html>");

          break;
        }
      }
    }

    delay(1);
    client.stop();
  }
}

void toggleLEDBar() {
  irsend.sendNEC(0xFF28D7, 32);    
}

void toggleLEDStrip() {
  irsend.sendNEC(0xFF02FD, 32);
}
