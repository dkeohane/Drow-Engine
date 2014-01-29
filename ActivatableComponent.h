#ifndef ACTIVATEABLECOMPONENT_H_
#define ACTIVATEABLECOMPONENT_H_

class ActivatableComponent: public artemis::Component
{
public:
	/*
	* To be used for Components whose activity may need
	* to be changed during its lifetime
	*/
	ActivatableComponent(){active = true;}
	bool isActive() const { return active; }
	void setActive(bool val) { active = val; }
private:
	bool active;
};

#endif