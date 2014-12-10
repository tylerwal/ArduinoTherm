//#include "Arduino.h"
#include "Thermostat.h"
#include <iostream>

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

int main()
{
	PerformPeriodicThermostatUpdate();
}

void PerformPeriodicThermostatUpdate()
{
	// ***************** Update global temperature probe values *****************
	//dhtStatus = dht.getStatusString();
	//currentHumidity = dht.getHumidity();
	//currentTemperature = dht.toFahrenheit(dht.getTemperature());

	// ***************** Perform Thermostat Functions *****************
	if (
		(goalTemperature > currentTemperature) &&
		!isHeatRunning &&
		(thermostat.StateSetting == Auto || thermostat.StateSetting == Heat)
		)
	{
		//digitalWrite(9, HIGH);
		//digitalWrite(8, LOW);
		//Serial.println("Heat Up");
		std::cout << "Heat Up";
		thermostat.CurrentState = Heat;
		thermostat.setCurrentState(thermostat.getOffState());
		std::cout << "Class ";
		thermostat.getCurrentState()->TemperatureLessThanGoal();
	}
	else if (
		(goalTemperature < currentTemperature) &&
		!isCoolRunning &&
		(thermostat.StateSetting == Auto || thermostat.StateSetting == Cool)
		)
	{
		//digitalWrite(9, LOW);
		//digitalWrite(8, HIGH);
		std::cout << "Cool down";
		thermostat.CurrentState = Cool;
		thermostat.getCurrentState()->TemperatureGreaterThanGoal();
	}
	else
	{
		//digitalWrite(9, LOW);
		//digitalWrite(8, LOW);
		std::cout << "Off";
		thermostat.CurrentState = Off;
		//thermostat.getCurrentState()->TemperatureLessThanGoal();
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
	//return millis() - 
	return this->StartTimeCurrentState;
};

void Thermostat::setCurrentState(IState * state)
{
	currentState = state;
};

IState * Thermostat::getCurrentState()
{
	std::cout << "get current state";
	std::cout << currentState->something();
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
	//Serial.println("temp greater than goal");
};
void OffState::TemperatureLessThanGoal()
{
	//Serial.println("temp less than goal");
};
char* something()
{
	return "something in test!!!";
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