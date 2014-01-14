#ifndef CHARACTERRPGATTRIBUTES_H_
#define CHARACTERRPGATTRIBUTES_H_


#include "Artemis\Artemis.h"

#include "Ability.h"

class CharacterRPGAttributes : public artemis::Component
{
public:
	/*CharacterRPGAttributes(std::vector<Ability> abilities)
	{
		this->abilities = abilities;
	}*/

	CharacterRPGAttributes(std::map<string, Ability> abilities)
	{
		this->abilities = abilities;
		this->currentHealth = 100;
		this->maxHealth = 100;

		this->strength = 10;
		this->agility = 10;
		this->intelligence = 10;
	}
	~CharacterRPGAttributes(){}

	double getCurrentHealth(){ return currentHealth; }
	double getMaxHealth(){ return maxHealth; }
	double getStrength(){ return strength; }
	double getAgility(){ return agility; }
	double getIntelligence(){ return intelligence; }

	void setCurrentHealth(double val) { currentHealth = val; }
	void setMaxHealth(double val) { maxHealth = val; }
	void setStrength(double strength){ this->strength = strength; }
	void setAgility(double agility){ this->agility = agility; }
	void setIntelligence(double intelligence){ this->intelligence = intelligence; }	

	std::map<string, Ability>* getAbilities(){ return &abilities; }
	void setAbilities(std::map<string, Ability> val) { abilities = val; }

	Ability* getAbility(string abilityName)
	{
		Ability* a = NULL;
		abilityIterator = abilities.find(abilityName);

		if(abilityIterator == abilities.end())
		{
			cout << "Cannot find that ability" << endl;
		}
		else
		{
			a = &abilityIterator->second;
		}

		return a;
	}

	std::vector<string> getAbilityNames()
	{
		std::vector<string> abilityNames;
		for(abilityIterator = abilities.begin(); abilityIterator != abilities.end(); abilityIterator++)
		{
			abilityNames.push_back(abilityIterator->first);
		}
		return abilityNames;
	}

private:
	double currentHealth;
	double maxHealth;

	double strength;
	double agility;
	double intelligence;

	std::map<string, Ability>::iterator abilityIterator;
	std::map<string, Ability> abilities;
};

#endif