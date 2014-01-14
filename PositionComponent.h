#ifndef POSITIONCOMPONENT_H_
#define POSITIONCOMPONENT_H_

#include "Global.h"

class PositionComponent : public artemis::Component
{
public:
	PositionComponent(float posX, float posY)
	{
		this->posX = posX;
		this->posY = posY;
	};

	float getPosX() const { return posX; }
	void setPosX(float val) { posX = val; }

	float getPosY() const { return posY; }
	void setPosY(float val) { posY = val; }

	sf::Vector2f getPosition(){ return sf::Vector2f(posX, posY); }
	void setPosition(sf::Vector2f position){ this->posX = position.x; this->posY = position.y; }

private:
	float posX;
	float posY;

};

#endif