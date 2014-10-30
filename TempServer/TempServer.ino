#include <SPI.h>
#include <Ethernet.h>
#include <string.h>
#include "DHT.h"

DHT dht;

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);

EthernetServer server(80); // Port 80 is http.
EthernetClient client;

#define bufferMax 128
int bufferSize;
char buffer[bufferMax];

char cmd[15];    // increase these if you need longer commands/parameters
char param1[15];
char param2[15];

void setup()
{
 Ethernet.begin(mac, ip);
 server.begin();

 Serial.begin(9600);
 Serial.print("server is at ");
 Serial.println(Ethernet.localIP());
  
 dht.setup(2); // data pin 2
}

void loop()
{
 //EthernetClient 
 client = server.available();
 if (client)
 {
   WaitForRequest(client);
   ParseReceivedRequest();
   PerformRequestedCommands();
   
   client.stop();
 }
}

void WaitForRequest(EthernetClient client) // Sets buffer[] and bufferSize
{
 bufferSize = 0;

 while (client.connected()) {
   if (client.available()) {
     char c = client.read();
     if (c == '\n')
       break;
     else
       if (bufferSize < bufferMax)
         buffer[bufferSize++] = c;
       else
         break;
   }
 }
 
 PrintNumber("bufferSize", bufferSize);
}

void ParseReceivedRequest()
{
 Serial.println("in ParseReceivedRequest");
 Serial.println(buffer);
 
 //Received buffer contains "GET /cmd/param1/param2 HTTP/1.1".  Break it up.
 char* slash1;
 char* slash2;
 char* slash3;
 char* space2;
 
 slash1 = strstr(buffer, "/") + 1; // Look for first slash
 slash2 = strstr(slash1, "/") + 1; // second slash
 slash3 = strstr(slash2, "/") + 1; // third slash
 space2 = strstr(slash2, " ") + 1; // space after second slash (in case there is no third slash)
 if (slash3 > space2) slash3=slash2;

 PrintString("slash1",slash1);
 PrintString("slash2",slash2);
 PrintString("slash3",slash3);
 PrintString("space2",space2);
 
 // strncpy does not automatically add terminating zero, but strncat does! So start with blank string and concatenate.
 cmd[0] = 0;
 param1[0] = 0;
 param2[0] = 0;
 strncat(cmd, slash1, slash2-slash1-1);
 strncat(param1, slash2, slash3-slash2-1);
 strncat(param2, slash3, space2-slash3-1);
 
 PrintString("cmd",cmd);
 PrintString("param1",param1);
 PrintString("param2",param2);
}

void PerformRequestedCommands()
{
 if ( strcmp(cmd,"getTemp") == 0 ) GetTemp();
}

void GetTemp()
{
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  
  PrintHttpHeader();
  
  if (strcmp(param1, "temp") == 0) 
  {
    client.print(dht.toFahrenheit(temperature), 1);
  }
  else if (strcmp(param1, "humidity") == 0) 
  {
    client.print(humidity, 1);    
  }
  else if (strcmp(param1, "status") == 0) 
  {
    client.print(dht.getStatusString());   
  }
  else
  {
    client.print("Status: ");
    client.print(dht.getStatusString());
    client.println("");
    client.print("Humidity: ");
    client.print(humidity, 1);
    client.println("");
    client.print("Temperature: ");
    client.println(dht.toFahrenheit(temperature), 1);
  }
}

void PrintString(char* label, char* str)
{
 Serial.print(label);
 Serial.print("=");
 Serial.println(str);
}

void PrintNumber(char* label, int number)
{
 Serial.print(label);
 Serial.print("=");
 Serial.println(number, DEC);
}

void PrintHttpHeader()
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  client.println(); 
}
