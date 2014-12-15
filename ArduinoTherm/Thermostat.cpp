#include "Arduino.h"
#include "Thermostat.h"

#define HeatOutputPin 9
#define CoolOutputPin 8
#define FanOutputPin 7
#define EmergencyHeatOutputPin 6

float currentTemperature;
float goalTemperature;
float currentHumidity;
const char* dhtStatus;

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
		digitalWrite(HeatOutputPin, HIGH);
		digitalWrite(CoolOutputPin, LOW); 
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
		digitalWrite(HeatOutputPin, LOW); 
		digitalWrite(CoolOutputPin, HIGH);
		Serial.println("Cool down");
		thermostat.CurrentState = Cool;
	}
	else
	{
		digitalWrite(HeatOutputPin, LOW); 
		digitalWrite(CoolOutputPin, LOW);
		Serial.println("Off");
		thermostat.CurrentState = Off;
	} */
}

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

void Thermostat::StartCool()
{
};
void Thermostat::StartHeat()
{
};
void Thermostat::StartEmergencyHeat()
{
};
void Thermostat::StartFan()
{
};
void Thermostat::StopAll()
{
	digitalWrite(HeatOutputPin, LOW); 
	digitalWrite(CoolOutputPin, LOW);
	digitalWrite(FanOutputPin, LOW);
	digitalWrite(EmergencyHeatOutputPin, LOW);
	Serial.println("Off");
	thermostat.CurrentState = Off;
};

/* ************************** Various Modes ************************** */
/* ************* Off Mode ************* */
void OffMode::Operate()
{
	thermostat.StopAll();
};

/* ************* Auto Mode ************* */
void AutoMode::Operate()
{
	digitalWrite(HeatOutputPin, HIGH); 
	digitalWrite(CoolOutputPin, HIGH);
	Serial.println("Auto");
	thermostat.CurrentState = Auto;
};

/* ************* Fan Mode ************* */
void FanMode::Operate()
{
	digitalWrite(HeatOutputPin, HIGH); 
	digitalWrite(CoolOutputPin, HIGH);
	Serial.println("Auto");
	thermostat.CurrentState = Auto;
};

/* ************* Heat Mode ************* */
void HeatMode::Operate()
{
};

/* ************* Cool Mode ************* */
void CoolMode::Operate()
{
};

/* ************* Emergency Heat Mode ************* */
void EmergencyHeatMode::Operate()
{
};

/* ************* AutoWithEmergency Heat Mode ************* */
void AutoWithEmergencyHeatMode::Operate()
{
};
