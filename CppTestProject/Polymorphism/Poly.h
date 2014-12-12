class IState;

class IState
{
public:
	virtual void Method() = 0;
};

class StateA : public IState
{
public:
	void Method();
};

class StateB : public IState
{
public:
	void Method();
};