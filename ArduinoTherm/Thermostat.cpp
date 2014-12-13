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
	if (
		(
			goalTemperature > currentTemperature) && 
			!isHeatRunning && 
			(thermostat.StateSetting == Auto || thermostat.StateSetting == Heat)
		)
	{
		digitalWrite(9, HIGH);
		digitalWrite(8, LOW); 
		Serial.println("Heat Up");
		thermostat.CurrentState = Heat;
		//thermostat.setCurrentState(thermostat.getOffState());
		//Serial.println("Class: ");
		thermostat.getCurrentState()->TemperatureLessThanGoal();
	}
	else if 
		(
			(goalTemperature < currentTemperature) && 
			!isCoolRunning && 
			(thermostat.StateSetting == Auto || thermostat.StateSetting == Cool)
		)
	{
		digitalWrite(9, LOW); 
		digitalWrite(8, HIGH);
		Serial.println("Cool down");
		//thermostat.CurrentState = Cool;
		thermostat.getCurrentState()->TemperatureGreaterThanGoal();
	}
	else
	{
		digitalWrite(9, LOW); 
		digitalWrite(8, LOW);
		Serial.println("Off");
		thermostat.CurrentState = Off;
		//thermostat.getCurrentState()->TemperatureLessThanGoal();
	}
}

/* ************* Thermostat ************* */

Thermostat::Thermostat()
{
	offState = new OffState(this);	
	fanState = new FanState(this);	
	heatState = new HeatState(this);	
	coolState = new CoolState(this);	
	emergencyHeatState = new EmergencyHeatState(this);
	
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
void OffState::TemperatureGreaterThanGoal()
{
	Serial.println("temp greater than goal");
};
void OffState::TemperatureLessThanGoal()
{
	Serial.println("temp less than goal");
};

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