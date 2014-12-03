#ifndef EXTERNALS_H
#define EXTERNALS_H

#if ARDUINO < 100
  #include <WProgram.h>
#else
  #include <Arduino.h>
#endif

float currentTemperature;
float goalTemperature;
float currentHumidity;
const char* dhtStatus;

#define temperatureHysteresis 2 // the amount above or below a threshold that is allowed
#define minRunTime 420000 // cooling minimum runtime allowed (prevent short cycles)
#define minOffTime 420000 // cooling minimum off time before can run again (protect compressor)
bool isFanEnabled;
bool isFanRunning;
bool isHeatEnabled;
bool isHeatRunning;
bool isCoolEnabled;
bool isCoolRunning;
/* SystemState systemState; */

enum HvacState 
{
	Off,
	Fan,
	Heat,
	Cool,
	EmergencyHeat
};

class SystemState
{
	public:
		unsigned long StartTimeCurrentState;
		HvacState HvacCurrentState;

		unsigned long TimeInCurrentState() { return millis() - this->StartTimeCurrentState;	}
};

class IState
{
	public:
		virtual void TemperatureEqualsGoal() = 0;
		virtual void TemperatureGreaterThanGoal() = 0;
		virtual void TemperatureLessThanGoal() = 0;
};

class Thermostat
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