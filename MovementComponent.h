#ifndef MOVEMENTCOMPONENT_H_
#define MOVEMENTCOMPONENT_H_

#include "Artemis/Artemis.h"
#include "I_Subject.h"

class MovementComponent: public artemis::Component, public I_Subject
{
public:
	MovementComponent(std::vector<string> filepaths)
	{
		for(unsigned int i = 0; i < filepaths.size(); i++)
		{
			movementStates[(MovementState)i] = filepaths.at(i);
		}
		movementStates[IDLE] = "IDLE";
		currentMovementState = IDLE;
	}
	~MovementComponent(){}


	static const enum MovementState
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		IDLE
	};

	MovementComponent::MovementState getCurrentMovementState(){ return currentMovementState; }
	void setCurrentMovementState(MovementState val) { currentMovementState = val; notify();}

	std::string getCurrentMovementStateString(){ return movementStates[getCurrentMovementState()]; }
	virtual string getValue(){ return getCurrentMovementStateString(); }

private:
	MovementState currentMovementState;
	std::map<MovementState, std::string> movementStates;
};
#endif