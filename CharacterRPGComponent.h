#ifndef CHARACTERRPGCOMPONENT_H_
#define CHARACTERRPGCOMPONENT_H_


#include "Artemis\Artemis.h"
#include "CharacterAttributes.h"
#include "Ability.h"
#include "CharacterAttributesObserver.h"

class CharacterRPGComponent : public artemis::Component, public CharacterAttributesObserver
{
public:
	CharacterRPGComponent()
	{
		AttributeValues[CharacterAttributes::MAXHEALTH] = 100.0;
		AttributeValues[CharacterAttributes::CURRENTHEALTH] = 100.0;

		AttributeValues[CharacterAttributes::AGILITY] = 10.0;
		AttributeValues[CharacterAttributes::STRENGTH] = 10.0;
		AttributeValues[CharacterAttributes::INTELLIGENCE] = 10.0;
	}

	CharacterRPGComponent(std::map<string, Ability> abilities)
	{
		this->abilities = abilities;
		AttributeValues[CharacterAttributes::MAXHEALTH] = 100.0;
		AttributeValues[CharacterAttributes::CURRENTHEALTH] = 100.0;

		AttributeValues[CharacterAttributes::AGILITY] = 10.0;
		AttributeValues[CharacterAttributes::STRENGTH] = 10.0;
		AttributeValues[CharacterAttributes::INTELLIGENCE] = 10.0;
	}
	~CharacterRPGComponent(){}

	double getAttributeValue(CharacterAttributes::Attribute attr)
	{
		return AttributeValues[attr];
	}

	void setAttributeValue(CharacterAttributes::Attribute attr, double value)
	{
		if(value >= 0)
		{
			if(attr == CharacterAttributes::CURRENTHEALTH && value > AttributeValues[CharacterAttributes::MAXHEALTH])
			{
				AttributeValues[attr] = value;
				AttributeValues[CharacterAttributes::MAXHEALTH] = value; 
			}else{
				AttributeValues[attr] = value;
			}
		}
	}

	std::map<string, Ability>* getAbilities(){ return &abilities; }
	void setAbilities(std::map<string, Ability> val) { abilities = val; }

	Ability* getAbility(string& abilityName)
	{
		Ability* a = NULL;
		abilityIterator = abilities.find(abilityName);

		if(abilityIterator == abilities.end())
		{
			cout << "Cannot find that ability : " << abilityName << endl;
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

	void handleAttributeValueChange(CharacterAttributes::Attribute attr, double value)
	{
		if(attr == CharacterAttributes::CURRENTHEALTH && AttributeValues[attr] + value > AttributeValues[CharacterAttributes::MAXHEALTH])
		{
			AttributeValues[attr] = AttributeValues[CharacterAttributes::MAXHEALTH];
		}
		else
		{
			if (AttributeValues[attr] + value > 0)
			{
				AttributeValues[attr] += value;
			}else{
				AttributeValues[attr] = 0;
			}
		}
	}


	virtual void Update(CharacterAttributesSubject* theChangeSubject)
	{
		std::map<CharacterAttributes::Attribute, double> subjectValues = theChangeSubject->getValue();
		for(attributeIterator = subjectValues.begin(); attributeIterator != subjectValues.end(); attributeIterator++)
		{
			handleAttributeValueChange(attributeIterator->first, attributeIterator->second);
		}
	}

private:
	std::map<CharacterAttributes::Attribute, double>::iterator attributeIterator;
	std::map<CharacterAttributes::Attribute, double> AttributeValues;

	std::map<string, Ability>::iterator abilityIterator;
	std::map<string, Ability> abilities;
};

#endif