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
	EmergencyHeat,
	AutoWithEmergencyHeat
};

class IState;

class IThermostat
{
	public:
		virtual void setCurrentState(IState * state) = 0;
		virtual IState * getCurrentState() = 0;
		
		virtual IState * getOffState() = 0;
		virtual IState * getFanState() = 0;
		virtual IState * getHeatState() = 0;
		virtual IState * getCoolState() = 0;
		virtual IState * getEmergencyHeatState() = 0;
};

class IState
{
	public:
		virtual void TemperatureEqualsGoal() = 0;
		virtual void TemperatureGreaterThanGoal() = 0;
		virtual void TemperatureLessThanGoal() = 0;
				
		HvacState associatedHvacState;
};

class OffState;
class FanState;
class HeatState;
class CoolState;
class EmergencyHeatState;

class Thermostat : IThermostat
{
	private:
		IState * currentState;

		IState * offState;
		IState * fanState;
		IState * heatState;
		IState * coolState;
		IState * emergencyHeatState;
	public:
		Thermostat(); //IState * cool);
	
		unsigned long StartTimeCurrentState;
		unsigned long TimeInCurrentState();

		// ******************** desired state ********************
		HvacState StateSetting;

		// ******************** current state ********************
		HvacState CurrentState;
	
		void setCurrentState(IState * state);
		IState * getCurrentState();
		
		IState * getOffState();
		IState * getFanState();
		IState * getHeatState();
		IState * getCoolState();
		IState * getEmergencyHeatState();
};

class OffState: public IState
{
	public:
		OffState(IThermostat * thermostat);
		void TemperatureEqualsGoal();
		void TemperatureGreaterThanGoal();
		void TemperatureLessThanGoal();
};
class FanState: public IState
{
	public:
		FanState(IThermostat * thermostat);
		void TemperatureEqualsGoal();
		void TemperatureGreaterThanGoal();
		void TemperatureLessThanGoal();
};
class HeatState: public IState
{
	public:
		HeatState(IThermostat * thermostat);
		void TemperatureEqualsGoal();
		void TemperatureGreaterThanGoal();
		void TemperatureLessThanGoal();
};
class CoolState: public IState
{
	public:
		CoolState(IThermostat * thermostat);
		void TemperatureEqualsGoal();
		void TemperatureGreaterThanGoal();
		void TemperatureLessThanGoal();
};
class EmergencyHeatState: public IState
{
	public:
		EmergencyHeatState(IThermostat * thermostat);
		void TemperatureEqualsGoal();
		void TemperatureGreaterThanGoal();
		void TemperatureLessThanGoal();
};
 
 #endif /* EXTERNALS_H */