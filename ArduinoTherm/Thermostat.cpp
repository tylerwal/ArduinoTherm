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
IOperationalMode * offMode = new OffMode();
IOperationalMode * autoMode = new AutoMode();
IOperationalMode * fanMode = new FanMode();
IOperationalMode * heatMode = new HeatMode();
IOperationalMode * coolMode = new CoolMode();
IOperationalMode * emergencyHeatMode = new EmergencyHeatMode();
IOperationalMode * autoWithEmergencyHeatMode = new AutoWithEmergencyHeatMode();

void InitiateThermostat()
{
	offMode->associatedHvacState = Off;
	autoMode->associatedHvacState = Auto;
	fanMode->associatedHvacState = Fan;
	heatMode->associatedHvacState = Heat;
	coolMode->associatedHvacState = Cool;
	emergencyHeatMode->associatedHvacState = EmergencyHeat;
	autoWithEmergencyHeatMode->associatedHvacState = AutoWithEmergencyHeat;
	
	thermostat.StartTimeCurrentState = millis();
	thermostat.setCurrentMode(autoMode);
}

void PerformPeriodicThermostatUpdate()
{
	// ***************** Update global temperature probe values *****************
	dhtStatus = dht.getStatusString();
	currentHumidity = dht.getHumidity();
	currentTemperature = dht.toFahrenheit(dht.getTemperature());
	
	thermostat.getCurrentMode()->Operate();
	
	/* // ***************** Perform Thermostat Functions *****************
	if (
		(
			goalTemperature > currentTemperature) && 
			!isHeatRunning && 
			(thermostat.ModeSetting == Auto || thermostat.ModeSetting == Heat)
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
			(thermostat.ModeSetting == Auto || thermostat.ModeSetting == Cool)
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
	} */
}

/* ************* Thermostat ************* */
Thermostat::Thermostat()
{
	//setCurrentMode(offMode);
}; 

unsigned long Thermostat::TimeInCurrentState()
{
	return millis() - this->StartTimeCurrentState;
};

void Thermostat::setCurrentMode(IOperationalMode * mode)
{
	currentMode = mode;
};

IOperationalMode * Thermostat::getCurrentMode()
{
	return currentMode;
};

/* ************************** Various Modes ************************** */
/* ************* Off Mode ************* */
void OffMode::Operate()
{
	digitalWrite(9, LOW); 
	digitalWrite(8, LOW);
	Serial.println("Off");
	thermostat.CurrentState = Off;
};

/* ************* Auto Mode ************* */
void AutoMode::Operate()
{
	digitalWrite(9, HIGH); 
	digitalWrite(8, HIGH);
	Serial.println("Auto");
	thermostat.CurrentState = Auto;
};

/* ************* Fan Mode ************* */
void FanMode::Operate(){};

/* ************* Heat Mode ************* */
void HeatMode::Operate(){};

/* ************* Cool Mode ************* */
void CoolMode::Operate(){};

/* ************* Emergency Heat Mode ************* */
void EmergencyHeatMode::Operate(){};

/* ************* AutoWithEmergency Heat Mode ************* */
void AutoWithEmergencyHeatMode::Operate(){};