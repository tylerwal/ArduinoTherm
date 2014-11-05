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
	PrintString("Method", request);
	PrintString("Command", command);
	PrintString("Parameter", parameter);
	
/* 	if ( strcmp(command,"getTemp") == 0 )
	{
		GetTemp();
	}
	else if ( strcmp(command,"setTemp") == 0 )
	{
		SetTemp();
	}
	else
	{
		PrintHttpHeader("HTTP/1.1 404 Not found");
	} */
}

void PrintString(char* label, char* str)
{
	Serial.print(label);
	Serial.print("=");
	Serial.println(str);
}

void PrintNumber(char* label, int num)
{
	Serial.print(label);
	Serial.print("=");
	Serial.println(num);
}
