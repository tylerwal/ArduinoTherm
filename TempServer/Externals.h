struct ParsedRequest
{
	char* httpMethod;
	char* command;
	char* parameter;
};

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

    unsigned long TimeInCurrentState()
	{
		return millis() - this->StartTimeCurrentState;
	}
};

class IState
{
	public:
		virtual void GoalTemperatureReached() = 0;
};
