#include "BattleState.h"


void BattleState::Load(sf::RenderWindow& window)
{
	entityManager = battleWorld.getEntityManager();
	systemManager = battleWorld.getSystemManager();
	groupManager = battleWorld.getGroupManager();
	tagManager = battleWorld.getTagManager();

	artemis::Entity& background = entityManager->create();
	background.addComponent(new PositionComponent(0,0));
	background.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource("Media/Images/background1.png")));
	background.refresh();

	createPlayer();
	createEnemy();
	createAttackComponents();
	createHUD(tagManager->getEntity("Player"), tagManager->getEntity("Enemy"));

	battleSystem = new BattleSystem(&tagManager->getEntity("Player"), &tagManager->getEntity("Enemy"));
	menuComponentSystem = new MenuComponentSystem("ActionDecisionState");
	menuComponentSystem->attach(this);
	mcsRenderer = new MenuComponentGroupRenderingSystem(&window);
	mcsRenderer->setMenuComponentsGroup("ActionDecisionState");

	systemManager->setSystem(battleSystem);
	systemManager->setSystem(new SpriteSystem(&window));
	systemManager->setSystem(menuComponentSystem);
	systemManager->setSystem(new AnimationSystem(&window));
	systemManager->setSystem(new BattleHUDSystem(&window));
	systemManager->setSystem(mcsRenderer);
	systemManager->initializeAll();

	artemis::Entity& actionDecisionBorder = entityManager->create();
	actionDecisionBorder.addComponent(new PositionComponent(40, 550));
	actionDecisionBorder.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource("Media/Images/actionDecisionBorder.png")));
	actionDecisionBorder.refresh();

	font.loadFromFile("Media/Fonts/font1.ttf");

	artemis::Entity& attack = entityManager->create();
	attack.addComponent(new PositionComponent((float)window.getSize().x / 3, (float)window.getSize().y - 150));
	attack.addComponent(new MenuTextComponent("Attack", font, sf::Color::Blue, sf::Color::White, 40));
	attack.addComponent(new PlayerInputComponent(0, 30));
	attack.refresh();

	artemis::Entity& run = entityManager->create();
	run.addComponent(new PositionComponent((float)(window.getSize().x / 3) * 2, (float)window.getSize().y - 150));
	run.addComponent(new MenuTextComponent("Run", font, sf::Color::Blue, sf::Color::White, 40));
	run.addComponent(new PlayerInputComponent(0, 30));
	run.refresh();

	groupManager->set("ActionDecisionState", attack);
	groupManager->set("ActionDecisionState", run);



	currentState = ACTION_DECISION;
	
	battleWorld.loopStart();
	battleWorld.setDelta(0.0016f);
}

void BattleState::Draw(sf::RenderWindow& window)
{
	if(currentState == ACTION_DECISION || currentState == ATTACK_DECISION)
	{
		systemManager->getSystem<SpriteSystem>()->process();
		systemManager->getSystem<MenuComponentSystem>()->process();
		systemManager->getSystem<MenuComponentGroupRenderingSystem>()->process();
		systemManager->getSystem<AnimationSystem>()->process();
		systemManager->getSystem<BattleHUDSystem>()->process();
	}
	else // an attack has been chosen
	{
		systemManager->getSystem<SpriteSystem>()->process();
		systemManager->getSystem<AnimationSystem>()->process();
		systemManager->getSystem<BattleHUDSystem>()->process();
	}
}

void BattleState::Update(sf::Event& event)
{
	menuComponentSystem->Update(event);
}

void BattleState::Update(I_Subject* theChangeSubject)
{
	if(theChangeSubject->getValue() == "Attack")
	{
		currentState = ATTACK_DECISION;
		menuComponentSystem->setMenuComponentsGroup("AttackDecisionState"); // Update the MenuComponents group to be handled
		mcsRenderer->setMenuComponentsGroup("AttackDecisionState"); // Update the MenuComponents to be drawn
		CharacterRPGAttributes* c = (CharacterRPGAttributes*)tagManager->getEntity("Player").getComponent<CharacterRPGAttributes>();

		std::vector<string> abilityNames = c->getAbilityNames();
		for(unsigned int i = 0; i < abilityNames.size(); i++)
		{
			playerAttacks.at(i)->removeComponent<MenuTextComponent>();
			playerAttacks.at(i)->addComponent(new MenuTextComponent(abilityNames.at(i), font, sf::Color::Green, sf::Color::White, 40));
			playerAttacks.at(i)->refresh();
		}
	} 
	else if(theChangeSubject->getValue() == "Run")
	{
		// clear player and enemy entities, reset battleState to default, then notify
		notify();
	}
	else if(theChangeSubject->getValue() == "Back")
	{
		// For the moment can only move back from the attack decision state.
		currentState = ACTION_DECISION;
		menuComponentSystem->setMenuComponentsGroup("ActionDecisionState");
		mcsRenderer->setMenuComponentsGroup("ActionDecisionState");
	}
	else // it is an attack
	{
		cout << theChangeSubject->getValue() << endl;
		this->battleSystem->setAttack(theChangeSubject->getValue());
		currentState = ACTION_DECISION;
		menuComponentSystem->setMenuComponentsGroup("ActionDecisionState");
		mcsRenderer->setMenuComponentsGroup("ActionDecisionState");
	}
}

void BattleState::createPlayer()
{
	std::map<string, sf::Texture*> textures;
	std::map<std::string, Animation*> *animationListPtr = new std::map<std::string, Animation*>();
	std::vector<string> stateStrings;


	Animation* playerDownAnimation = new Animation();
	int frameCount = 3;
	sf::Vector2i frameSize(21, 25);
	sf::Time frameDuration = sf::milliseconds(50);
	playerDownAnimation->addRow(sf::Vector2i(0, 0), frameCount, frameSize, frameDuration);

	Animation* playerUpAnimation = new Animation();
	frameSize = sf::Vector2i(21, 25);
	frameDuration = sf::milliseconds(50);
	playerUpAnimation->addRow(sf::Vector2i(0, 0), frameCount, frameSize, frameDuration);

	Animation* playerLeftAnimation = new Animation();
	frameSize = sf::Vector2i(21, 24);
	frameDuration = sf::milliseconds(50);
	playerUpAnimation->addRow(sf::Vector2i(0, 0), frameCount, frameSize, frameDuration);

	Animation* playerRightAnimation = new Animation();
	frameSize = sf::Vector2i(21, 24);
	frameDuration = sf::milliseconds(50);
	playerUpAnimation->addRow(sf::Vector2i(0, 0), 3, frameSize, frameDuration);

	Animation* idleAnimation = new Animation();
	frameSize = sf::Vector2i(21, 25);
	frameDuration = sf::milliseconds(50);
	idleAnimation->addRow(sf::Vector2i(0, 0), 12, frameSize, frameDuration);

	Animation* attackAnimation = new Animation();
	attackAnimation->addRow(sf::Vector2i(0, 0), 5, sf::Vector2i(72,60), sf::milliseconds(100));
	attackAnimation->setRepeats(false);

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

	(*animationListPtr)["Media/Images/throw.png"] = attackAnimation;
	textures["Media/Images/throw.png"] = (TextureManager::getInstance()->getResource("Media/Images/throw.png"));

	artemis::Entity& player = this->entityManager->create();
	player.addComponent(new PositionComponent(100,400));
	player.addComponent(new PlayerInputComponent(0, 30.0f));
	player.addComponent(new VelocityComponent(3,10));
	player.addComponent(new AnimationComponent(animationListPtr, textures));

	Ability a("Ability_1", 10, 10, "Media/Images/throw.png");
	Ability b("Ability_2", 10, 10, "Media/Images/throw.png");
	Ability c("Ability_3", 10, 10, "Media/Images/throw.png");
	Ability d("Ability_4", 10, 10, "Media/Images/throw.png");

	std::map<string, Ability> abilities;
	abilities["Ability_1"]  = a;
	abilities["Ability_2"]  = b;
	abilities["Ability_3"]  = c;
	abilities["Ability_4"]  = d;

	tagManager->subscribe("Player", player);

	player.addComponent(new CharacterRPGAttributes(abilities));
	player.refresh();
}

void BattleState::createEnemy()
{
	Ability a("Ability_1", 10, 10, "Media/Images/attack.png");
	Ability b("Ability_2", 10, 10, "Media/Images/attack.png");
	Ability c("Ability_3", 10, 10, "Media/Images/attack.png");
	Ability d("Ability_4", 10, 10, "Media/Images/attack.png");

	std::map<string, Ability> abilities;
	abilities["Ability_1"]  = a;
	abilities["Ability_2"]  = b;
	abilities["Ability_3"]  = c;
	abilities["Ability_4"]  = d;

	artemis::Entity& enemy = this->entityManager->create();
	enemy.addComponent(new PositionComponent(900, 50));
	enemy.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource("Media/Images/enemy1.png")));
	enemy.addComponent(new CharacterRPGAttributes(abilities));
	tagManager->subscribe("Enemy", enemy);
	enemy.refresh();
}

void BattleState::createAttackComponents()
{
	// Could move to Battle System?


	artemis::Entity& attack1 = entityManager->create();
	attack1.addComponent(new PositionComponent(70, 170));
	attack1.addComponent(new MenuTextComponent("attack 1", font, sf::Color::Green, sf::Color::White, 40));
	attack1.addComponent(new PlayerInputComponent(0, 30));
	attack1.refresh();
	groupManager->set("AttackDecisionState", attack1);

	artemis::Entity& attack2 = entityManager->create();
	attack2.addComponent(new PositionComponent(70, 270));
	attack2.addComponent(new MenuTextComponent("attack 2", font, sf::Color::Green, sf::Color::White, 40));
	attack2.addComponent(new PlayerInputComponent(0, 30));
	attack2.refresh();
	groupManager->set("AttackDecisionState", attack2);

	artemis::Entity& attack3 = entityManager->create();
	attack3.addComponent(new PositionComponent(70, 370));
	attack3.addComponent(new MenuTextComponent("attack 3", font, sf::Color::Green, sf::Color::White, 40));
	attack3.addComponent(new PlayerInputComponent(0, 30));
	attack3.refresh();
	groupManager->set("AttackDecisionState", attack3);

	artemis::Entity& attack4 = entityManager->create();
	attack4.addComponent(new PositionComponent(70, 470));
	attack4.addComponent(new MenuTextComponent("attack 4", font, sf::Color::Green, sf::Color::White, 40));
	attack4.addComponent(new PlayerInputComponent(0, 30));
	attack4.refresh();
	groupManager->set("AttackDecisionState", attack4);

	playerAttacks.push_back(&attack1);
	playerAttacks.push_back(&attack2);
	playerAttacks.push_back(&attack3);
	playerAttacks.push_back(&attack4);
}

void BattleState::createHUD(artemis::Entity& player, artemis::Entity& enemy)
{
	std::vector<sf::Texture> barT;
	std::vector<sf::Texture> barBGT;

	barT.push_back(*TextureManager::getInstance()->getResource("Media/Images/healthBar.png"));
	barBGT.push_back(*TextureManager::getInstance()->getResource("Media/Images/healthBarBackground.png"));


	sf::Vector2f barPos1(105.0f, 105.0f);
	sf::Vector2f barBGPos1(100.0f, 100.0f);

	std::vector<sf::Vector2f> pos1;
	std::vector<sf::Vector2f> pos2;

	pos1.push_back(barPos1);
	pos2.push_back(barBGPos1);

	player.addComponent(new HUDComponent(barT, barBGT, pos1, pos2));

	pos1.clear();
	pos2.clear();

	pos1.push_back(sf::Vector2f(625, 520));
	pos2.push_back(sf::Vector2f(620, 515));

	enemy.addComponent(new HUDComponent(barT, barBGT, pos1, pos2));
}