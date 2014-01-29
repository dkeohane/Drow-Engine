#include "CollisionSystem.h"


void CollisionSystem::processCollisions(artemis::ImmutableBag<artemis::Entity*>* groupA, artemis::ImmutableBag<artemis::Entity*>* groupB)
{
	if(!groupA->isEmpty() && !groupB->isEmpty())
	{
		for(unsigned int i = 0; i < groupA->getCount(); i++)
		{
			a = groupA->get(i);

			posA = positionMapper.get(*a)->getPosition();
			speedA = (velocityMapper.get(*a)) ? velocityMapper.get(*a)->getVelocity() : sf::Vector2f(0.0f,0.0f); 

			for(unsigned int j = 0; j < groupB->getCount(); j++)
			{
				b = groupB->get(j);

				posB = positionMapper.get(*b)->getPosition();
				speedB = (velocityMapper.get(*b)) ? velocityMapper.get(*b)->getVelocity() : sf::Vector2f(0.0f,0.0f); 

				if(a == b)
				{
					continue;
				}

				if(collisionExists(a, b))
				{
					direction = MathHelper::normalise(MathHelper::getDirection(posA,posB));
					multA = MathHelper::mult(direction, speedA + force);
					multB = MathHelper::mult(direction, speedB);
					cout << "COLLISION " << endl;
					positionMapper.get(*a)->setPosition(posA - multA);
					positionMapper.get(*b)->setPosition(posB + multB);
				}
			}
		}
	}
}
