#ifndef COLLISIONSYSTEM_H_
#define COLLISIONSYSTEM_H_

#include "Global.h"
#include "CollisionComponent.h"
#include "PositionComponent.h"
#include "VelocityComponent.h"

class CollisionSystem : public artemis::EntityProcessingSystem
{
public:
	CollisionSystem()
	{
		addComponentType<CollisionComponent>();
		addComponentType<PositionComponent>();
	}
	~CollisionSystem(){}

	virtual void initialize() 
	{
		positionMapper.init(*world);
		collisionMapper.init(*world);
		velocityMapper.init(*world);
		groupManager = world->getGroupManager();

		force = sf::Vector2f(10.0f, 10.0f);
	}

	virtual void processEntities(artemis::ImmutableBag<artemis::Entity*>& bag)
	{
		artemis::ImmutableBag<artemis::Entity*>* players = groupManager->getEntities("Player");
		artemis::ImmutableBag<artemis::Entity*>* walls = groupManager->getEntities("Wall");
		artemis::ImmutableBag<artemis::Entity*>* architecture = groupManager->getEntities("Architecture");

		processCollisions(players, walls);
		processCollisions(players, architecture);
	}

	virtual void processEntity(artemis::Entity &e)
	{
	
	}

	bool collisionExists(artemis::Entity* a, artemis::Entity* b)
	{
		sf::IntRect a1 = (*collisionMapper.get(*a)->getCollisionBox());
		sf::IntRect a2 = (*collisionMapper.get(*b)->getCollisionBox());
		return a1.intersects(a2);
	}

	void processCollisions(artemis::ImmutableBag<artemis::Entity*>* groupA, artemis::ImmutableBag<artemis::Entity*>* groupB);

private:
	artemis::Entity* a;
	artemis::Entity* b;

	sf::Vector2f posA;
	sf::Vector2f posB;
	sf::Vector2f speedA;
	sf::Vector2f speedB;
	sf::Vector2f direction;
	sf::Vector2f multA;
	sf::Vector2f multB;
	sf::Vector2f force;

	artemis::GroupManager* groupManager;
	artemis::ComponentMapper<PositionComponent> positionMapper;
	artemis::ComponentMapper<VelocityComponent> velocityMapper;
	artemis::ComponentMapper<CollisionComponent> collisionMapper;

};
#endif