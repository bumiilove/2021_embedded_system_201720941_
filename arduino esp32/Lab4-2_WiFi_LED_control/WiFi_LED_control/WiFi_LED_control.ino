#include <WiFi.h>
const char* ssid = "Ajou Univ";
//const char* password = "";
WiFiServer server(80);

void setup()
{
Serial.begin(115200);
pinMode(15, OUTPUT); // set the LED pin mode
delay(10);

// We start by connecting to a WiFi network

Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);

WiFi.begin(ssid);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
server.begin();

int value = 0;
}
void loop(){
  WiFiClient client = server.available(); // listen for incoming clients

  if (client) { // if you get a client,
    Serial.println("New Client."); // print a message out the serial port
    String currentLine = ""; // make a String to hold incoming data from the client
    while (client.connected()) { // loop while the client's connected
      if (client.available()) { // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c);// print it out the serial monitor
        if (c == '\n') { // if the byte is a newline character

          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("<font size='100'>Web Server Test<br>");
            client.print("Click <a href=\"/H\">here</a> LED ON<br>");
            client.print("Click <a href=\"/L\">here</a> LED OFF<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;

          } 
          else { // if you got a newline, then clear currentLine:
          currentLine = "";
          }
        } 
        else if (c != '\r') { // if you got anything else but a carriage return character,
        currentLine += c; // add it to the end of the currentLine
        }
        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
        digitalWrite(15, HIGH); // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
        digitalWrite(15, LOW); // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
