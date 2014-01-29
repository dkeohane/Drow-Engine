#include "MainMenuState.h"

MainMenuState::MainMenuState(sf::RenderWindow& window)
{
	this->Load(window);
}

MainMenuState::~MainMenuState()
{

}

void MainMenuState::Load(sf::RenderWindow& window)
{
	entityManager = menuWorld.getEntityManager();
	systemManager = menuWorld.getSystemManager();
	groupManager = menuWorld.getGroupManager();

	mapLoader = new MapLoader(&menuWorld);
	mapLoader->loadTextComponents("Config/Maps/MainMenuTextComponents.json", false, NULL);

	systemManager->setSystem(new SpriteSystem(&window));
	systemManager->setSystem(new MenuComponentSystem("MainMenuComponents"));
	systemManager->setSystem(new MenuComponentRenderingSystem(&window));
	systemManager->initializeAll();

	((MenuComponentSystem*)(systemManager->getSystem<MenuComponentSystem>()))->attach(this);

	artemis::Entity& background = entityManager->create();
	background.addComponent(new PositionComponent(0,0));
	background.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource("Media/Images/MainMenuBackground.jpg")));
	background.refresh();

	menuWorld.loopStart();
	menuWorld.setDelta(0.0016f);
}

void MainMenuState::Draw(sf::RenderWindow& window)
{
	systemManager->getSystem<SpriteSystem>()->process();
	systemManager->getSystem<MenuComponentRenderingSystem>()->process();
}

void MainMenuState::Update(sf::Event& event)
{
	systemManager->getSystem<MenuComponentSystem>()->process();
	((MenuComponentSystem*)(systemManager->getSystem<MenuComponentSystem>()))->Update(event);
}

void MainMenuState::Update(I_Subject* theChangeSubject)
{
	if(theChangeSubject->getValue() != "Back") // Already in the main menu, can't go back
	{
		if(theChangeSubject->getValue() == "Play")
		{
			subjectValue = "GameState";
		}
		else if(theChangeSubject->getValue() == "Credits")
		{
			subjectValue = "CreditsMenu";
		}
		else if(theChangeSubject->getValue() == "Quit")
		{
			subjectValue = "Quit";
		}
		this->notify();
	}
}