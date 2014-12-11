void PerformPeriodicThermostatUpdate();

class IState;

class IThermostat
{
public:
	virtual void setCurrentState(IState * state) = 0;
	virtual IState * getCurrentState() = 0;

	virtual IState * getOffState() = 0;
};

class IState
{
public:
	virtual void TemperatureEqualsGoal() = 0;
	virtual void TemperatureGreaterThanGoal() = 0;
	virtual void TemperatureLessThanGoal() = 0;
};

class OffState;

class Thermostat : IThermostat
{
private:
	IState * currentState;

	IState * offState;
public:
	Thermostat();

	unsigned long StartTimeCurrentState;
	unsigned long TimeInCurrentState();

	void setCurrentState(IState * state);
	IState * getCurrentState();

	IState * getOffState();
};

class OffState : public IState
{
public:
	OffState(IThermostat * thermostat);
	void TemperatureEqualsGoal();
	void TemperatureGreaterThanGoal();
	void TemperatureLessThanGoal();
};