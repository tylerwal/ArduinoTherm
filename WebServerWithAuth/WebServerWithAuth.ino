#include <SPI.h>
#include <Ethernet.h>

#define maxRequestLength 120

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);
EthernetClient client;

char request[maxRequestLength];
char* command;
char* parameter;

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
		PerformRequestedCommand();

		client.stop();
	}
}

void WaitForRequest(EthernetClient client)
{
	int requestLength = 0;

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
				if (requestLength < maxRequestLength)
				{
					request[requestLength++] = c;
				}
				else
				{
					break;
				}
			}
		}
	}
}

// Parses the request into the HTTP method (request), command, and paramter variables
// Received request templace: "METHOD /command/parameter HTTP/1.1"
void ParseReceivedRequest()
{
	// chop off HTTP version
	request[strrchr(request, ' ') - request] = '\0';

	int firstSlashPosition = strstr(request, "/") - request;
	request[firstSlashPosition - 1] = '\0'; // place a null character in 1 index in front of slash
	command = &request[firstSlashPosition + 1]; // set command equal to the reference of 1 character after the slash
	
	int secondSlashPosition = strstr(command, "/") - command;
	command[secondSlashPosition] = '\0'; // place a null character at slash location

	parameter = &command[secondSlashPosition + 1]; // set parameter equal to the reference of 1 character after the slash
}

void PerformRequestedCommand()
{	
	if (CompareStrings(request, "GET"))
	{
	}
	else if (CompareStrings(request, "PUT"))
	{
	}
	else if (CompareStrings(request, "POST"))
	{
	}
	else if (CompareStrings(request, "DELETE"))
	{
	}
}

bool CompareStrings(char* one, char* two)
{
	return strcmp(one, two) == 0;
}