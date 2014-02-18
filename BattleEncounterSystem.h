#ifndef BATTLEENCOUNTERSYSTEM_H_
#define BATTLEENCOUNTERSYSTEM_H_

#include "Global.h"
#include "I_Subject.h"

#include "PausableClock.h"
#include "BattleEncounterComponent.h"
#include "CollisionComponent.h"
#include "PositionComponent.h"

class BattleEncounterSystem : public artemis::EntityProcessingSystem, public I_Subject
{
public:
	BattleEncounterSystem(sf::RenderWindow* window)
	{
		this->window = window;

		addComponentType<BattleEncounterComponent>();
		addComponentType<PositionComponent>();
		pClock.restart();
		battleDelay = sf::seconds(10);
	}

	~BattleEncounterSystem(){}

	virtual void initialize() 
	{
		positionMapper.init(*world);
		collisionMapper.init(*world);
		battleEncounterMapper.init(*world);

		player = &world->getTagManager()->getEntity("Player");
	}

	virtual void processEntity(artemis::Entity &e)
	{
		if(pClock.getElapsedTime() >= battleDelay)
		{
			if(collisionExists(&e))
			{
				cout << "COLLISION" << endl;
				if(battleEncounterMapper.get(e)->getEncounterChance() <= (rand() % 100 + 1))
				{
					notify();
				}
				pClock.restart();
			}
		}
	}

	virtual string getValue(){ return "Battle"; }

	bool collisionExists(artemis::Entity* a)
	{
		collisionBox1 = (*collisionMapper.get(*player)->getCollisionBox());
		collisionBox2 = (*battleEncounterMapper.get(*a)->getCollisionBox());
		return collisionBox1.intersects(collisionBox2);
	}

	void debug(artemis::Entity& e)
	{
		PositionComponent& p = *positionMapper.get(e);

		shape.setPosition(p.getPosX() - battleEncounterMapper.get(e)->getCollisionBox()->width / 2, p.getPosY() - battleEncounterMapper.get(e)->getCollisionBox()->height / 2);
		shape.setSize(sf::Vector2f((float)battleEncounterMapper.get(e)->getCollisionBox()->width, (float)battleEncounterMapper.get(e)->getCollisionBox()->height));
		shape.setOutlineThickness(2.0f);
		shape.setOutlineColor(sf::Color::Blue);
		shape.setFillColor(sf::Color::Transparent);
		window->draw(shape);
	}

private:
	artemis::Entity* player;
	PausableClock pClock;
	sf::Time battleDelay;
	sf::IntRect collisionBox1, collisionBox2;

	sf::RectangleShape shape;
	sf::RenderWindow* window;


	artemis::ComponentMapper<PositionComponent> positionMapper;
	artemis::ComponentMapper<CollisionComponent> collisionMapper;
	artemis::ComponentMapper<BattleEncounterComponent> battleEncounterMapper;
};
#endif