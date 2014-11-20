#define maxBufferLength 120
#include <iostream>

char* sentString = "GET /temp/getItNow/ HTTP/1.1\nSome other stuff\nmore stuff\nEnd";

typedef void(*actionMethod)(char*, char*);
void GetHttpRequest(char *);

struct ParsedRequest
{
	char* httpMethod;
	char* command;
	char* parameter;
};

void ParseReceivedRequest(char*, ParsedRequest &);

int main()
{
	char buffer[maxBufferLength];
	GetHttpRequest(buffer);

	ParsedRequest parsedRequest;
	ParseReceivedRequest(buffer, parsedRequest);

	std::cout << "HTTP Method: ";
	std::cout << parsedRequest.httpMethod;
	std::cout << "\n";

	std::cout << "Command: ";
	std::cout << parsedRequest.command;
	std::cout << "\n";

	std::cout << "Parameter: ";
	std::cout << parsedRequest.parameter;
	std::cout << "\n";

	std::cout << "\n";
	system("pause");
}

void GetHttpRequest(char *buffer)
{
	int bufferLength = 0;

	for ( int i = 0; i < strlen(sentString); i++)
	{
		char c = sentString[i];

		if (c == '\n')
		{
			break;
		}
		else
		{
			if (bufferLength < maxBufferLength)
			{
				buffer[bufferLength++] = c;
			}
			else
			{
				break;
			}
		}
	}

	buffer[bufferLength++] = '\0';
}

// Parses the buffer into the HTTP method (request), command, and paramter variables
// Received buffer templace: "METHOD /command/parameter HTTP/1.1"
void ParseReceivedRequest(char* buffer, ParsedRequest & parsedRequest)
{
	// chop off HTTP version
	buffer[strrchr(buffer, ' ') - buffer] = '\0';
	parsedRequest.httpMethod = buffer;

	int firstSlashPosition = strstr(buffer, "/") - buffer;
	// place a null character in 1 index in front of slash
	buffer[firstSlashPosition - 1] = '\0';
	// set command equal to the reference of 1 character after the slash
	parsedRequest.command = &buffer[firstSlashPosition + 1];

	int secondSlashPosition = strstr(parsedRequest.command, "/") - parsedRequest.command;
	parsedRequest.command[secondSlashPosition] = '\0';

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

	action(parsedRequest.command, parsedRequest.parameter);
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

void PrintHttpHeader(char* code)
{
	std::cout <<"HTTP/1.1 ";
	std::cout << code;
	std::cout << "Content-Type: text/html";
	std::cout << "Connection: close";
}