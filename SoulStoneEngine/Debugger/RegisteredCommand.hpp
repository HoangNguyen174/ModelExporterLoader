#ifndef REGISTERED_COMMAND_H
#define REGISTERED_COMMAND_H
#include <vector>
#include <string>

typedef void (*CommandFuncPtr) (const std::string& args);

class RegisteredCommand
{
	public:
		std::string m_commandName;
		CommandFuncPtr m_callbackFunction;
		std::string m_commandDescription;
};

#endif

