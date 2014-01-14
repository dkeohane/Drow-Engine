#ifndef MENUCOMPONENTGROUPRENDERINGSYSTEM_H_
#define MENUCOMPONENTGROUPRENDERINGSYSTEM_H_

#include "Global.h"

#include "MenuTextComponent.h"
#include "PositionComponent.h"

class MenuComponentGroupRenderingSystem : public artemis::EntityProcessingSystem
{
public:
	MenuComponentGroupRenderingSystem(sf::RenderWindow* window)
	{
		this->window = window;
		addComponentType<MenuTextComponent>();
		addComponentType<PositionComponent>();
	}
	~MenuComponentGroupRenderingSystem(){}

	virtual void initialize() 
	{
		groupManager = world->getGroupManager();
		menuTextMapper.init(*world);
		positionMapper.init(*world);
	}

	virtual void processEntity(artemis::Entity &e)
	{
		if(groupManager->isInGroup(menuComponentsGroup, e))
		{
			PositionComponent& p = *positionMapper.get(e);
			sf::Text text = menuTextMapper.get(e)->getComponentText();
			text.setPosition(p.getPosition());
			window->draw(text);
		}
	}

	std::string getMenuComponentsGroup() const { return menuComponentsGroup; }
	void setMenuComponentsGroup(std::string val) { menuComponentsGroup = val; }

private:
	std::string menuComponentsGroup;
	artemis::GroupManager* groupManager;

	sf::RenderWindow* window;
	artemis::ComponentMapper<MenuTextComponent> menuTextMapper;
	artemis::ComponentMapper<PositionComponent> positionMapper;
};

#endif