#ifndef ABILITY_H_
#define ABILITY_H_

#include "Global.h"

struct Ability
{
	Ability()
		:abilityName("default"), damageValue(0), healingValue(0)
	{
	
	}

	Ability(std::string abilityName, int damageValue, int healingValue)
		: abilityName(abilityName), damageValue(damageValue), healingValue(healingValue)
	{

	}

	std::string abilityName;
	int damageValue;
	int healingValue;
};

#endif