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

void InitiateThermostat();
void PerformPeriodicThermostatUpdate();

enum HvacState 
{
	Off,
	Auto,
	Fan,
	Heat,
	Cool,
	EmergencyHeat,
	AutoWithEmergencyHeat
};

class IOperationalMode;

class IOperationalMode
{
	public:
		virtual void TemperatureEqualsGoal() = 0;
		virtual void TemperatureGreaterThanGoal() = 0;
		virtual void TemperatureLessThanGoal() = 0;
				
		HvacState associatedHvacState;
};

class OffState;
class AutoState;
class FanState;
class HeatState;
class CoolState;
class EmergencyHeatState;
class AutoWithEmergencyHeatState;

class Thermostat
{
	private:
		IOperationalMode * currentState;
	public:	
		unsigned long StartTimeCurrentState;
		unsigned long TimeInCurrentState();

		// ******************** desired state ********************
		HvacState StateSetting;

		// ******************** current state ********************
		HvacState CurrentState;
	
		Thermostat();
		void setCurrentState(IOperationalMode * state);
		IOperationalMode * getCurrentState();
};

class OffState: public IOperationalMode
{
	public:
		void TemperatureEqualsGoal();
		void TemperatureGreaterThanGoal();
		void TemperatureLessThanGoal();
};
class AutoState: public IOperationalMode
{
	public:
		void TemperatureEqualsGoal();
		void TemperatureGreaterThanGoal();
		void TemperatureLessThanGoal();
};
class FanState: public IOperationalMode
{
	public:
		void TemperatureEqualsGoal();
		void TemperatureGreaterThanGoal();
		void TemperatureLessThanGoal();
};
class HeatState: public IOperationalMode
{
	public:
		void TemperatureEqualsGoal();
		void TemperatureGreaterThanGoal();
		void TemperatureLessThanGoal();
};
class CoolState: public IOperationalMode
{
	public:
		void TemperatureEqualsGoal();
		void TemperatureGreaterThanGoal();
		void TemperatureLessThanGoal();
};
class EmergencyHeatState: public IOperationalMode
{
	public:
		void TemperatureEqualsGoal();
		void TemperatureGreaterThanGoal();
		void TemperatureLessThanGoal();
};
class AutoWithEmergencyHeatState: public IOperationalMode
{
	public:
		void TemperatureEqualsGoal();
		void TemperatureGreaterThanGoal();
		void TemperatureLessThanGoal();
};
 
 #endif /* EXTERNALS_H */