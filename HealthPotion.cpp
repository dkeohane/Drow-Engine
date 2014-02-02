#include "HealthPotion.h"

HealthPotion::HealthPotion(double healthIncrease, string textureFilePath)
{
	itemValues[CharacterAttributes::CURRENTHEALTH]  = healthIncrease;
	this->healthIncrease = healthIncrease;
	this->setItemDescription("This is a Health Potion :O");
	setTextureFilePath(textureFilePath);
}

HealthPotion::~HealthPotion()
{

}

void HealthPotion::Use()
{
	notify();
}

