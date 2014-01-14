#ifndef VELOCITYCOMPONENT_H_
#define VELOCITYCOMPONENT_H_

#include "SFML\Graphics.hpp"
#include "Artemis\Artemis.h"

class VelocityComponent : public artemis::Component
{
public:
    VelocityComponent(float velocityX, float velocityY)
    {
        this->velocityX = velocityX;
        this->velocityY = velocityY;
    };

	float getVelocityX() const { return velocityX; }
	void setVelocityX(float val) { velocityX = val; }

	float getVelocityY() const { return velocityY; }
	void setVelocityY(float val) { velocityY = val; }

	sf::Vector2f getVelocity(){ return sf::Vector2f(velocityX, velocityY); }
	void setVelocity(sf::Vector2f velocity){ velocityX = velocity.x; velocityY = velocity.y; }

private:
	float velocityX;
	float velocityY;

};

#endif