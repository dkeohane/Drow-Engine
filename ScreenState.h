#ifndef SCREENSTATE_H_
#define SCREENSTATE_H_

#include "I_State.h"
#include "I_Subject.h"

class ScreenState : public I_State, public I_Subject
{
public:
	virtual void Load(sf::RenderWindow& window) = 0;
	virtual void ProcessState() = 0;
	virtual void Update(sf::Event& event) = 0;

	virtual void onStateEntry(){};
	virtual void onStateDeparture(){};

	virtual string getValue() = 0;
};

#endif