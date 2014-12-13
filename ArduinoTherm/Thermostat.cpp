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
IOperationalMode * offState = new OffState();
IOperationalMode * autoState = new AutoState();
IOperationalMode * fanState = new FanState();
IOperationalMode * heatState = new HeatState();
IOperationalMode * coolState = new CoolState();
IOperationalMode * emergencyHeatState = new EmergencyHeatState();
IOperationalMode * autoWithEmergencyHeatState = new AutoWithEmergencyHeatState();

void InitiateThermostat()
{
	thermostat.StartTimeCurrentState = millis();
	thermostat.setCurrentState(autoState);
}

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
	//setCurrentState(offState);
}; 

unsigned long Thermostat::TimeInCurrentState()
{
	return millis() - this->StartTimeCurrentState;
};

void Thermostat::setCurrentState(IOperationalMode * state)
{
	currentState = state;
};

IOperationalMode * Thermostat::getCurrentState()
{
	return currentState;
};

/* ************************** Various States ************************** */
/* ************* Off State ************* */
void OffState::TemperatureEqualsGoal(){};
void OffState::TemperatureGreaterThanGoal()
{
	Serial.println("temp greater than goal");
};
void OffState::TemperatureLessThanGoal()
{
	Serial.println("temp less than goal");
};

/* ************* Auto State ************* */
void AutoState::TemperatureEqualsGoal(){};
void AutoState::TemperatureGreaterThanGoal(){};
void AutoState::TemperatureLessThanGoal(){};

/* ************* Fan State ************* */
void FanState::TemperatureEqualsGoal(){};
void FanState::TemperatureGreaterThanGoal(){};
void FanState::TemperatureLessThanGoal(){};

/* ************* Heat State ************* */
void HeatState::TemperatureEqualsGoal(){};
void HeatState::TemperatureGreaterThanGoal(){};
void HeatState::TemperatureLessThanGoal(){};

/* ************* Cool State ************* */
void CoolState::TemperatureEqualsGoal(){};
void CoolState::TemperatureGreaterThanGoal(){};
void CoolState::TemperatureLessThanGoal(){};

/* ************* Emergency Heat State ************* */
void EmergencyHeatState::TemperatureEqualsGoal(){};
void EmergencyHeatState::TemperatureGreaterThanGoal(){};
void EmergencyHeatState::TemperatureLessThanGoal(){};

/* ************* AutoWithEmergency Heat State ************* */
void AutoWithEmergencyHeatState::TemperatureEqualsGoal(){};
void AutoWithEmergencyHeatState::TemperatureGreaterThanGoal(){};
void AutoWithEmergencyHeatState::TemperatureLessThanGoal(){};