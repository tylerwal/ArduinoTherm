#include <SPI.h>
#include <Ethernet.h>

#define maxRequestLength 120

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);
EthernetClient client;

char request[maxRequestLength];

char method[6];
char command[15];
char parameter[15];

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

void ParseReceivedRequest()
{
	Serial.println(request);

	//Received request contains "METHOD /command/parameter HTTP/1.1".  Break it up.
	char* method;
	char* commandParameterSubstring;
	char* parameterSubstring;
	char* slash3;
	char* space2;

	commandParameterSubstring = strstr(request, "/") + 1; // everything after first slash
	parameterSubstring = strstr(commandParameterSubstring, "/") + 1; // everything after second slash

	PrintString("commandParameterSubstring",commandParameterSubstring);
	PrintString("parameterSubstring",parameterSubstring);
	// PrintString("slash3",slash3);
	// PrintString("space2",space2);

	// strncpy does not automatically add terminating zero, but strncat does! So start with blank string and concatenate.
	method[0] = 0;
	command[0] = 0;
	parameter[0] = 0;
	//strncat(method, request, commandParameterSubstring-1);
	strncat(command, commandParameterSubstring, parameterSubstring-commandParameterSubstring-1);
	strncat(parameter, parameterSubstring, slash3-parameterSubstring-1);

	Serial.print(parameterSubstring-commandParameterSubstring-1);
	PrintString("command",command);
	PrintString("parameter",parameter);
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