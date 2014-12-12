#include "StatePattern.h"
#include <iostream>

Container container;

int main()
{
	container.setCurrentState(container.getStateA());
	LogicFunction();
}

void LogicFunction()
{
	IState * currentState = container.getCurrentState();
	currentState->MethodOne();
}

/* Container */

Container::Container()
{
	StateA state(this);
	stateA = &state;
	
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
void StateA::MethodOne()
{
};
void StateA::MethodTwo()
{
};

/* State B */

StateB::StateB(IContainer * container)
{
};
void StateB::MethodOne()
{
};
void StateB::MethodTwo()
{
};