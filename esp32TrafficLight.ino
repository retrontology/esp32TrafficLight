#include <WiFi.h>


const char* ssid = "Insert WiFi SSID here";
const char* password = "Insert WiFi psk here";
const char red = 23;
const char yellow = 22;
const char green = 1;
const char redMask = 0x01;

WiFiServer server(80);
String header;
char outputState = 0;
const char yellowMask = 0x02;
const char greenMask = 0x04;
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

void setup()
{
  Serial.begin(115200);
  
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void parseRequest(String header)
{
  if(header.length() >= 6 && header[5] != '/')
  {
    outputState = atoi(&header[5]);
    if(outputState < 8)
    {
      if(outputState & redMask) digitalWrite(red, HIGH);
      else digitalWrite(red, LOW);
      if(outputState & yellowMask) digitalWrite(yellow, HIGH);
      else digitalWrite(yellow, LOW);
      if(outputState & greenMask) digitalWrite(green, HIGH);
      else digitalWrite(green, LOW);
    }
  }
}

void loop()
{
  WiFiClient client = server.available();
  if (client)
  {                             
    currentTime = millis();
    previousTime = currentTime;
    String currentLine = "";
    while (client.connected() && currentTime - previousTime <= timeoutTime)
    {
      currentTime = millis();
      if (client.available())
      {
        char c = client.read();
        //Serial.write(c);
        header += c;
        if (c == '\n')
        {
          if (currentLine.length() == 0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            parseRequest(header);
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println("div { border: 5px solid black; background-color: #C7B300; width:250px; height 800px; margin: auto}");
            client.println(".button { border: 5px solid black; height: 200px; width: 200px; cursor: pointer; border-radius: 50%;}");
            client.println(".redOn { background-color: #FF0000; }");
            client.println(".redOff { background-color: #A93939; }");
            client.println(".yellowOn { background-color: #FFFF00; }");
            client.println(".yellowOff {background-color: #A9A939; }");
            client.println(".greenOn { background-color: #00FF00; }");
            client.println(".greenOff {background-color: #39A939; }");
            client.println("</style></head>");

            client.println("<div>");
            if (outputState & redMask)
            {
              client.print("<p><a href=\"");
              client.print(String(outputState & ~redMask));
              client.println("\"><button class=\"button redOn\"></button></a></p>");
            }
            else
            {
              client.print("<p><a href=\"");
              client.print(String(outputState | redMask));
              client.println("\"><button class=\"button redOff\"></button></a></p>");
            }
            if (outputState & yellowMask)
            {
              client.print("<p><a href=\"");
              client.print(String(outputState & ~yellowMask));
              client.println("\"><button class=\"button yellowOn\"></button></a></p>");
            }
            else
            {
              client.print("<p><a href=\"");
              client.print(String(outputState | yellowMask));
              client.println("\"><button class=\"button yellowOff\"></button></a></p>");
            }
            if (outputState & greenMask)
            {
              client.print("<p><a href=\"");
              client.print(String(outputState & ~greenMask));
              client.println("\"><button class=\"button greenOn\"></button></a></p>");
            }
            else
            {
              client.print("<p><a href=\"");
              client.print(String(outputState | greenMask));
              client.println("\"><button class=\"button greenOff\"></button></a></p>");
            }
            client.println("</div>");
            
            client.println("</body></html>");
            client.println();
            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (c != '\r')
        {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("");
  }
}
