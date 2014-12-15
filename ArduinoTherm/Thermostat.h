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
		virtual void Operate() = 0;
				
		HvacState associatedHvacState;
};

class OffMode;
class AutoMode;
class FanMode;
class HeatMode;
class CoolMode;
class EmergencyHeatMode;
class AutoWithEmergencyHeatMode;

class Thermostat
{
	private:
		IOperationalMode * currentMode;
	public:	
		unsigned long StartTimeCurrentState;
		unsigned long TimeInCurrentState();
		
		void StartCool();
		void StartHeat();
		void StartEmergencyHeat();
		void StartFan();
		void StopAll();

		// ******************** desired state ********************
		HvacState StateSetting;

		// ******************** current state ********************
		HvacState CurrentState;
	
		/* Thermostat(); */
		void setCurrentMode(IOperationalMode * mode);
		IOperationalMode * getCurrentMode();
};

class OffMode: public IOperationalMode
{
	public:
		void Operate();
};
class AutoMode: public IOperationalMode
{
	public:
		void Operate();
};
class FanMode: public IOperationalMode
{
	public:
		void Operate();
};
class HeatMode: public IOperationalMode
{
	public:
		void Operate();
};
class CoolMode: public IOperationalMode
{
	public:
		void Operate();
};
class EmergencyHeatMode: public IOperationalMode
{
	public:
		void Operate();
};
class AutoWithEmergencyHeatMode: public IOperationalMode
{
	public:
		void Operate();
};
 
 #endif /* EXTERNALS_H */