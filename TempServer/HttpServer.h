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

typedef void (*actionMethod)(EthernetClient client, const char*, const char*);

void GetHttpRequest(EthernetClient client, char* buffer);

void ParseReceivedRequest(char* buffer, ParsedRequest & parsedRequest);

void PrintHttpHeader(EthernetClient client, const char* statusCode);

void PerformRequestedCommand(EthernetClient client, ParsedRequest & parsedRequest);

void PerformGet(EthernetClient client, const char* command, const char* parameter);

void PerformPut(EthernetClient client, const char* command, const char* parameter);

#endif /* HTTPSERVER_H */