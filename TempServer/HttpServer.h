#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#if ARDUINO < 100
  #include <WProgram.h>
#else
  #include <Arduino.h>
#endif

#define maxBufferLength 120

struct ParsedRequest
{
	char* httpMethod;
	char* command;
	char* parameter;
};

void GetHttpRequest(EthernetClient client, char* buffer);

/* void GetHttpRequest(EthernetClient client, char* buffer)
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
} */

#endif /* HTTPSERVER_H */