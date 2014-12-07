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

Thermostat thermostat;

void PerformPeriodicThermostatUpdate()
{
	// ***************** Update global temperature probe values *****************
	dhtStatus = dht.getStatusString();
	currentHumidity = dht.getHumidity();
	currentTemperature = dht.toFahrenheit(dht.getTemperature());
	
	// ***************** Perform Thermostat Functions *****************
	if ((goalTemperature > currentTemperature) && 
		!isHeatRunning && 
		(thermostat.StateSetting == Auto || thermostat.StateSetting == Heat)
		)
	{
		digitalWrite(9, HIGH);
		digitalWrite(8, LOW); 
		Serial.println("Heat Up");
		thermostat.CurrentState = Heat;
		thermostat.getCurrentState()->TemperatureLessThanGoal();
	}
	else if ((goalTemperature < currentTemperature) && 
		!isCoolRunning && 
		(thermostat.StateSetting == Auto || thermostat.StateSetting == Cool)
		)
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

Thermostat::Thermostat()
{
	OffState ostate(this);
	offState = &ostate;
	
	FanState fstate(this);
	fanState = &fstate;
	
	HeatState hstate(this);
	heatState = &hstate;
	
	CoolState cstate(this);
	coolState = &cstate;
	
	EmergencyHeatState ehstate(this);
	emergencyHeatState = &ehstate;
	
	setCurrentState(offState);
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
OffState::OffState(IThermostat * thermostat){};
void OffState::TemperatureEqualsGoal(){};
void OffState::TemperatureGreaterThanGoal(){};
void OffState::TemperatureLessThanGoal(){};

/* ************* Fan State ************* */
FanState::FanState(IThermostat * thermostat){};
void FanState::TemperatureEqualsGoal(){};
void FanState::TemperatureGreaterThanGoal(){};
void FanState::TemperatureLessThanGoal(){};

/* ************* Heat State ************* */
HeatState::HeatState(IThermostat * thermostat){};
void HeatState::TemperatureEqualsGoal(){};
void HeatState::TemperatureGreaterThanGoal(){};
void HeatState::TemperatureLessThanGoal(){};

/* ************* Cool State ************* */
CoolState::CoolState(IThermostat * thermostat){};
void CoolState::TemperatureEqualsGoal(){};
void CoolState::TemperatureGreaterThanGoal(){};
void CoolState::TemperatureLessThanGoal(){};

/* ************* Emergency Heat State ************* */
EmergencyHeatState::EmergencyHeatState(IThermostat * thermostat){};
void EmergencyHeatState::TemperatureEqualsGoal(){};
void EmergencyHeatState::TemperatureGreaterThanGoal(){};
void EmergencyHeatState::TemperatureLessThanGoal(){};