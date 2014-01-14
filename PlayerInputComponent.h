#ifndef PLAYERINPUTCOMPONENT_H_ 
#define PLAYERINPUTCOMPONENT_H_

class PlayerInputComponent: public artemis::Component
{
public:
	PlayerInputComponent(int gamepadID, float axisThreshold)
	{
		this->gamepadID = gamepadID;
		this->axisThreshold = axisThreshold;
	}
	~PlayerInputComponent(){}

	int getGamepadID() const { return gamepadID; }
	void setGamepadID(int val) { gamepadID = val; }

	float getAxisThreshold() const { return axisThreshold; }
	void setAxisThreshold(float val) { axisThreshold = val; }

private:
	int gamepadID;
	float axisThreshold;

};

#endif