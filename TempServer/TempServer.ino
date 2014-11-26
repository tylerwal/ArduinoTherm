#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"
#include "TimerOne.h"
#include "Structs.h"

#define maxBufferLength 120
#define DhtUpdateInterval 3000000

// ************ Temp/Humidity Probe **************
DHT dht;
float currentTemperature;
float goalTemperature;
float currentHumidity;
const char* dhtStatus;

// ************ Thermostat ************
bool isFanRunning;
bool isHeatEnabled;
bool isHeatRunning;
bool isCoolEnabled;
bool isCoolRunning;

// ************ Client/Server ************
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);
EthernetClient client;

// ************ Declarations ************
typedef void (*actionMethod)(const char*, const char*);
void GetRequest(char *);
void ParseReceivedRequest(const char*, ParsedRequest &);

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
	Timer1.attachInterrupt(PerformPeriodicThermostatUpdate);
	
	pinMode(13, OUTPUT);
	pinMode(12, OUTPUT);
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
		PrintHttpHeader("404 Not found");
		return;
	}
	
	action(parsedRequest.command, parsedRequest.parameter);
}

void PerformPeriodicThermostatUpdate()
{
	// ***************** Update global temperature probe values *****************
	dhtStatus = dht.getStatusString();
	currentHumidity = dht.getHumidity();
	currentTemperature = dht.toFahrenheit(dht.getTemperature());
	
	// ***************** Perform Thermostat Functions *****************
	if (goalTemperature > currentTemperature)
	{
		digitalWrite(13, HIGH);
		digitalWrite(12, LOW); 
		Serial.println("Heat Up");
	}
	else if (goalTemperature < currentTemperature)
	{
		digitalWrite(13, LOW); 
		digitalWrite(12, HIGH);
		Serial.println("Cool down");
	}
}

void PerformGet(const char* command, const char* parameter)
{
	PrintHttpHeader("200 OK");
	
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
	else if (CompareStrings("ResourceDirectory", command))
	{
		client.print("Resource Directory:<br><br>Get: Temp, GoalTemperature, Humidity, Status<br>Put: GoalTemperature");
	}
	else
	{
		client.print("404");
	}
}
	
void PerformPut(const char* command, const char* parameter)
{
	PrintHttpHeader("200 OK");
	
	if (CompareStrings("GoalTemperature", command))
	{
		goalTemperature = atof(parameter);
		client.print(goalTemperature);
	}
}
	
bool CompareStrings(const char* one, const char* two)
{
	return strcmp(one, two) == 0;
}

char* FloatToString(float input)
{
	char buffer[6];
	
	itoa(input * 10, buffer, 10);
	int inputLength = strlen(buffer);
	buffer[inputLength + 1] = buffer[inputLength];
	buffer[inputLength] = buffer[inputLength - 1];
	buffer[inputLength - 1] = '.';

	return buffer;
}
	
void PrintHttpHeader(const char* statusCode)
{
	client.print("HTTP/1.1 ");
	client.println(statusCode);
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
