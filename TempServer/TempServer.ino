#include <SPI.h>
#include <Ethernet.h>
#include <string.h>
#include "DHT.h"
#include "TimerOne.h"
#include "Structs.h"

#define maxBufferLength 120
#define DhtUpdateInterval 3000000

DHT dht;
float desiredTemp;
float humidity;
float temperature;
String dhtStatus;

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
	
	dht.setup(2);
	
	// DHT-22 needs a delay before first read
	delay(1000);
	
	Timer1.initialize(DhtUpdateInterval);
	Timer1.attachInterrupt(UpdateTempValues);
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
		
		PerformRequestedCommand(parsedRequest);
		
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
// Received buffer template: "METHOD /command/parameter HTTP/1.1"
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

void PerformRequestedCommand(ParsedRequest & parsedRequest)
{
	actionMethod action;

	PrintHttpHeader("200 OK");

	if (CompareStrings(parsedRequest.httpMethod, "GET"))
	{
		action = &Get;
	}
	else if (CompareStrings(parsedRequest.httpMethod, "PUT"))
	{
		action = &Put;
	}
	else if (CompareStrings(parsedRequest.httpMethod, "POST"))
	{
	}
	else if (CompareStrings(parsedRequest.httpMethod, "DELETE"))
	{
	}
	else
	{
		PrintHttpHeader("HTTP/1.1 404 Not found"); 
	}

	action(parsedRequest.command, parsedRequest.parameter);
}

void UpdateTempValues()
{
	dhtStatus = dht.getStatusString();
	humidity = dht.getHumidity();
	temperature = dht.getTemperature();
}

void Get(char* command, char* parameter)
{
	PrintHttpHeader("200 OK");

	if (CompareStrings("Temp", command))
	{
		client.print(dht.toFahrenheit(temperature), 1);
	}
	else if (CompareStrings("Humidity", command)) 
	{
		client.print(humidity, 1);    
	}
	else if (CompareStrings("Status", command))
	{
		client.print(dhtStatus);   
	}
        else if (CompareStrings("FreeMemory", command))
        {
                 client.print(freeRam());
        }
	else
	{
		client.print("Status: ");
		client.print(dhtStatus);
		client.println("");
		client.print("Humidity: ");
		client.print(humidity, 1);
		client.println("");
		client.print("Temperature: ");
		client.println(dht.toFahrenheit(temperature), 1);
	}
}

void Put(char* command, char* parameter)
{
	PrintHttpHeader("200 OK");
	
	if (CompareStrings("DesiredTemp", command))
	{
		desiredTemp = atof(parameter);

		client.print(desiredTemp);
	}
}

bool CompareStrings(char* one, char* two)
{
	return strcmp(one, two) == 0;
}

void PrintHttpHeader(char* code)
{
	client.print("HTTP/1.1 ");
	client.println(code);
	client.println("Content-Type: text/html");
	client.println("Connection: close");  // the connection will be closed after completion of the response
	client.println(); 
}

// code @ https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory
int freeRam()
{
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
