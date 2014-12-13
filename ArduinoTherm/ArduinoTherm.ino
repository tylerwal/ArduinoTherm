#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"
#include "TimerOne.h"
#include "Thermostat.h"
#include "HttpServer.h"

// ************ Temp/Humidity Probe **************
#define DhtUpdateInterval 3000000 // how often the temp probe's values are used to update global variables
DHT dht;
extern float currentTemperature;
extern float goalTemperature;
extern float currentHumidity;
extern const char* dhtStatus;

// ************ Thermostat ************
/* 
bool isFanEnabled;
bool isFanRunning;
bool isHeatEnabled; 
*/
extern bool isHeatRunning;
/* bool isCoolEnabled; */
extern bool isCoolRunning;
extern Thermostat thermostat;

// ************ Client/Server ************
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);
EthernetClient client;

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

	InitiateThermostat();
	/* thermostat.StartTimeCurrentState = millis(); */
	/* thermostat.CurrentState = Off; */
	thermostat.StateSetting = Auto;

	goalTemperature = 72.0; // set a default in case there is a restart
	
	Timer1.initialize(DhtUpdateInterval);
	Timer1.attachInterrupt(PerformPeriodicThermostatUpdate);

	pinMode(8, OUTPUT);
	pinMode(9, OUTPUT);
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
		
		PerformRequestedCommand(client, parsedRequest);
		
		client.stop();
	}
}
