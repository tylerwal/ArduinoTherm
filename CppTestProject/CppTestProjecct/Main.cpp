#define maxRequestLength 120
#include <iostream>

char* sentString = "GET /test/test2/ HTTP/1.1\nSome other stuff\nmore stuff\nEnd";

void GetRequest(char *);

class ParsedRequest
{
public:
	char* httpMethod;
	char* command;
	char* parameter;
};

ParsedRequest* ParseReceivedRequest(char*);

int main()
{
	char request[maxRequestLength];
	GetRequest(request);
	
	std::cout << "\n";
	std::cout << "In main:\n";
	std::cout << request;
	std::cout << "\n";
	std::cout << typeid(request).name();
	std::cout << "\n";

	//parsedRequest = 
	ParseReceivedRequest(request);
	//
	std::cout << "\n";
	system("pause");
}

void GetRequest(char *request)
{
	int requestLength = 0;

	for ( int i = 0; i < strlen(sentString); i++)
	{
		char c = sentString[i];

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

	request[requestLength++] = '\0';
}

ParsedRequest* ParseReceivedRequest(char* request)
{
	ParsedRequest parsedRequest;

	std::cout << "\n";
	std::cout << "In Parse:\n";
	std::cout << request;
	std::cout << "\n";
	std::cout << typeid(request).name();
	std::cout << "\n";

	/*
	// chop off HTTP version
	request[strrchr(request, ' ') - request] = '\0';

	int firstSlashPosition = strstr(request, "/") - request;
	request[firstSlashPosition - 1] = '\0'; // place a null character in 1 index in front of slash
	parsedRequest.command = &request[firstSlashPosition + 1]; // set command equal to the reference of 1 character after the slash

	int secondSlashPosition = strstr(parsedRequest.command, "/") - parsedRequest.command;
	parsedRequest.command[secondSlashPosition] = '\0'; // place a null character at slash location

	parsedRequest.parameter = &parsedRequest.command[secondSlashPosition + 1]; // set parameter equal to the reference of 1 character after the slash
	*/

	return new ParsedRequest;
}