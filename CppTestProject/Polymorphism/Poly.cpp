#include "Poly.h"
#include <iostream>

int main()
{
	IState * someState;

	std::cout << "State A's implementation:\n";
	StateA stateA;
	stateA.Method();
	std::cout << "\n\n";

	std::cout << "'Interface' of State A's implementation:\n";
	someState = &stateA;
	someState->Method();
	std::cout << "\n\n";

	std::cout << "State B's implementation:\n";
	StateB stateB;
	stateB.Method();
	std::cout << "\n\n";

	std::cout << "'Interface' of State B's implementation:\n";
	someState = &stateB;
	someState->Method();
	std::cout << "\n\n";

	std::cout << "\n";
	system("pause");
}

void StateA::Method()
{
	std::cout << "state a";
};

void StateB::Method()
{
	std::cout << "state b";
};