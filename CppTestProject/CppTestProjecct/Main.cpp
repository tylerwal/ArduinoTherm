#define maxRequestLength 120
#include <iostream>

char* sentString = "GET /test/test2/ HTTP/1.1\nSome other stuff\nmore stuff\nEnd";

char* WaitForRequest();

class ParsedRequest
{
public:
	char* httpMethod;
	char* command;
	char* parameter;
};

ParsedRequest& ParseReceivedRequest(char*, ParsedRequest);

int main()
{
	std::cout << "Hello World!";
	std::cout << "\n";
	ParsedRequest parsedRequest;
	char* request = WaitForRequest();
	parsedRequest = ParseReceivedRequest(request, parsedRequest);
	//std::cout << request;
	std::cout << "\n";
	system("pause");
}

char* WaitForRequest()
{
	int requestLength = 0;
	char request[maxRequestLength];

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

	std::cout << request;

	return &request[0];
}

ParsedRequest& ParseReceivedRequest(char* request, ParsedRequest parsedRequest)
{
	//ParsedRequest parsedRequest;

	// chop off HTTP version
	request[strrchr(request, ' ') - request] = '\0';

	int firstSlashPosition = strstr(request, "/") - request;
	request[firstSlashPosition - 1] = '\0'; // place a null character in 1 index in front of slash
	parsedRequest.command = &request[firstSlashPosition + 1]; // set command equal to the reference of 1 character after the slash

	int secondSlashPosition = strstr(parsedRequest.command, "/") - parsedRequest.command;
	parsedRequest.command[secondSlashPosition] = '\0'; // place a null character at slash location

	parsedRequest.parameter = &parsedRequest.command[secondSlashPosition + 1]; // set parameter equal to the reference of 1 character after the slash

	return parsedRequest;
}