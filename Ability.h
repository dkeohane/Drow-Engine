#ifndef ABILITY_H_
#define ABILITY_H_

#include "Global.h"

struct Ability
{
	Ability()
		:abilityName("default"), damageValue(0), healingValue(0)
	{
	
	}

	Ability(std::string abilityName, int damageValue, int healingValue, string abilityAnimationFilepath)
		: abilityName(abilityName), 
		  damageValue(damageValue),
		  healingValue(healingValue), 
		  abilityAnimationFilepath(abilityAnimationFilepath)
	{

	}

	std::string abilityName;
	int damageValue;
	int healingValue;
	string abilityAnimationFilepath;
};

#endif