#include "BattleState.h"


void BattleState::Load(sf::RenderWindow& window)
{
	entityManager = battleWorld.getEntityManager();
	systemManager = battleWorld.getSystemManager();
	groupManager = battleWorld.getGroupManager();
	tagManager = battleWorld.getTagManager();

	mcs = new MenuComponentSystem("ActionDecisionState");
	mcs->attach(this);
	mcsRenderer = new MenuComponentGroupRenderingSystem(&window);
	mcsRenderer->setMenuComponentsGroup("ActionDecisionState");


	createPlayer();
	createAttackComponents();

	battleSystem = new BattleSystem(&tagManager->getEntity("Player"), &tagManager->getEntity("Player"));

	systemManager->setSystem(battleSystem);
	systemManager->setSystem(new SpriteSystem(&window));
	systemManager->setSystem(mcs);
	systemManager->setSystem(new AnimationSystem(&window));
	systemManager->setSystem(new BattleHUDSystem(&window));
	systemManager->setSystem(mcsRenderer);
	systemManager->initializeAll();

	
	font.loadFromFile("Media/Fonts/font1.ttf");

	artemis::Entity& attack = entityManager->create();
	attack.addComponent(new PositionComponent(70,70));
	attack.addComponent(new MenuTextComponent("Attack", font, sf::Color::Green, sf::Color::White, 40));
	attack.addComponent(new PlayerInputComponent(0, 30));
	attack.refresh();

	artemis::Entity& run = entityManager->create();
	run.addComponent(new PositionComponent(270,70));
	run.addComponent(new MenuTextComponent("Run", font, sf::Color::Green, sf::Color::White, 40));
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
	}
}

void BattleState::Update(sf::Event& event)
{
	mcs->Update(event);
}

void BattleState::Update(I_Subject* theChangeSubject)
{
	if(theChangeSubject->getValue() == "Attack")
	{
		currentState = ATTACK_DECISION;
		mcs->setMenuComponentsGroup("AttackDecisionState"); // Update the MenuComponents group to be handled
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
		mcs->setMenuComponentsGroup("ActionDecisionState");
		mcsRenderer->setMenuComponentsGroup("ActionDecisionState");
	}
	else // it is an attack
	{
		cout << theChangeSubject->getValue() << endl;
		this->battleSystem->setAttack(theChangeSubject->getValue());
		currentState = ACTION_DECISION;
		mcs->setMenuComponentsGroup("ActionDecisionState");
		mcsRenderer->setMenuComponentsGroup("ActionDecisionState");
	}
}

void BattleState::createPlayer()
{
	// Will get player and entity from Memento pattern
	artemis::Entity& player = this->entityManager->create();
	player.setTag("Player");

	player.addComponent(new PositionComponent(200,200));
	player.addComponent(new PlayerInputComponent(0, 30.0f));
	player.addComponent(new VelocityComponent(3,3));

	std::map<string, sf::Texture*> textures;
	textures["idle"] = (TextureManager::getInstance()->getResource("Media/Images/avatar.png"));

	std::map<std::string, Animation*> *animationListPtr = new std::map<std::string, Animation*>();
	Animation* avatarAnimation = new Animation();
	sf::Vector2i origin(0, 0);
	int frameCount = 12;
	sf::Vector2i frameSize(80, 80);
	sf::Time frameDuration = sf::milliseconds(50);
	avatarAnimation->addRow(origin, frameCount, frameSize, frameDuration);
	(*animationListPtr)["idle"] = avatarAnimation;

	player.addComponent(new AnimationComponent(animationListPtr, textures));

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

	std::vector<sf::Texture> barT;
	std::vector<sf::Texture> barBGT;

	barT.push_back(*TextureManager::getInstance()->getResource("Media/Images/healthBar.png"));
	barBGT.push_back(*TextureManager::getInstance()->getResource("Media/Images/healthBarBackground.png"));


	sf::Vector2f barPos1(100.0f, 100.0f);
	sf::Vector2f barBGPos1(100.0f, 100.0f);

	std::vector<sf::Vector2f> pos1;
	std::vector<sf::Vector2f> pos2;

	pos1.push_back(barPos1);
	pos2.push_back(barBGPos1);

	player.addComponent(new HUDComponent(barT, barBGT, pos1, pos2));

	player.addComponent(new CharacterRPGAttributes(abilities));
	player.refresh();
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
