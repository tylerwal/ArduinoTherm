#include <SPI.h>
#include <Ethernet.h>
#include "Structs.h"

#define maxBufferLength 120

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);
EthernetClient client;

typedef void (*actionMethod)(char*, char*);

void GetRequest(char *);

void ParseReceivedRequest(char*, ParsedRequest &);

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
		char buffer[maxBufferLength];
		GetHttpRequest(client, buffer);

		ParsedRequest parsedRequest;		
		ParseReceivedRequest(buffer, parsedRequest);
		
        PrintHttpHeader("200 OK");
		client.println("Method:");
		client.println(parsedRequest.httpMethod);
		client.println("Command:");
		client.println(parsedRequest.command);
		client.println("Parameter:");
		client.println(parsedRequest.parameter);

		client.stop();
	}
}

void GetHttpRequest(EthernetClient client, char* buffer)
{
	int bufferLength = 0;
	
	while (client.connected())
	{
		if (client.available())
		{
			char singleChar = client.read();

			if (singleChar == '\n')
			{
				break;
			}
			else
			{
				if (bufferLength < maxBufferLength)
				{
                    buffer[bufferLength++] = singleChar;
				}
				else
				{
					break;
				}
			}
		}
	}
	
    buffer[bufferLength++] = '\0';
}


// Parses the buffer into the HTTP method (request), command, and paramter variables
// Received buffer templace: "METHOD /command/parameter HTTP/1.1"
void ParseReceivedRequest(char* buffer, ParsedRequest & parsedRequest)
{
	// ************ HTTP Method ************
	// chop off HTTP version
	buffer[strrchr(buffer, ' ') - buffer] = '\0';
	parsedRequest.httpMethod = buffer;

	// ************ Command ************
	int firstSlashPosition = strstr(buffer, "/") - buffer;
	// place a null character in 1 index in front of slash
	buffer[firstSlashPosition - 1] = '\0'; 
	// set command equal to the reference of 1 character after the slash
	parsedRequest.command = &buffer[firstSlashPosition + 1]; 
	int secondSlashPosition = strstr(parsedRequest.command, "/") - parsedRequest.command;
	parsedRequest.command[secondSlashPosition] = '\0';

	// ************ Parameter ************
	// set parameter equal to the reference of 1 character after the slash
	parsedRequest.parameter = &parsedRequest.command[secondSlashPosition + 1]; 
}

void PerformRequestedCommand()
{	
	actionMethod action;

	PrintHttpHeader("200 OK");
	
	if (CompareStrings(request, "GET"))
	{
		action = &Get;
	}
	else if (CompareStrings(request, "PUT"))
	{
		action = &Put;
	}
	else if (CompareStrings(request, "POST"))
	{
	}
	else if (CompareStrings(request, "DELETE"))
	{
	}
	
	action(command, parameter);
	client.println(freeRam());
}

void Get(char* com, char* par)
{
/* 	client.println("Get!!");
	client.println(com);
	client.println(par); */
}

void Put(char* com, char* par)
{
/* 	client.println("Put??");
	client.println(com);
	client.println(par); */
}

bool CompareStrings(char* one, char* two)
{
	return strcmp(one, two) == 0;
}

void PrintHttpHeader(String code)
{
	client.print("HTTP/1.1 ");
	client.println(code);
	client.println("Content-Type: text/html");
	client.println("Connection: close");  // the connection will be closed after completion of the response
	client.println(); 
}

// code @ https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory
int freeRam ()
{
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
