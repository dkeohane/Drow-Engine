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
	menuGrouping = "MainMenuComponents";

	systemManager->setSystem(new SpriteSystem(&window));
	systemManager->setSystem(new MenuComponentSystem(menuGrouping));
	systemManager->setSystem(new MenuComponentRenderingSystem(&window));
	systemManager->initializeAll();

	((MenuComponentSystem*)(systemManager->getSystem<MenuComponentSystem>()))->attach(this);

	sf::Font font;
	font.loadFromFile("Media/Fonts/font1.ttf");

	artemis::Entity& background = entityManager->create();
	background.addComponent(new PositionComponent(0,0));
	background.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource("Media/Images/MainMenuBackground.jpg")));
	background.refresh();

	artemis::Entity& playGame = entityManager->create();
	playGame.addComponent(new PositionComponent(70,70));
	playGame.addComponent(new MenuTextComponent("Play", font, sf::Color::Green, sf::Color::White, 40));
	playGame.addComponent(new PlayerInputComponent(0, 30));
	playGame.refresh();

	artemis::Entity& showCredits = entityManager->create();
	showCredits.addComponent(new PositionComponent(270,70));
	showCredits.addComponent(new MenuTextComponent("Credits", font, sf::Color::Green, sf::Color::White, 40));
	showCredits.addComponent(new PlayerInputComponent(0, 30));
	showCredits.refresh();

	artemis::Entity& quitGame = entityManager->create();
	quitGame.addComponent(new PositionComponent(430, 70));
	quitGame.addComponent(new MenuTextComponent("Quit", font, sf::Color::Green, sf::Color::White, 40));
	quitGame.addComponent(new PlayerInputComponent(0, 30));
	quitGame.refresh();

	groupManager->set(menuGrouping, playGame);
	groupManager->set(menuGrouping, showCredits);
	groupManager->set(menuGrouping, quitGame);

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