#ifndef I_STATE_H_
#define I_STATE_H_

#include "Global.h"
#include "I_Subject.h"

class I_State
{
public:
	virtual void ProcessState() = 0;
	virtual void Update(sf::Event& event) = 0;
};
#endif