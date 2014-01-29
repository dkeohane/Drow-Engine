#ifndef HEALTHPOTION_H_
#define HEALTHPOTION_H_

#include "UsableItem.h"

class HealthPotion: public UsableItem
{
public:
	HealthPotion();
	HealthPotion(double healthIncrease, string textureFilePath);
	~HealthPotion();

	virtual void Use(CharacterRPGAttributes* c);
	
	double getHealthIncrease() const { return healthIncrease; }
	void setHealthIncrease(double val) { healthIncrease = val; }

private:
	double healthIncrease;

};

#endif