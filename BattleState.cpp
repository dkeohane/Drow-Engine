#include "BattleState.h"

void BattleState::Load(sf::RenderWindow& window)
{
	// Artemis Managers
	entityManager = battleWorld.getEntityManager();
	systemManager = battleWorld.getSystemManager();
	groupManager = battleWorld.getGroupManager();
	tagManager = battleWorld.getTagManager();
	
	// Entity Component Mappers
	spriteMapper.init(battleWorld);
	menuTextMapper.init(battleWorld);
	animationMapper.init(battleWorld);
	RPGattributesMapper.init(battleWorld);

	fileLoader = new FileLoader(&battleWorld);
	window.setView(sf::View(sf::Vector2f(400, 300), sf::Vector2f(800, 600)));

	stateSpriteGroups[ACTION_DECISION] = "ActionDecisionSprites";
	stateSpriteGroups[ATTACK_DECISION] = "AttackDecisionSprites";

	fileLoader->loadStateSprites("Config/Maps/BattleStateSprites.json");
	setStateSpriteComponents(ATTACK_DECISION, false);

	fileLoader->loadPlayerDetails("Config/Maps/Player.json");
	createEnemy();
	//createHUD(tagManager->getEntity("Player"), tagManager->getEntity("Enemy"));
	this->setPlayer(&tagManager->getEntity("Player"));
	this->setEnemy(&tagManager->getEntity("Enemy"));

	menuComponentSystem = new MenuComponentSystem("ActionDecisionState");
	menuComponentSystem->attach(this);

	mcsRenderer = new MenuComponentGroupRenderingSystem(&window);
	mcsRenderer->setMenuComponentsGroup("ActionDecisionState");
	
	systemManager->setSystem(mcsRenderer);
	systemManager->setSystem(new SpriteSystem(&window));
	systemManager->setSystem(menuComponentSystem);
	systemManager->setSystem(new AnimationSystem(&window));
	systemManager->setSystem(new BattleHUDSystem(&window));
	systemManager->initializeAll();

	fileLoader->loadTextComponents("Config/Maps/ActionDecisionTextComponents.json", false, NULL);
	fileLoader->loadTextComponents("Config/Maps/AttackDecisionTextComponents.json", false, &playerAttacks);

	currentState = ACTION_DECISION;
	
	battleWorld.loopStart();
	battleWorld.setDelta(0.0016f);
}

void BattleState::ProcessState()
{
	if(currentState == ACTION_DECISION || currentState == ATTACK_DECISION)
	{
		systemManager->getSystem<SpriteSystem>()->process();
		systemManager->getSystem<MenuComponentSystem>()->process();
		systemManager->getSystem<MenuComponentGroupRenderingSystem>()->process();
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
		setStateSpriteComponents(ACTION_DECISION, false);
		setStateSpriteComponents(ATTACK_DECISION, true);
		currentState = ATTACK_DECISION;
		menuComponentSystem->setMenuComponentsGroup("AttackDecisionState"); // Update the MenuComponents group to be handled
		mcsRenderer->setMenuComponentsGroup("AttackDecisionState"); 

		std::vector<string> abilityNames = RPGattributesMapper.get(*player)->getAbilityNames();
		for(unsigned int i = 0; i < abilityNames.size(); i++)
		{
			menuTextMapper.get(*playerAttacks.at(i))->setComponentText(abilityNames.at(i));
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
		if(currentState != ACTION_DECISION)
		{
			setStateSpriteComponents(ACTION_DECISION, true);
			setStateSpriteComponents(ATTACK_DECISION, false);
			currentState = ACTION_DECISION;
			menuComponentSystem->setMenuComponentsGroup("ActionDecisionState");
			mcsRenderer->setMenuComponentsGroup("ActionDecisionState");
		}
	}
	else // it is a player attack
	{
		setStateSpriteComponents(ACTION_DECISION, true);
		setStateSpriteComponents(ATTACK_DECISION, false);

		cout << theChangeSubject->getValue() << endl;

		// This is a player attack
		currentState = PLAYERTURN;
		this->abilitySelection(theChangeSubject->getValue());

		currentState = ENEMYTURN;
		// add calculate enemy method here
		this->abilitySelection(theChangeSubject->getValue());

		currentState = ACTION_DECISION;
		menuComponentSystem->setMenuComponentsGroup("ActionDecisionState");
		mcsRenderer->setMenuComponentsGroup("ActionDecisionState");
	}
}

void BattleState::abilitySelection(string abilityName)
{		
	if(RPGattributesMapper.get(*player)->getAttributeValue(CharacterAttributes::CURRENTHEALTH) > 0 && RPGattributesMapper.get(*enemy)->getAttributeValue(CharacterAttributes::CURRENTHEALTH) > 0)
	{
		if(currentState == PLAYERTURN)
		{
			usePlayerAbility(RPGattributesMapper.get(*player)->getAbility(abilityName));
		}
		else if(currentState == ENEMYTURN)
		{
			useEnemyAbility(RPGattributesMapper.get(*enemy)->getAbility(abilityName));
		}
	}
	else
	{
		cout << "SOMEONE IS DEAD" << endl;
	}
}

void BattleState::usePlayerAbility(Ability* ability)
{
	double damage = calculateDamage(ability, player, enemy);
	RPGattributesMapper.get(*enemy)->setAttributeValue(CharacterAttributes::CURRENTHEALTH, RPGattributesMapper.get(*enemy)->getAttributeValue(CharacterAttributes::CURRENTHEALTH) - damage);
}

void BattleState::useEnemyAbility(Ability* ability)
{
	double damage = calculateDamage(ability, enemy, player);
	RPGattributesMapper.get(*player)->setAttributeValue(CharacterAttributes::CURRENTHEALTH, RPGattributesMapper.get(*player)->getAttributeValue(CharacterAttributes::CURRENTHEALTH) - damage);
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
	enemy.addComponent(new PositionComponent(550, 50));
	enemy.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource("Media/Images/enemy1.png")));
	spriteMapper.get(enemy)->getSprite()->setScale(.75f, .75f);
	enemy.addComponent(new CharacterRPGComponent(abilities));
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

void BattleState::setStateSpriteComponents(BattleStates state, bool enable)
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