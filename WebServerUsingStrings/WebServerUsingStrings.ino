#include <SPI.h>
#include <Ethernet.h>
#include <string.h>

#define bufferMax 128

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);
EthernetClient client;

int bufferSize;
char buffer[bufferMax];

char cmd[15];
char param1[15];
char param2[15];

void setup()
{
	Ethernet.begin(mac, ip);
	server.begin();
	
	Serial.begin(9600);
	Serial.print("server is at ");
	Serial.println(Ethernet.localIP());
}

void loop()
{
	client = server.available();
	if (client)
	{
		WaitForRequest(client);
		ParseReceivedRequest();
		
		client.stop();
	}
}

void WaitForRequest(EthernetClient client)
{
	bufferSize = 0;
	
	while (client.connected()) 
	{
		if (client.available()) 
		{
			char c = client.read();
			if (c == '\n')
			{
				break;
			}
			else
			{
				if (bufferSize < bufferMax)
				{
					buffer[bufferSize++] = c;
				}
				else
				{
					break;
				}
			}
		}
	}
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
	
	// strncpy does not automatically add terminating zero, but strncat does! So start with blank string and concatenate.
	cmd[0] = 0;
	param1[0] = 0;
	param2[0] = 0;
	strncat(cmd, slash1, slash2-slash1-1);
	strncat(param1, slash2, slash3-slash2-1);
	strncat(param2, slash3, space2-slash3-1);
}
