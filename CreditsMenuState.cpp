#include "CreditsMenuState.h"



void CreditsMenuState::Load(sf::RenderWindow& window)
{
	entityManager = creditsWorld.getEntityManager();
	systemManager = creditsWorld.getSystemManager();
	groupManager = creditsWorld.getGroupManager();
	menuGrouping = "CreditsMenuComponents";

	systemManager->setSystem(new MenuComponentSystem(menuGrouping));
	((MenuComponentSystem*)(systemManager->getSystem<MenuComponentSystem>()))->attach(this);
	systemManager->setSystem(new MenuComponentRenderingSystem(&window));
	systemManager->setSystem(new SpriteSystem(&window));
	systemManager->initializeAll();

	artemis::Entity& background = entityManager->create();
	background.addComponent(new PositionComponent(0,0));
	background.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource("Media/Images/MainMenuBackground.jpg")));
	background.refresh();

	sf::Font font;
	font.loadFromFile("Media/Fonts/font1.ttf");

	artemis::Entity& back = entityManager->create();
	back.addComponent(new PositionComponent(70,70));
	back.addComponent(new MenuTextComponent("Return", font, sf::Color::Green, sf::Color::White, 40));
	back.addComponent(new PlayerInputComponent(0, 30));
	back.refresh();

	groupManager->set(menuGrouping, back);

	creditsWorld.loopStart();
	creditsWorld.setDelta(0.0016f);
}

void CreditsMenuState::Draw(sf::RenderWindow& window)
{
	systemManager->getSystem<SpriteSystem>()->process();
	systemManager->getSystem<MenuComponentRenderingSystem>()->process();
}

void CreditsMenuState::Update(sf::Event& event)
{
	systemManager->getSystem<MenuComponentSystem>()->process();
	((MenuComponentSystem*)systemManager->getSystem<MenuComponentSystem>())->Update(event);
}

void CreditsMenuState::Update(I_Subject* theChangeSubject)
{
	// For now just call notify because we always want to return to the main menu from the Credits menu
	if(theChangeSubject->getValue() == "Back")
	{
		notify();
	}
}
