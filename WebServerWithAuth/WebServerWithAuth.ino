#include <SPI.h>
#include <Ethernet.h>
#include <string.h>
#include "DHT.h"
#include "TimerOne.h"

#define bufferMax 128
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

int bufferSize;
char buffer[bufferMax];

char cmd[15];
char param1[15];
char param2[15];

void setup()
{
	//test
	//pinMode(13, OUTPUT);
	
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
	//test
	//digitalWrite(13, LOW);
	client = server.available();
	if (client)
	{
		WaitForRequest(client);
		ParseReceivedRequest();
		PerformRequestedCommands();
		
		client.stop();
	}
}

void WaitForRequest(EthernetClient client)
{
	bufferSize = 0;
	
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
				if (bufferSize < bufferMax)
				{
					buffer[bufferSize++] = c;
				}
				else
				{
					break;
				}
			}
		}
	}
	
	PrintNumber("bufferSize", bufferSize);
}

void ParseReceivedRequest()
{
	Serial.println("in ParseReceivedRequest");
	Serial.println(buffer);
	
	//Received buffer contains "GET /cmd/param1/param2 HTTP/1.1".  Break it up.
	char* slash1;
	char* slash2;
	char* slash3;
	char* space2;
	
	slash1 = strstr(buffer, "/") + 1; // Look for first slash
	slash2 = strstr(slash1, "/") + 1; // second slash
	slash3 = strstr(slash2, "/") + 1; // third slash
	space2 = strstr(slash2, " ") + 1; // space after second slash (in case there is no third slash)
	
	if (slash3 > space2) 
	{
		slash3=slash2;
	}
	
	PrintString("slash1",slash1);
	PrintString("slash2",slash2);
	PrintString("slash3",slash3);
	PrintString("space2",space2);
	
	// strncpy does not automatically add terminating zero, but strncat does! So start with blank string and concatenate.
	cmd[0] = 0;
	param1[0] = 0;
	param2[0] = 0;
	strncat(cmd, slash1, slash2-slash1-1);
	strncat(param1, slash2, slash3-slash2-1);
	strncat(param2, slash3, space2-slash3-1);
	
	PrintString("cmd",cmd);
	PrintString("param1",param1);
	PrintString("param2",param2);
}

void PerformRequestedCommands()
{
	if ( strcmp(cmd,"getTemp") == 0 ) 
	{
		GetTemp();
	}
	else if ( strcmp(cmd,"setTemp") == 0 ) 
	{
		SetTemp();
	}
	else
	{
		PrintHttpHeader("HTTP/1.1 404 Not found"); 
	}
}

void UpdateTempValues()
{
	dhtStatus = dht.getStatusString();
	humidity = dht.getHumidity();
	temperature = dht.getTemperature();
}

void GetTemp()
{
	PrintHttpHeader("HTTP/1.1 200 OK");
	
	dhtStatus = dht.getStatusString();
	humidity = dht.getHumidity();
	temperature = dht.getTemperature();
	
	if (strcmp(param1, "temp") == 0) 
	{
		client.print(dht.toFahrenheit(temperature), 1);
	}
	else if (strcmp(param1, "humidity") == 0) 
	{
		client.print(humidity, 1);    
	}
	else if (strcmp(param1, "status") == 0) 
	{
		client.print(dhtStatus);   
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

void SetTemp()
{
	PrintHttpHeader("HTTP/1.1 200 OK");
	
	if (strcmp(param1, "current") == 0) 
	{
		client.print("Desired Temp Set At: ");
	}
	else
	{
		desiredTemp = atof(param1);
		client.print("Desired Temp Set To: ");
	}

	client.print(desiredTemp);
	client.println("");
}

void PrintString(char* label, char* str)
{
	Serial.print(label);
	Serial.print("=");
	Serial.println(str);
}

void PrintNumber(char* label, int number)
{
	Serial.print(label);
	Serial.print("=");
	Serial.println(number, DEC);
}

void PrintHttpHeader(String code)
{
	client.println(code);
	client.println("Content-Type: text/html");
	client.println("Connection: close");  // the connection will be closed after completion of the response
	client.println(); 
}
