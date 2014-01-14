#ifndef MENUCOMPONENTRENDERINGSYSTEM_H_
#define MENUCOMPONENTRENDERINGSYSTEM_H_

#include "Global.h"

#include "MenuTextComponent.h"
#include "PositionComponent.h"

class MenuComponentRenderingSystem : public artemis::EntityProcessingSystem
{
public:
	MenuComponentRenderingSystem(sf::RenderWindow* window)
	{
		this->window = window;
		addComponentType<MenuTextComponent>();
		addComponentType<PositionComponent>();
	}
	~MenuComponentRenderingSystem(){}

	virtual void initialize() 
	{
		menuTextMapper.init(*world);
		positionMapper.init(*world);
	}

	virtual void processEntity(artemis::Entity &e)
	{
		PositionComponent& p = *positionMapper.get(e);
		sf::Text text = menuTextMapper.get(e)->getComponentText();
		text.setPosition(p.getPosition());
		window->draw(text);
	}

private:
	sf::RenderWindow* window;
	artemis::ComponentMapper<MenuTextComponent> menuTextMapper;
	artemis::ComponentMapper<PositionComponent> positionMapper;

};

#endif