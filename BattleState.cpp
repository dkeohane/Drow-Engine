#include "BattleState.h"

void BattleState::Load(sf::RenderWindow& window)
{
	entityManager = battleWorld.getEntityManager();
	systemManager = battleWorld.getSystemManager();
	groupManager = battleWorld.getGroupManager();
	tagManager = battleWorld.getTagManager();

	mapLoader = new MapLoader(&battleWorld);

	// temp
	artemis::Entity& background = entityManager->create();
	background.addComponent(new PositionComponent(0,0));
	background.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource("Media/Images/background1.png")));
	background.refresh();
	// end temp

	mapLoader->loadPlayerDetails("Config/Maps/Player.json");
	createEnemy();
	createHUD(tagManager->getEntity("Player"), tagManager->getEntity("Enemy"));

	battleSystem = new BattleSystem(&tagManager->getEntity("Player"), &tagManager->getEntity("Enemy"));
	menuComponentSystem = new MenuComponentSystem("ActionDecisionState");
	menuComponentSystem->attach(this);

	mcsRenderer = new MenuComponentGroupRenderingSystem(&window);
	mcsRenderer->setMenuComponentsGroup("ActionDecisionState");

	systemManager->setSystem(battleSystem);
	systemManager->setSystem(mcsRenderer);
	systemManager->setSystem(new SpriteSystem(&window));
	systemManager->setSystem(menuComponentSystem);
	systemManager->setSystem(new AnimationSystem(&window));
	systemManager->setSystem(new BattleHUDSystem(&window));
	systemManager->initializeAll();

	battleSystem->setPlayerAttack(&tagManager->getEntity("abilityAttack"));
	battleSystem->setEnemyAttack(&tagManager->getEntity("abilityAttack"));

	// temp
	artemis::Entity& actionDecisionBorder = entityManager->create();
	actionDecisionBorder.addComponent(new PositionComponent(40, 550));
	actionDecisionBorder.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource("Media/Images/actionDecisionBorder.png")));
	actionDecisionBorder.refresh();
	// end temp

	mapLoader->loadTextComponents("Config/Maps/ActionDecisionTextComponents.json", false, NULL);
	mapLoader->loadTextComponents("Config/Maps/AttackDecisionTextComponents.json", false, &playerAttacks);

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
		mcsRenderer->setMenuComponentsGroup("AttackDecisionState"); 

		CharacterRPGAttributes* c = (CharacterRPGAttributes*)tagManager->getEntity("Player").getComponent<CharacterRPGAttributes>();
		std::vector<string> abilityNames = c->getAbilityNames();
		for(unsigned int i = 0; i < abilityNames.size(); i++)
		{
			playerAttacks.at(i)->removeComponent<MenuTextComponent>();
			playerAttacks.at(i)->addComponent(new MenuTextComponent(abilityNames.at(i), *FontManager::getInstance()->getResource("Media/Fonts/font1.ttf"), sf::Color::Green, sf::Color::White, 40));
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