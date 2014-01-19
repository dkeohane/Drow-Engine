#ifndef COLLISIONCOMPONENT_H_
#define COLLISIONCOMPONENT_H_

#include "Global.h"

class CollisionComponent: public artemis::Component
{
public:
	CollisionComponent()
	{
		collisionBox = sf::IntRect(0,0,0,0);
	}
	~CollisionComponent(){}

	sf::IntRect* getCollisionBox(){ return &collisionBox; }
	void setCollisionBox(sf::IntRect& val) { collisionBox = val; }

private:
	sf::IntRect collisionBox;

};

#endif