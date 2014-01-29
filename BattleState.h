#ifndef BATTLESTATE_H_
#define BATTLESTATE_H_

#include "ScreenState.h"
#include "Ability.h"

// Components
#include "PositionComponent.h"
#include "SpriteComponent.h"
#include "VelocityComponent.h"
#include "CharacterRPGAttributes.h"

// Systems
#include "SpriteSystem.h"
#include "AnimationSystem.h"
#include "MenuComponentSystem.h"
#include "BattleSystem.h"
#include "BattleHUDSystem.h"
#include "MenuComponentGroupRenderingSystem.h"

#include "MapLoader.h"
#include "TextureManager.h"

class BattleState: public ScreenState, public I_Observer
{
public:
	BattleState(sf::RenderWindow& window){ this->Load(window); }
	~BattleState(){}

	virtual void Load(sf::RenderWindow& window);
	virtual void Draw(sf::RenderWindow& window);
	virtual void Update(sf::Event& event);

	// Observer Update
	virtual void Update(I_Subject* theChangeSubject);
	// Subject getValue
	virtual string getValue(){ return "GameState"; }

	void createHUD(artemis::Entity& player, artemis::Entity& enemy);
	void createEnemy();

	static const enum BattleStates
	{
		ACTION_DECISION,
		ATTACK_DECISION,
		ENEMY_DECISION,
		ENEMY_ATTACK,
		RUN,
		_Count
	};

private:
	artemis::World battleWorld;
	string subjectValue;

	BattleStates currentState;

	MenuComponentSystem* menuComponentSystem;
	MenuComponentGroupRenderingSystem* mcsRenderer;
	BattleSystem* battleSystem;
	artemis::Entity* menuEntity;

	//Managers
	artemis::EntityManager* entityManager;
	artemis::SystemManager* systemManager;
	artemis::GroupManager* groupManager;
	artemis::TagManager* tagManager;

	MapLoader* mapLoader;

	std::vector<artemis::Entity*> playerAttacks;
};
#endif