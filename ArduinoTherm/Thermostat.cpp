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


CoolState coolState;
Thermostat thermostat(&coolState);
void PerformPeriodicThermostatUpdate()
{
	// ***************** Update global temperature probe values *****************
	dhtStatus = dht.getStatusString();
	currentHumidity = dht.getHumidity();
	currentTemperature = dht.toFahrenheit(dht.getTemperature());
	
	// ***************** Perform Thermostat Functions *****************
	if ((goalTemperature > currentTemperature) && !isHeatRunning && (thermostat.StateSetting == Auto || thermostat.StateSetting == Heat))
	{
		digitalWrite(9, HIGH);
		digitalWrite(8, LOW); 
		Serial.println("Heat Up");
		thermostat.CurrentState = Heat;
	}
	else if ((goalTemperature < currentTemperature) && !isCoolRunning && (thermostat.StateSetting == Auto || thermostat.StateSetting == Cool))
	{
		digitalWrite(9, LOW); 
		digitalWrite(8, HIGH);
		Serial.println("Cool down");
		thermostat.CurrentState = Cool;
	}
	else
	{
		digitalWrite(9, LOW); 
		digitalWrite(8, LOW);
		Serial.println("Off");
		thermostat.CurrentState = Off;
	}
}

/* ************* Thermostat ************* */

Thermostat::Thermostat(CoolState * state) : coolState(state)
{
	//CoolState cstate;
	//coolState = &cstate;
	//offState = &(CoolState());
	//coolState = &CoolState; //thermostat);
/* 
	currentState = CoolState(*this);
	offState = CoolState(*this);
	fanState = CoolState(*this);
	heatState = CoolState(*this);
	emergencyHeatState = CoolState(*this); */
}; 

unsigned long Thermostat::TimeInCurrentState()
{
	return millis() - this->StartTimeCurrentState;
};

void Thermostat::setCurrentState(IState * state)
{
	currentState = state;
};

IState * Thermostat::getCurrentState()
{
	return currentState;
};

IState * Thermostat::getOffState() 
{
	return offState;
};
IState * Thermostat::getFanState() 
{ 
	return fanState;
};
IState * Thermostat::getHeatState() 
{ 
	return heatState;
};
IState * Thermostat::getCoolState() 
{ 
	return coolState;
};
IState * Thermostat::getEmergencyHeatState() 
{ 
	return emergencyHeatState;
};

/* ************************** Various States ************************** */
/* ************* Off State ************* */
/* ************* Fan State ************* */
/* ************* Heat State ************* */
/* ************* Cool State ************* */
//CoolState::CoolState(){}; //Thermostat * thermostat){};
void CoolState::TemperatureEqualsGoal(){};
void CoolState::TemperatureGreaterThanGoal(){};
void CoolState::TemperatureLessThanGoal(){};
void CoolState::Operate(){};
/* ************* Emergency Heat State ************* */