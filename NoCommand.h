#ifndef NOCOMMAND_H_
#define NOCOMMAND_H_

#include "Command.h"

class NoCommand : public Command
{
public:
	// Used when no command it to be executed
	virtual void execute(){};
};
#endif