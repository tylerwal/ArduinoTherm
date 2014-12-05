#ifndef EXTERNALS_H
#define EXTERNALS_H

#if ARDUINO < 100
  #include <WProgram.h>
#else
  #include <Arduino.h>
#endif

#include "DHT.h"

extern float currentTemperature;
extern float goalTemperature;
extern float currentHumidity;
extern const char* dhtStatus;
extern DHT dht;

#define temperatureHysteresis 2 // the amount above or below a threshold that is allowed
#define minRunTime 420000 // cooling minimum runtime allowed (prevent short cycles)
#define minOffTime 420000 // cooling minimum off time before can run again (protect compressor)
/* extern bool isFanEnabled;
extern bool isFanRunning;
extern bool isHeatEnabled;
extern bool isHeatRunning;
extern bool isCoolEnabled;
extern bool isCoolRunning; */
/* extern Thermostat thermostat; */

void PerformPeriodicThermostatUpdate();

enum HvacState 
{
	Off,
	Auto,
	Fan,
	Heat,
	Cool,
	EmergencyHeat
};

class IThermostat
{

};

class Thermostat
{
	public:
		unsigned long StartTimeCurrentState;
		HvacState CurrentState;
		HvacState SetState;

		unsigned long TimeInCurrentState(); // { return millis() - this->StartTimeCurrentState;	}
};

class IState
{
	public:
		virtual void TemperatureEqualsGoal() = 0;
		virtual void TemperatureGreaterThanGoal() = 0;
		virtual void TemperatureLessThanGoal() = 0;
};

class OldThermostat
{
	/* CoolState coolState;
	HeatState heatState;
	OffState offState;
	EmergencyHeatState emergencyHeatState;*/
	IState * state; 
};
 

/*
class CoolState: public IState
{
	public:
		void TemperatureEqualsGoal()
		{ 
		}
		void TemperatureGreaterThanGoal()
		{ 
		}
		void TemperatureLessThanGoal()
		{ 
		}
};

class HeatState: public IState
{
	public:
		void TemperatureEqualsGoal()
		{ 
		}
		void TemperatureGreaterThanGoal()
		{ 
		}
		void TemperatureLessThanGoal()
		{ 
		}
};

class OffState: public IState
{
	private:
		Thermostat thermostat;
	public:
		OffState(Thermostat tstat)
		{
			thermostat = tstat;
		}
		void TemperatureEqualsGoal()
		{ 
		}
		void TemperatureGreaterThanGoal()
		{ 
		}
		void TemperatureLessThanGoal()
		{ 
		}
};

class EmergencyHeatState: public IState
{
	public:
		void TemperatureEqualsGoal()
		{ 
		}
		void TemperatureGreaterThanGoal()
		{ 
		}
		void TemperatureLessThanGoal()
		{ 
		}
};
 */
 
 #endif /* EXTERNALS_H */