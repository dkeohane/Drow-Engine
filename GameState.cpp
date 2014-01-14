#include "GameState.h"
#include "CameraComponent.h"
#include "CameraSystem.h"

void GameState::loadMap(const std::string filepath)
{
	//open up the file
	std::filebuf fb;
	fb.open(filepath, std::ios::in);

	//try to parse it
	Json::Value root;
	Json::Reader reader;
	bool parseSuccessful = reader.parse(std::istream(&fb), root);
	//now we have it parsed we can close it
	fb.close();

	if(!parseSuccessful)
	{
		std::cerr << "Unable to load entity: " << filepath << std::endl;
		std::cerr << reader.getFormatedErrorMessages() << std::endl;
		return;
	}

	Json::Value numberMapping = root.get("NumberMap", NULL);
	if (numberMapping == NULL)
	{
		cout << "Number Values is NULL" << endl;
	}

	cout << numberMapping << endl;
	for(unsigned int i = 0; i < numberMapping.size(); i++)
	{
		Json::Value numberData = numberMapping[i];
		int num = numberData["name"].asInt();
		string textureFilepath = numberData["textureFilepath"].asString();
		bool collision = numberData["Collision"].asBool();
		addBackgroundEntity(num, textureFilepath, collision);
	}

	Json::Value mapData = root.get("Map1", NULL);
	if(mapData == NULL)
	{
		cout << "OH SHIT!?" << endl;
	}

	for(unsigned int i = 0; i < mapData.size(); i++)
	{
		Json::Value mapRow = mapData[i];
		for (unsigned int j = 0; j < mapRow.size(); j++)
		{
			artemis::Entity& e = this->entityManager->create();

			for(unsigned int k = 0; k < (backgroundMapping[mapRow[j].asInt()])->getComponents().getCount(); k++)
			{
				e.addComponent((backgroundMapping[mapRow[j].asInt()])->getComponents().get(k));
			}
			e.addComponent(new PositionComponent(j * 32, i * 32));
			e.refresh();
		}

	}
}



void GameState::Load(sf::RenderWindow& window)
{
	entityManager = gameWorld.getEntityManager();
	systemManager = gameWorld.getSystemManager();
	groupManager = gameWorld.getGroupManager();
	tagManager = gameWorld.getTagManager();

	systemManager->setSystem(new MovementSystem());
	systemManager->setSystem(new SpriteSystem(&window));
	systemManager->setSystem(new AnimationSystem(&window));
	systemManager->setSystem(new MenuComponentRenderingSystem(&window));
	systemManager->setSystem(new CameraSystem());
	//systemManager->setSystem(new MenuComponentSystem());
	systemManager->initializeAll();


	/*artemis::Entity& background = this->entityManager->create();
	background.addComponent(new PositionComponent(0,0));
	background.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource("Media/Images/background1.png")));
	background.refresh();
	*/

	sf::Texture playerTexture;
	playerTexture.loadFromFile("Media/Images/player.png");
	artemis::Entity& player = this->entityManager->create();
	player.addComponent(new VelocityComponent(3,3));
	player.addComponent(new PositionComponent(20,20));
	player.addComponent(new SpriteComponent(playerTexture));
	//player.addComponent(new PlayerInputComponent(0, 30.0f));
	player.setTag("Player");
	player.refresh();

	

	artemis::Entity& avatar = this->entityManager->create();
	avatar.addComponent(new PositionComponent(20,20));
	avatar.addComponent(new PlayerInputComponent(0, 30.0f));
	avatar.addComponent(new VelocityComponent(3,10));
	
	// FIX ME
	v.setSize(sf::Vector2f(1000,1000));
	avatar.addComponent(new CameraComponent(&window, v));
	// PLZ

	std::map<string, sf::Texture*> textures;
	std::map<std::string, Animation*> *animationListPtr = new std::map<std::string, Animation*>();
	std::vector<string> stateStrings;
	

	Animation* playerDownAnimation = new Animation();
	sf::Vector2i origin(0,0);
	int frameCount = 3;
	sf::Vector2i frameSize(21, 25);
	sf::Time frameDuration = sf::milliseconds(50);
	playerDownAnimation->addRow(origin, frameCount, frameSize, frameDuration);

	Animation* playerUpAnimation = new Animation();
	origin = sf::Vector2i(0, 0);
	frameCount = 3;
	frameSize = sf::Vector2i(21, 25);
	frameDuration = sf::milliseconds(50);
	playerUpAnimation->addRow(origin, frameCount, frameSize, frameDuration);

	Animation* playerLeftAnimation = new Animation();
	origin = sf::Vector2i(0, 0);
	frameCount = 3;
	frameSize = sf::Vector2i(21, 24);
	frameDuration = sf::milliseconds(50);
	playerUpAnimation->addRow(origin, frameCount, frameSize, frameDuration);

	Animation* playerRightAnimation = new Animation();
	origin = sf::Vector2i(0, 0);
	frameCount = 3;
	frameSize = sf::Vector2i(21, 24);
	frameDuration = sf::milliseconds(50);
	playerUpAnimation->addRow(origin, frameCount, frameSize, frameDuration);

	Animation* idleAnimation = new Animation();
	origin = sf::Vector2i(0, 0);
	frameCount = 12;
	frameSize = sf::Vector2i(21, 25);
	frameDuration = sf::milliseconds(50);
	idleAnimation->addRow(origin, frameCount, frameSize, frameDuration);

	// UP
	(*animationListPtr)["Media/Images/playerUp.png"] = idleAnimation;
	textures["Media/Images/playerUp.png"] = (TextureManager::getInstance()->getResource("Media/Images/playerUp.png"));
	stateStrings.push_back("Media/Images/playerUp.png");

	// DOWN
	(*animationListPtr)["Media/Images/playerDown2.png"] = idleAnimation;
	textures["Media/Images/playerDown2.png"] = (TextureManager::getInstance()->getResource("Media/Images/playerDown2.png"));
	stateStrings.push_back("Media/Images/playerDown2.png");

	// LEFT
	(*animationListPtr)["Media/Images/playerLeft.png"] = idleAnimation;
	textures["Media/Images/playerLeft.png"] = (TextureManager::getInstance()->getResource("Media/Images/playerLeft.png"));
	stateStrings.push_back("Media/Images/playerLeft.png");
	
	// RIGHT
	(*animationListPtr)["Media/Images/playerRight.png"] = idleAnimation;
	textures["Media/Images/playerRight.png"] = (TextureManager::getInstance()->getResource("Media/Images/playerRight.png"));
	stateStrings.push_back("Media/Images/playerRight.png");

	// IDLE
	(*animationListPtr)["Media/Images/playerDown2.png"] = idleAnimation;
	textures["Media/Images/playerDown2.png"] = (TextureManager::getInstance()->getResource("Media/Images/playerDown2.png"));
	stateStrings.push_back("Media/Images/playerDown2.png");

	avatar.addComponent(new AnimationComponent(animationListPtr, textures));
	avatar.addComponent(new MovementComponent(stateStrings));
	((MovementComponent*)avatar.getComponent<MovementComponent>())->attach((AnimationComponent*)avatar.getComponent<AnimationComponent>());

	Ability a("Ability_1", 10, 10);
	Ability b("Ability_2", 10, 10);
	Ability c("Ability_3", 10, 10);
	Ability d("Ability_4", 10, 10);
	
	/*
	std::vector<Ability> abilities;
	abilities.push_back(a);
	abilities.push_back(b);
	abilities.push_back(c);
	abilities.push_back(d);
	*/

	std::map<string, Ability> abilities;
	abilities["Ability_1"]  = a;
	abilities["Ability_2"]  = b;
	abilities["Ability_3"]  = c;
	abilities["Ability_4"]  = d;

	avatar.addComponent(new CharacterRPGAttributes(abilities));
	avatar.refresh();

	loadMap("Config/Maps/Map1.json");


	gameWorld.loopStart();
	gameWorld.setDelta(0.0016f);
}

void GameState::Update(sf::Event& event)
{

}


void GameState::Draw(sf::RenderWindow& window)
{	
	//window.draw(bgSprite);
	systemManager->getSystem<CameraSystem>()->process();
	systemManager->getSystem<MovementSystem>()->process();
	systemManager->getSystem<SpriteSystem>()->process();
	systemManager->getSystem<AnimationSystem>()->process();
	systemManager->getSystem<MenuComponentRenderingSystem>()->process();
	//systemManager->getSystem<MenuComponentSystem>()->process();
}

void GameState::addBackgroundEntity(int num, string& textureFilepath, bool collision)
{
	// Do nothing with collision for the moment

	std::map<int, artemis::Entity*>::const_iterator it = backgroundMapping.find(num);
	if(it == backgroundMapping.end())
	{
		artemis::Entity* e = &this->entityManager->create();
		e->addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource(textureFilepath)));
		e->refresh();
		backgroundMapping[num] = e;
	}
	else
	{
		cout << "It already exists" << endl;
	}
}
