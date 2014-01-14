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

//temp
#include "MenuComponentGroupRenderingSystem.h"
// end temp

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

	// Very temp!
	void createPlayer();
	void createAttackComponents();

	static const enum battleStates
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

	battleStates currentState;

	//temp
	sf::Font font;
	//untemp

	MenuComponentSystem* mcs;
	MenuComponentGroupRenderingSystem* mcsRenderer;
	BattleSystem* battleSystem;
	artemis::Entity* menuEntity;
	
	artemis::Entity* player;


	//Managers
	artemis::EntityManager* entityManager;
	artemis::SystemManager* systemManager;
	artemis::GroupManager* groupManager;
	artemis::TagManager* tagManager;

	std::vector<artemis::Entity*> playerAttacks;
};
#endif