#include "HealthPotion.h"

HealthPotion::HealthPotion(double healthIncrease, string textureFilePath)
{
	this->healthIncrease = healthIncrease;
	this->setItemDescription("This is a Health Potion :O");
	setTextureFilePath(textureFilePath);
}

HealthPotion::~HealthPotion()
{

}

void HealthPotion::Use(CharacterRPGAttributes* c)
{
	c->increaseCurrentHealth(healthIncrease);
}

