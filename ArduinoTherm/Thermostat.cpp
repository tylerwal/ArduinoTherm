#include "Arduino.h"
#include "Thermostat.h"

float currentTemperature;
float goalTemperature;
float currentHumidity;
const char* dhtStatus;

bool isFanEnabled;
bool isFanRunning;
bool isHeatEnabled;
bool isHeatRunning;
bool isCoolEnabled;
bool isCoolRunning;

SystemState systemState;

unsigned long SystemState::TimeInCurrentState()
{
	return millis() - this->StartTimeCurrentState;
};

void PerformPeriodicThermostatUpdate()
{
	// ***************** Update global temperature probe values *****************
	dhtStatus = dht.getStatusString();
	currentHumidity = dht.getHumidity();
	currentTemperature = dht.toFahrenheit(dht.getTemperature());
	
	// ***************** Perform Thermostat Functions *****************
	if ((goalTemperature > currentTemperature) && !isHeatRunning && (systemState.SetState == Auto || systemState.SetState == Heat))
	{
		digitalWrite(9, HIGH);
		digitalWrite(8, LOW); 
		Serial.println("Heat Up");
		systemState.CurrentState = Heat;
	}
	else if ((goalTemperature < currentTemperature) && !isCoolRunning && (systemState.SetState == Auto || systemState.SetState == Cool))
	{
		digitalWrite(9, LOW); 
		digitalWrite(8, HIGH);
		Serial.println("Cool down");
		systemState.CurrentState = Cool;
	}
	else
	{
		digitalWrite(9, LOW); 
		digitalWrite(8, LOW);
		Serial.println("Off");
		systemState.CurrentState = Off;
	}
}
