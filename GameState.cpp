#include "GameState.h"

void GameState::Load(sf::RenderWindow& window)
{
	entityManager = gameWorld.getEntityManager();
	systemManager = gameWorld.getSystemManager();
	groupManager = gameWorld.getGroupManager();
	tagManager = gameWorld.getTagManager();

	systemManager->setSystem(new MovementSystem());
	systemManager->setSystem(new SpriteSystem(&window));
	systemManager->setSystem(new AnimationSystem(&window));
	systemManager->setSystem(new CameraSystem());

	// new
	systemManager->setSystem(new CollisionSystem());
	systemManager->setSystem(new CollisionboxUpdateSystem(&window));
	// bleh

	systemManager->initializeAll();

	mapLoader = new MapLoader(&gameWorld);

	createPlayer(window);
	mapLoader->loadMap("Config/Maps/Map1.json");

	artemis::Entity& temp = this->entityManager->create();
	temp.addComponent(new PositionComponent(2000,200));
	temp.addComponent(new CollisionComponent());
	((CollisionComponent*)temp.getComponent<CollisionComponent>())->setCollisionBox(sf::IntRect(900,200,100,100));
	groupManager->set("Wall", temp);
	temp.refresh();

	gameWorld.loopStart();
	gameWorld.setDelta(0.0016f);
}

void GameState::Update(sf::Event& event)
{

}


void GameState::Draw(sf::RenderWindow& window)
{	
	//window.draw(bgSprite);
	systemManager->getSystem<CollisionboxUpdateSystem>()->process();
	systemManager->getSystem<CollisionSystem>()->process();
	

	systemManager->getSystem<CameraSystem>()->process();
	systemManager->getSystem<MovementSystem>()->process();
	systemManager->getSystem<SpriteSystem>()->process();
	systemManager->getSystem<AnimationSystem>()->process();
}

void GameState::createPlayer(sf::RenderWindow& window)
{
	std::map<string, sf::Texture*> textures;
	std::map<std::string, Animation*> *animationListPtr = new std::map<std::string, Animation*>();
	std::vector<string> stateStrings;


	Animation* playerDownAnimation = new Animation();
	sf::Vector2i origin(0,0);
	int frameCount = 3;
	sf::Vector2i frameSize(21, 25);
	sf::Time frameDuration = sf::milliseconds(500);
	playerDownAnimation->addRow(origin, frameCount, frameSize, frameDuration);

	Animation* playerUpAnimation = new Animation();
	origin = sf::Vector2i(0, 0);
	frameCount = 3;
	frameSize = sf::Vector2i(21, 25);
	frameDuration = sf::milliseconds(500);
	playerUpAnimation->addRow(origin, frameCount, frameSize, frameDuration);

	Animation* playerLeftAnimation = new Animation();
	origin = sf::Vector2i(0, 0);
	frameCount = 3;
	frameSize = sf::Vector2i(21, 25);
	frameDuration = sf::milliseconds(500);
	playerLeftAnimation->addRow(origin, frameCount, frameSize, frameDuration);

	Animation* playerRightAnimation = new Animation();
	origin = sf::Vector2i(0, 0);
	frameCount = 3;
	frameSize = sf::Vector2i(21, 25);
	frameDuration = sf::milliseconds(500);
	playerRightAnimation->addRow(origin, frameCount, frameSize, frameDuration);

	Animation* idleAnimation = new Animation();
	origin = sf::Vector2i(0, 0);
	frameCount = 1;
	frameSize = sf::Vector2i(21, 25);
	frameDuration = sf::milliseconds(100);
	idleAnimation->addRow(origin, frameCount, frameSize, frameDuration);

	// UP
	(*animationListPtr)["Media/Images/playerUp.png"] = playerUpAnimation;
	textures["Media/Images/playerUp.png"] = (TextureManager::getInstance()->getResource("Media/Images/playerUp.png"));
	stateStrings.push_back("Media/Images/playerUp.png");

	// DOWN
	(*animationListPtr)["Media/Images/playerDown2.png"] = playerDownAnimation;
	textures["Media/Images/playerDown2.png"] = (TextureManager::getInstance()->getResource("Media/Images/playerDown2.png"));
	stateStrings.push_back("Media/Images/playerDown2.png");

	// LEFT
	(*animationListPtr)["Media/Images/playerLeft.png"] = playerLeftAnimation;
	textures["Media/Images/playerLeft.png"] = (TextureManager::getInstance()->getResource("Media/Images/playerLeft.png"));
	stateStrings.push_back("Media/Images/playerLeft.png");

	// RIGHT
	(*animationListPtr)["Media/Images/playerRight.png"] = playerRightAnimation;
	textures["Media/Images/playerRight.png"] = (TextureManager::getInstance()->getResource("Media/Images/playerRight.png"));
	stateStrings.push_back("Media/Images/playerRight.png");

	// IDLE
	(*animationListPtr)["Media/Images/playerDown.png"] = idleAnimation;
	textures["Media/Images/playerDown.png"] = (TextureManager::getInstance()->getResource("Media/Images/playerDown.png"));
	stateStrings.push_back("Media/Images/playerDown.png");

	artemis::Entity& player = this->entityManager->create();
	player.addComponent(new PositionComponent(20,20));
	player.addComponent(new PlayerInputComponent(0, 30.0f));
	// FIX ME
	v.setSize(sf::Vector2f(800,600));
	player.addComponent(new CameraComponent(&window, v));
	// PLZ
	player.addComponent(new VelocityComponent(6.f, 6.f));
	player.addComponent(new AnimationComponent(animationListPtr, textures));
	player.addComponent(new MovementComponent(stateStrings));
	((MovementComponent*)player.getComponent<MovementComponent>())->attach((AnimationComponent*)player.getComponent<AnimationComponent>());

	Ability a("Ability_1", 10, 10, "Media/Images/playerDown2.png");
	Ability b("Ability_2", 10, 10, "Media/Images/playerDown2.png");
	Ability c("Ability_3", 10, 10, "Media/Images/playerDown2.png");
	Ability d("Ability_4", 10, 10, "Media/Images/playerDown2.png");

	std::map<string, Ability> abilities;
	abilities["Ability_1"]  = a;
	abilities["Ability_2"]  = b;
	abilities["Ability_3"]  = c;
	abilities["Ability_4"]  = d;

	groupManager->set("Player", player);
	
	player.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource("Media/Images/playerDown2.png")));
	player.addComponent(new CharacterRPGAttributes(abilities));
	player.addComponent(new CollisionComponent());
	player.refresh();
}