#include "Thermostat.h"
#include <iostream>

Thermostat thermostat;

int main()
{
	thermostat.setCurrentState(thermostat.getOffState());
	PerformPeriodicThermostatUpdate();
}

void PerformPeriodicThermostatUpdate()
{
	IState * currentState = thermostat.getCurrentState();
	currentState->TemperatureEqualsGoal();
}

Thermostat::Thermostat()
{
	OffState ostate(this);
	offState = &ostate;
	
	setCurrentState(offState);
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