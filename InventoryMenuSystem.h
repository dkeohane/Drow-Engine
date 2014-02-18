#ifndef INVENTORYMENUSYSTEM_H_
#define INVENTORYMENUSYSTEM_H_

#include "Global.h"
#include "MenuTextComponent.h"
#include "I_Subject.h"
#include "PositionComponent.h"
#include "InventoryComponent.h"

class InventoryMenuSystem: public artemis::EntityProcessingSystem, public I_Subject
{
public:
	InventoryMenuSystem(std::vector<string> menuItemNames, std::string menuComponentsGroup)
	{
		axisThreshold = 30;
		gamePadID = 0;

		delayTime = sf::milliseconds(200);
		time.restart();
		this->menuComponentsGroup = menuComponentsGroup;
		menuEntity = NULL;
		inventoryEntity = NULL;

		this->menuItemNames = menuItemNames;

		addComponentType<InventoryComponent>();

		maxOnScreen = 4;
		maxEntitiesOnScreen = 4;
		onScreenSelectedIndex = 0;
		totalSelectedIndex = 0;
		startDisplayIndex = 0;
		endDisplayIndex = 0;
	}
	~InventoryMenuSystem(){}

	virtual void initialize()
	{
		groupManager = world->getGroupManager();
		menuTextMapper.init(*world);
		positionMapper.init(*world);
		inventoryMapper.init(*world);
		endDisplayIndex = startDisplayIndex + maxOnScreen;
		currentState = ITEM_SELECTION;
	}

	virtual void processEntity(artemis::Entity &e);

	static const enum SelectionState
	{
		ITEM_SELECTION,
		ITEM_ACTION
	};

	void menuUp();
	void menuDown();
	void Update(sf::Event& event);

	std::string getMenuComponentsGroup() const { return menuComponentsGroup; }
	void setMenuComponentsGroup(std::string val) { menuComponentsGroup = val; menuEntity = NULL; }
	std::vector<string> getMenuItemNames() const { return menuItemNames; }
	void setMenuItemNames(std::vector<string> val) { menuItemNames = val; }
	int getMaxOnScreen() const { return maxOnScreen; }
	void setMaxOnScreen(int val) { maxOnScreen = val; }

	virtual string getValue(){ return subjectValue; }
	void Draw();

private:
	float xVal, yVal, axisThreshold;
	int gamePadID;

	int maxOnScreen, maxEntitiesOnScreen;
	int onScreenSelectedIndex, totalSelectedIndex;
	int startDisplayIndex;
	int endDisplayIndex;

	std::string menuComponentsGroup, subjectValue;
	sf::Time delayTime;
	sf::Clock time;

	artemis::Entity* menuEntity;
	artemis::Entity* inventoryEntity;
	artemis::GroupManager* groupManager;

	std::vector<string> menuItemNames;
	sf::RenderWindow* window;

	string selectedItem;
	SelectionState currentState;
	
	artemis::ComponentMapper<MenuTextComponent> menuTextMapper;
	artemis::ComponentMapper<PositionComponent> positionMapper;
	artemis::ComponentMapper<InventoryComponent> inventoryMapper;
};

#endif