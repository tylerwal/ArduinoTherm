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

struct SystemState
{
	unsigned long StartTimeCurrentState;
	HvacState HvacCurrentState;;
};