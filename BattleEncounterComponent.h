#ifndef BATTLEENCOUNTERCOMPONENT_H_
#define BATTLEENCOUNTERCOMPONENT_H_

#include "Global.h"

class BattleEncounterComponent: public artemis::Component
{
public:
	BattleEncounterComponent(sf::IntRect& collisionBox, int encounterChance, int minLevel, int maxLevel)
		: collisionBox(collisionBox), encounterChance(encounterChance), minLevel(minLevel), maxLevel(maxLevel)
	{
	}

	~BattleEncounterComponent(){}

	sf::IntRect* getCollisionBox(){ return &collisionBox; }
	int getEncounterChance(){ return encounterChance; }
	int getMinLevel(){ return minLevel; }
	int getMaxLevel(){ return maxLevel; }
	
	void setCollisionBox(sf::IntRect& val) { collisionBox = val; }

private:
	sf::IntRect collisionBox;
	int encounterChance, minLevel, maxLevel;

};
#endif