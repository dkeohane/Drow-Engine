#ifndef BATTLEHUDSYSTEM_H_
#define BATTLEHUDSYSTEM_H_

#include "Global.h"
#include "HUDComponent.h"
#include "CharacterRPGAttributes.h"

class BattleHUDSystem: public artemis::EntityProcessingSystem
{
public:
	BattleHUDSystem(sf::RenderWindow* window)
	{
		this->window = window;
		addComponentType<HUDComponent>();
		addComponentType<CharacterRPGAttributes>();
	}
	~BattleHUDSystem(){}

	virtual void initialize()
	{
		HUDMapper.init(*world);
		RPGattributesMapper.init(*world);
	}

	virtual void processEntity(artemis::Entity &e)
	{
		bars = (*HUDMapper.get(e)).getHudBars();
		CharacterRPGAttributes& cRPG = *RPGattributesMapper.get(e);

		for(unsigned int i = 0; i < bars->size(); i++)
		{
			sf::IntRect tRext(0, 0, int((cRPG.getCurrentHealth() / cRPG.getMaxHealth()) * bars->at(i).bar.getSize().x), (int)bars->at(i).bar.getSize().y);
			
			bars->at(i).bar.setTextureRect(tRext);
			bars->at(i).bar.setScale((float)cRPG.getCurrentHealth() / (float)cRPG.getMaxHealth(), 1.0f);

			window->draw(bars->at(i).barBackground);
			window->draw(bars->at(i).bar);
		}
	}

private:
	sf::RenderWindow* window;

	std::vector<GuageBar>* bars;
	artemis::ComponentMapper<HUDComponent> HUDMapper;
	artemis::ComponentMapper<CharacterRPGAttributes> RPGattributesMapper;
};
#endif