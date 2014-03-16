#include "GameState.h"
#include "MenuComponentGroupRenderingSystem.h"

void GameState::Load(sf::RenderWindow& window)
{
	entityManager = gameWorld.getEntityManager();
	systemManager = gameWorld.getSystemManager();
	groupManager = gameWorld.getGroupManager();
	tagManager = gameWorld.getTagManager();

	fileLoader = new FileLoader(&gameWorld);
	spriteMapper.init(gameWorld);
	inventoryMapper.init(gameWorld);
	characterRPGMapper.init(gameWorld);
	
	p = new ParticleEmitter(&gameWorld);
	p->setMaxLifeTime(sf::milliseconds(600));
	p->setMinLifeTime(sf::milliseconds(450));

	stateSpriteGroups[PAUSED] = "PauseMenuSprites";
	stateSpriteGroups[INVENTORY] = "InventoryMenuSprites";
	
	stateMenuTextGroups[PAUSED] = "PauseMenuText";
	stateMenuTextGroups[INVENTORY] = "InventoryMenuText";

	fileLoader->loadPlayerDetails("Config/Maps/Player.json");
	fileLoader->loadMap("Config/Maps/Map2.json");
	fileLoader->loadTextComponents("Config/Maps/PauseMenuGroup.json", true, NULL);
	fileLoader->loadTextComponents("Config/Maps/temp.json", true, NULL);

	tagManager->getEntity("Player").addComponent(new CameraComponent(&window, sf::View(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(500, 400))));
	I_State::blackBoard->setEntry("Player RPG Component", characterRPGMapper.get(tagManager->getEntity("Player")));
	tagManager->getEntity("Player").refresh();

	systemManager->setSystem(new ParticleSystem());
	systemManager->setSystem(new MovementSystem());
	systemManager->setSystem(new CameraSystem());
	systemManager->setSystem(new CollisionSystem());
	systemManager->setSystem(new SpriteSystem(&window));
	systemManager->setSystem(new AnimationSystem(&window));
	systemManager->setSystem(new CollisionboxUpdateSystem(&window));
	systemManager->setSystem(new AnimationCollisionboxUpdateSystem(&window));
	systemManager->setSystem(new MenuComponentGroupRenderingSystem(&window));
	systemManager->setSystem(new MenuComponentSystem(stateMenuTextGroups[PAUSED]));
	((MenuComponentSystem*)(systemManager->getSystem<MenuComponentSystem>()))->attach(this);
	systemManager->setSystem(new ViewRelativeUpdateSystem(&window));
	systemManager->setSystem(new BattleEncounterSystem(&window));
	((BattleEncounterSystem*)(systemManager->getSystem<BattleEncounterSystem>()))->attach(this);
	systemManager->setSystem(new InventoryMenuSystem(inventoryMapper.get(tagManager->getEntity("Player"))->getUnequipedItemNames(), stateMenuTextGroups[INVENTORY]));
	((InventoryMenuSystem*)(systemManager->getSystem<InventoryMenuSystem>()))->attach(this);

	systemManager->initializeAll();

	// temp
	artemis::Entity& e = entityManager->create();
	e.addComponent(new PositionComponent(900,-50));
	e.addComponent(new ViewPositionComponent(900,-50));
	e.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource("Media/Images/Backgrounds/menuBG1.png")));
	spriteMapper.get(e)->setActive(false);
	e.refresh();
	groupManager->set(stateSpriteGroups[PAUSED], e);

	artemis::Entity& inventoryBackground = entityManager->create();
	inventoryBackground.addComponent(new PositionComponent(0,0));
	inventoryBackground.addComponent(new ViewPositionComponent(0,0));
	inventoryBackground.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource("Media/Images/Backgrounds/inventoryBackground.png")));
	spriteMapper.get(inventoryBackground)->getSprite()->setScale(2,2);
	inventoryBackground.refresh();
	groupManager->set(stateSpriteGroups[INVENTORY], inventoryBackground);
	// end temp

	setStateSpriteComponents(INVENTORY, false);

	currentState = previousState = PLAYING;

	gameWorld.loopStart();
	gameWorld.setDelta(0.0016f);

}

void GameState::Update(sf::Event& event)
{
	if(currentState == PAUSED)
	{
		((MenuComponentSystem*)systemManager->getSystem<MenuComponentSystem>())->Update(event);
	}
	else if (currentState == INVENTORY)
	{
		((InventoryMenuSystem*)systemManager->getSystem<InventoryMenuSystem>())->Update(event);
	}
	handlePlayerInput(event);
}


void GameState::ProcessState()
{	
	systemManager->getSystem<SpriteSystem>()->process();
	
	// replace with fnc ptrs later on
	if(currentState == PLAYING)
	{
		p->createParticle(10, sf::Vector2f(1.5f, 1.5f), sf::Vector2f(600.0f, 360.0f), "Media/Images/particle3.png", true);
		systemManager->getSystem<CollisionboxUpdateSystem>()->process();
		systemManager->getSystem<AnimationCollisionboxUpdateSystem>()->process();
		systemManager->getSystem<CollisionSystem>()->process();
		systemManager->getSystem<CameraSystem>()->process();
		systemManager->getSystem<MovementSystem>()->process();
		systemManager->getSystem<ParticleSystem>()->process();
		systemManager->getSystem<BattleEncounterSystem>()->process();
		systemManager->getSystem<AnimationSystem>()->process();
	}
	else if(currentState == PAUSED)
	{
		systemManager->getSystem<ViewRelativeUpdateSystem>()->process();
		systemManager->getSystem<MenuComponentSystem>()->process();
		systemManager->getSystem<MenuComponentGroupRenderingSystem>()->process();
		systemManager->getSystem<AnimationSystem>()->process();
	}
	else if (currentState == CHARACTER_DETAILS)
	{
		systemManager->getSystem<ViewRelativeUpdateSystem>()->process();
	}
	else if (currentState == INVENTORY)
	{
		systemManager->getSystem<ViewRelativeUpdateSystem>()->process();
		systemManager->getSystem<InventoryMenuSystem>()->process();
		systemManager->getSystem<MenuComponentGroupRenderingSystem>()->process();
	} 
	else if (currentState == QUIT)
	{
		window->setView(window->getDefaultView());
		notify();
		currentState = PLAYING;
	}
}

void GameState::Update(I_Subject* theChangeSubject)
{
	if(theChangeSubject->getValue() == "Quit")
	{
		subjectValue = "MainMenu";
		setStateSpriteComponents(PAUSED, false);
		currentState = QUIT;
	}
	else if(theChangeSubject->getValue() == "Inventory")
	{
		currentState = INVENTORY;
		previousState = PAUSED;
		setStateSpriteComponents(PAUSED, false);
		((MenuComponentGroupRenderingSystem*)systemManager->getSystem<MenuComponentGroupRenderingSystem>())->setMenuComponentsGroup(stateMenuTextGroups[INVENTORY]);
		setStateSpriteComponents(INVENTORY, true);
	}
	else if(theChangeSubject->getValue() == "Resume")
	{
		currentState = PLAYING;
		previousState = PLAYING;
		setStateSpriteComponents(PAUSED, false);
	}
	else if(theChangeSubject->getValue() == "Battle")
	{
		currentState = PLAYING;
		previousState = PLAYING;
		subjectValue = "BattleState";

		window->setView(sf::View(sf::Vector2f(400, 300), sf::Vector2f(800, 600)));
		notify();
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
				((MenuComponentGroupRenderingSystem*)systemManager->getSystem<MenuComponentGroupRenderingSystem>())->setMenuComponentsGroup(stateMenuTextGroups[PAUSED]);
				previousState = PLAYING;
				setStateSpriteComponents(currentState, true);
			}
		}
		else if(event.joystickButton.button == 1) // 1 is B on 360 controller
		{
			if(previousState != 0)
			{
				setStateSpriteComponents(currentState, false);
				currentState = previousState;
				setStateSpriteComponents(currentState, true);
				((MenuComponentGroupRenderingSystem*)systemManager->getSystem<MenuComponentGroupRenderingSystem>())->setMenuComponentsGroup(stateMenuTextGroups[currentState]);
				previousState = GameplayStates(0);
			}	
		}
	}
}

void GameState::setStateSpriteComponents(GameplayStates state, bool enable)
{
	if(enable)
	{
		for(int i = 0; i < (groupManager->getEntities(stateSpriteGroups[state]))->getCount(); i++)
		{
			spriteMapper.get(*(groupManager->getEntities(stateSpriteGroups[state]))->get(i))->setActive(true);
		}
	}else{
		for(int i = 0; i < (groupManager->getEntities(stateSpriteGroups[state]))->getCount(); i++)
		{
			spriteMapper.get(*(groupManager->getEntities(stateSpriteGroups[state]))->get(i))->setActive(false);
		}
	}
}
