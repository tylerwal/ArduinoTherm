void LogicFunction();

class IState;

class IContainer
{
public:
	virtual void setCurrentState(IState * state) = 0;
	virtual IState * getCurrentState() = 0;

	virtual IState * getStateA() = 0;
	virtual IState * getStateB() = 0;
};

class IState
{
public:
	virtual void MethodOne() = 0;
	virtual void MethodTwo() = 0;
};

class StateA;
class StateB;

class Container : IContainer
{
private:
	IState * currentState;

	IState * stateA;
	IState * stateB;
public:
	Container();
	
	void setCurrentState(IState * state);
	IState * getCurrentState();

	IState * getStateA();
	IState * getStateB();
};

class StateA : public IState
{
public:
	StateA(IContainer * thermostat);
	void MethodOne();
	void MethodTwo();
};

class StateB : public IState
{
public:
	StateB(IContainer * thermostat);
	void MethodOne();
	void MethodTwo();
};