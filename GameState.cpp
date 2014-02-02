#include "GameState.h"

void GameState::Load(sf::RenderWindow& window)
{
	entityManager = gameWorld.getEntityManager();
	systemManager = gameWorld.getSystemManager();
	groupManager = gameWorld.getGroupManager();
	tagManager = gameWorld.getTagManager();

	systemManager->setSystem(new MovementSystem());
	systemManager->setSystem(new CameraSystem());
	systemManager->setSystem(new CollisionSystem());
	systemManager->setSystem(new SpriteSystem(&window));
	systemManager->setSystem(new AnimationSystem(&window));
	systemManager->setSystem(new CollisionboxUpdateSystem(&window));
	systemManager->setSystem(new AnimationCollisionboxUpdateSystem(&window));
	systemManager->setSystem(new MenuComponentRenderingSystem(&window));
	systemManager->setSystem(new MenuComponentSystem("PauseMenuGroup"));
	((MenuComponentSystem*)(systemManager->getSystem<MenuComponentSystem>()))->attach(this);
	systemManager->setSystem(new ViewRelativeUpdateSystem(&window));
	systemManager->initializeAll();

	mapLoader = new MapLoader(&gameWorld);

	mapLoader->loadMap("Config/Maps/Map1.json");
	mapLoader->loadPlayerDetails("Config/Maps/Player.json");
	mapLoader->loadTextComponents("Config/Maps/PauseMenuGroup.json", true, NULL);

	tagManager->getEntity("Player").addComponent(new CameraComponent(&window, sf::View(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(1280, 800))));
	tagManager->getEntity("Player").addComponent(new InventoryComponent());

	// temp
	std::map<CharacterAttributes::Attribute, double> v;
	v[CharacterAttributes::MAXHEALTH] = 20;
	v[CharacterAttributes::STRENGTH] = 20;
	EquipableItem* item = new EquipableItem(EquipableItemSlots::HEAD, v);

	item->attach((CharacterRPGComponent*)tagManager->getEntity("Player").getComponent<CharacterRPGComponent>());
	((InventoryComponent*)tagManager->getEntity("Player").getComponent<InventoryComponent>())->equipItem(item);
	((InventoryComponent*)tagManager->getEntity("Player").getComponent<InventoryComponent>())->equipItem(item);
	tagManager->getEntity("Player").refresh();
	// end temp

	

	// temp
	artemis::Entity& e = entityManager->create();
	e.addComponent(new PositionComponent(600,0));
	e.addComponent(new ViewPositionComponent(600,0));
	e.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource("Media/Images/background1.png")));
	((SpriteComponent*)e.getComponent<SpriteComponent>())->setActive(false);
	e.refresh();
	groupManager->set("pauseMenuBackground", e);
	// end temp

	// temp
	artemis::Entity& e2 = entityManager->create();
	e2.addComponent(new PositionComponent(100,100));
	e2.addComponent(new ViewPositionComponent(100,100));
	e2.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource("Media/Images/blehk.png")));
	((SpriteComponent*)e2.getComponent<SpriteComponent>())->setActive(false);
	e2.refresh();
	groupManager->set("InventoryMenuBackground", e2);
	// end temp

	stateEntities[PAUSED] = "pauseMenuBackground";
	stateEntities[INVENTORY] = "InventoryMenuBackground";


	currentState = previousState = PLAYING;

	gameWorld.loopStart();
	gameWorld.setDelta(0.0016f);
}

void GameState::Update(sf::Event& event)
{
	handlePlayerInput(event);
	if(currentState == PAUSED)
	{
		((MenuComponentSystem*)systemManager->getSystem<MenuComponentSystem>())->Update(event);
	}
}


void GameState::Draw(sf::RenderWindow& window)
{	
	systemManager->getSystem<SpriteSystem>()->process();
	systemManager->getSystem<AnimationSystem>()->process();
	

	// replace with fnc ptrs later on
	if(currentState == PLAYING)
	{
		systemManager->getSystem<CollisionboxUpdateSystem>()->process();
		systemManager->getSystem<AnimationCollisionboxUpdateSystem>()->process();
		systemManager->getSystem<CollisionSystem>()->process();
		systemManager->getSystem<CameraSystem>()->process();
		systemManager->getSystem<MovementSystem>()->process();
	}
	else if(currentState == PAUSED)
	{
		systemManager->getSystem<ViewRelativeUpdateSystem>()->process();
		systemManager->getSystem<MenuComponentSystem>()->process();
		systemManager->getSystem<MenuComponentRenderingSystem>()->process();
	}
	else if (currentState == CHARACTER_DETAILS)
	{
		systemManager->getSystem<ViewRelativeUpdateSystem>()->process();
	}
	else if (currentState == INVENTORY)
	{
		
	} 
	else if (currentState == QUIT)
	{
		window.setView(window.getDefaultView());
		notify();
		currentState = PLAYING;
	}
}

void GameState::Update(I_Subject* theChangeSubject)
{
	if(theChangeSubject->getValue() == "Quit")
	{
		subjectValue = "MainMenu";
		setStateComponents(PAUSED, false);
		currentState = QUIT;
	}
	else if(theChangeSubject->getValue() == "Inventory")
	{
		currentState = INVENTORY;
		previousState = PAUSED;
		setStateComponents(PAUSED, false);
		setStateComponents(INVENTORY, true);
	}
	else if(theChangeSubject->getValue() == "Resume")
	{
		currentState = PLAYING;
		previousState = PLAYING;
		setStateComponents(PAUSED, false);
	}
}

void GameState::handlePlayerInput(sf::Event& event)
{
	if(event.type == sf::Event::JoystickButtonReleased)
	{
		if(event.joystickButton.button == 7) // 7 is start on 360 controller
		{
			if(currentState == PLAYING)
			{
				currentState = PAUSED; 
				previousState = PLAYING;
				setStateComponents(currentState, true);
			}
		}
		else if(event.joystickButton.button == 1) // 1 is B on 360 controller
		{
			if(previousState != 0)
			{
				setStateComponents(currentState, false);
				currentState = previousState;
				setStateComponents(currentState, true);
				previousState = GameplayStates(0);
			}
		}
	}
}


void GameState::setStateComponents(GameplayStates state, bool enable)
{
	if(enable)
	{
		for(unsigned int i = 0; i < (groupManager->getEntities(stateEntities[state]))->getCount(); i++)
		{
			((SpriteComponent*)(groupManager->getEntities(stateEntities[state])->get(i))->getComponent<SpriteComponent>())->setActive(true);
		}
	}else{
		for(unsigned int i = 0; i < (groupManager->getEntities(stateEntities[state]))->getCount(); i++)
		{
			((SpriteComponent*)(groupManager->getEntities(stateEntities[state])->get(i))->getComponent<SpriteComponent>())->setActive(false);
		}
	}
}


