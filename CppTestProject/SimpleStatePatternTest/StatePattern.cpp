#include "StatePattern.h"
#include <iostream>

int main()
{
	LogicFunction();
}

Container container;

void LogicFunction()
{
	//Container container;
	IState * stateToUse = container.getStateA();
	stateToUse->Method();
	system("pause");
}

/* Container */

Container::Container()
{
	/*StateA state(this);
	stateA = &state;*/

	stateA = new StateA(this);
		
	setCurrentState(stateA);
};

void Container::setCurrentState(IState * state)
{
	currentState = state;
};

IState * Container::getCurrentState()
{
	return currentState;
};

IState * Container::getStateA()
{
	return stateA;
};

IState * Container::getStateB()
{
	return stateB;
};

/* State A */

StateA::StateA(IContainer * container)
{
};
void StateA::Method()
{
	std::cout << "State A Method";
};

/* State B */

StateB::StateB(IContainer * container)
{
};
void StateB::Method()
{
	std::cout << "State B Method";
};