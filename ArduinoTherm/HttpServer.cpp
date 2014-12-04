#include "Arduino.h"
#include <Ethernet.h>
#include "HttpServer.h"
#include "Utilities.h"
#include "Thermostat.h"

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

void PrintHttpHeader(EthernetClient client, const char* statusCode)
{
	client.print("HTTP/1.1 ");
	client.println(statusCode);
	client.println("Content-Type: text/html");
	client.println("Connection: close");  // the connection will be closed after completion of the response
	client.println(); 
}

void PerformRequestedCommand(EthernetClient client, ParsedRequest & parsedRequest)
{
	actionMethod action;
	
	if (CompareStrings(parsedRequest.httpMethod, "GET"))
	{
		action = &PerformGet;
	}
	else if (CompareStrings(parsedRequest.httpMethod, "PUT"))
	{
		action = &PerformPut;
	}
	else
	{
		PrintHttpHeader(client, "404 Not found");
		return;
	}
	
	action(client, parsedRequest.command, parsedRequest.parameter);
}

void PerformGet(EthernetClient client, const char* command, const char* parameter)
{
	PrintHttpHeader(client, "200 OK");
	
	if (CompareStrings("Temperature", command))
	{
		client.print(currentTemperature, 1);
	}
	else if (CompareStrings("GoalTemperature", command)) 
	{
		client.print(goalTemperature, 1);
	}
	else if (CompareStrings("Humidity", command)) 
	{
		client.print(currentHumidity, 1);
	}
	else if (CompareStrings("Status", command))
	{
		client.print(dhtStatus);
	}
	else if (CompareStrings("FreeMemory", command))
	{
		client.print(freeRam());
	}
	else if (CompareStrings("TimeInCurrentState", command))
	{
		client.print(systemState.TimeInCurrentState());
	}
	else if (CompareStrings("ResourceDirectory", command))
	{
		client.print("Resource Directory:<br><br>Get: Temp, GoalTemperature, Humidity, Status<br>Put: GoalTemperature");
	}
	else
	{
		client.print("404");
	}
}
	
void PerformPut(EthernetClient client, const char* command, const char* parameter)
{
	PrintHttpHeader(client, "200 OK");
	
	if (CompareStrings("GoalTemperature", command))
	{
		goalTemperature = atof(parameter);
		client.print(goalTemperature);
	}
	else if (CompareStrings("ResetTimeInCurrentState", command))
	{
		systemState.StartTimeCurrentState = millis();
	}
}
