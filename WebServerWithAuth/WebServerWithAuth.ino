#include <SPI.h>
#include <Ethernet.h>

#define maxRequestLength 120

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);
EthernetClient client;

char request[maxRequestLength];

char method[30];
char command[30];
char parameter[30];

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
		//PerformRequestedCommand();

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

// Received request contains "METHOD /command/parameter HTTP/1.1"
void ParseReceivedRequest()
{
	PrintString("original request", request);

	// chop off HTTP version
	// "METHOD /command/parameter HTTP/1.1" -> "METHOD /command/parameter"
	// char* httpVersion = strrchr(request, ' ');
	request[strrchr(request, ' ') - request] = '\0';
	PrintString("new request", request);

	// "METHOD /command/parameter" -> "METHOD /command/parameter"
	// char* commandParameterSubstring = strstr(request, "/") + 1; 
	// int commandParameterSubstringLength = strlen(commandParameterSubstring);
	// PrintString("command paramter substring", commandParameterSubstring);
	// PrintNumber("command parameter length", commandParameterSubstringLength);
	
	int firstSlashPosition = strstr(request, "/") - request;
	PrintNumber("first slash position", firstSlashPosition);
	request[firstSlashPosition - 1] = '\0';
	PrintString("new request", request);
	PrintNumber("new request length", strlen(request));
	char* commandParameterSubstring = &request[firstSlashPosition + 1];
	PrintString("command parameter substring", commandParameterSubstring);
	
	int secondSlashPosition = strstr(commandParameterSubstring, "/") - commandParameterSubstring;
	PrintNumber("second slash position", secondSlashPosition);
	commandParameterSubstring[secondSlashPosition] = '\0';
	PrintString("command substring", commandParameterSubstring);
	char* parameterSubstring = &commandParameterSubstring[secondSlashPosition + 1];
	PrintString("parameter substring", parameterSubstring);
/*
	// parameter
	// "METHOD /command/parameter HTTP/1.1" -> "METHOD /command/parameter"
	char* parameterSubstring;
	int parameterSubstringLength;
 
	// everything after second slash
	parameterSubstring = strstr(commandParameterSubstring, "/") + 1;
	parameterSubstringLength = strlen(parameterSubstring);
	PrintString("parameter substring", parameterSubstring);
	
	// strncpy does not automatically add terminating zero, but strncat does! So start with blank string and concatenate.
	method[0] = 0;
	command[0] = 0;
	parameter[0] = 0;
	strncat(method, request, strlen(request) - commandParameterSubstringLength - 2); // looks good
	strncat(command, commandParameterSubstring, commandParameterSubstringLength - parameterSubstringLength  - 1); // looks good
	strncat(parameter, parameterSubstring, parameterSubstringLength - httpVersionLength); // looks good

	PrintString("Method", method);
	PrintString("Command", command);
	PrintString("Parameter", parameter);
*/
}

/* void PerformRequestedCommand()
{
	if ( strcmp(command,"getTemp") == 0 )
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
	}
} */

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
