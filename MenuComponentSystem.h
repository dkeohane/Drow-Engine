#ifndef MENUCOMPONENTSYSTEM_H_
#define MENUCOMPONENTSYSTEM_H_

#include "Global.h"

#include "MenuTextComponent.h"
#include "PlayerInputComponent.h"
#include "PositionComponent.h"

#include "I_Subject.h"

class MenuComponentSystem: public artemis::EntityProcessingSystem, public I_Subject
{
public:
	MenuComponentSystem(std::string menuComponentsGroup)
	{
		currentlySelected = 0;
		axisThreshold = -1;
		gamePadID = -1;
		delayTime = sf::milliseconds(200);
		time.restart();
		this->menuComponentsGroup = menuComponentsGroup;
		menuEntity = NULL;
		addComponentType<MenuTextComponent>();
		addComponentType<PlayerInputComponent>();
	}

	~MenuComponentSystem(){ }

	virtual void initialize() 
	{
		groupManager = world->getGroupManager();
		menuTextMapper.init(*world);
		playerInputMapper.init(*world);
	}

	void Update(sf::Event& event);

	std::string getMenuComponentsGroup() const { return menuComponentsGroup; }
	void setMenuComponentsGroup(std::string val) { menuComponentsGroup = val; menuEntity = NULL; }

	virtual string getValue()
	{ 
		return subjectValue; 
	}
	virtual void processEntity(artemis::Entity &e);

	void menuUpOrLeft();
	void menuDownOrRight();
	
private:
	float xVal, yVal, axisThreshold;
	int currentlySelected, gamePadID;
	std::string menuComponentsGroup, subjectValue;
	sf::Time delayTime;
	sf::Clock time;

	artemis::Entity* menuEntity;
	artemis::GroupManager* groupManager;

	artemis::ComponentMapper<MenuTextComponent> menuTextMapper;
	artemis::ComponentMapper<PlayerInputComponent> playerInputMapper;
	
};
#endif