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
	tagManager->getEntity("Player").refresh();

	artemis::Entity& e = entityManager->create();
	e.addComponent(new PositionComponent(600,0));
	e.addComponent(new ViewPositionComponent(600,0));
	e.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource("Media/Images/background1.png")));
	((SpriteComponent*)e.getComponent<SpriteComponent>())->setActive(false);
	e.refresh();
	groupManager->set("pauseMenuBackground", e);

	currentState = PLAYING;

	gameWorld.loopStart();
	gameWorld.setDelta(0.0016f);
}

void GameState::Update(sf::Event& event)
{
	if(event.type == sf::Event::JoystickButtonReleased)
	{
		if(event.joystickButton.button == 7)
		{
			if(currentState == PLAYING)
			{
				currentState = PAUSED;
				for(unsigned int i = 0; i < (groupManager->getEntities("pauseMenuBackground"))->getCount(); i++)
				{
					((SpriteComponent*)(groupManager->getEntities("pauseMenuBackground")->get(i))->getComponent<SpriteComponent>())->setActive(true);
				}
				//((SpriteComponent*)-.getComponent<SpriteComponent>())->setActive(true);
			}else{
				currentState = PLAYING;
				for(unsigned int i = 0; i < (groupManager->getEntities("pauseMenuBackground"))->getCount(); i++)
				{
					((SpriteComponent*)(groupManager->getEntities("pauseMenuBackground")->get(i))->getComponent<SpriteComponent>())->setActive(false);
				}
			}
		}
	}

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

	}
	else if (currentState == INVENTORY)
	{
		
	} 
	else if (currentState == QUIT)
	{
		notify();
	}
}

void GameState::Update(I_Subject* theChangeSubject)
{
	if(theChangeSubject->getValue() == "Quit")
	{
		subjectValue = "MainMenu";
		currentState = QUIT;
	}
	else if(theChangeSubject->getValue() == "Inventory")
	{
		currentState = INVENTORY;
	}
}