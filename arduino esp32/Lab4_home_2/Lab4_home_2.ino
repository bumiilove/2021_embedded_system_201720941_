#include <WiFi.h>
const char* ssid     = "Ajou Univ";
//const char* password = "";
WiFiServer server(80);
String currentLine;
void setup()
{
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}
void loop()
{
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = ""; // make a String to hold incoming data from the client
    while (client.connected() ){
      if (client.available()){
        char c = client.read();
        Serial.write(c);
        if (c == '\n'){
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");  
            client.println("Content-type:text/html");
            client.println();
            client.print("<font size='100'>Web Server Test<br>");
            client.println("<img src=\"https://grm-project-template-bucket.s3.ap-northeast-2.amazonaws.com/organization/ajou/logo/logo_ajou.png\">");

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
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}  
