#ifndef ACTIVATEABLECOMPONENT_H_
#define ACTIVATEABLECOMPONENT_H_

//#include "Artemis\Component.h"

class ActivatableComponent: public artemis::Component
{
public:
	/*
	* To be used for Components whose activity may need
	* to be changed during its lifetime
	*
	* Example: For the menuComponentSystem, it only handles
	* those entites who have a menuTextComponent which is
	* active at that time.
	*/
	ActivatableComponent(){active = true;}
	bool isActive() const { return active; }
	void setActive(bool val) { active = val; }
private:
	bool active;
};

#endif