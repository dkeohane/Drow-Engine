#ifndef BATTLESTATE_H_
#define BATTLESTATE_H_

#include "ScreenState.h"
#include "Ability.h"

// Components
#include "PositionComponent.h"
#include "SpriteComponent.h"
#include "CharacterRPGComponent.h"
#include "CharacterRPGComponent.h"

// Systems
#include "SpriteSystem.h"
#include "AnimationSystem.h"
#include "MenuComponentSystem.h"
#include "BattleHUDSystem.h"
#include "CameraSystem.h"
#include "MenuComponentGroupRenderingSystem.h"

#include "FileLoader.h"
#include "TextureManager.h"



class BattleState: public ScreenState, public I_Observer
{
public:
	BattleState(sf::RenderWindow& window){ this->Load(window); }
	~BattleState(){}

	static const enum BattleStates
	{
		ACTION_DECISION,
		ATTACK_DECISION,
		PLAYERTURN,
		ENEMYTURN,
		RUN,
		_Count
	};

	virtual void Load(sf::RenderWindow& window);
	virtual void ProcessState();
	virtual void Update(sf::Event& event);

	// Observer Update
	virtual void Update(I_Subject* theChangeSubject);
	// Subject getValue
	virtual string getValue(){ return "GameState"; }



	void createHUD(artemis::Entity& player, artemis::Entity& enemy);
	void createEnemy();
	void setStateSpriteComponents(BattleStates state, bool enable);
	
	void abilitySelection(string abilityName);
	double calculateDamage(Ability* ability, artemis::Entity* invoker, artemis::Entity* receiver){ return 10.0; }
	double calculateExperienceGain(){ return 5.0; }

	void usePlayerAbility(Ability* ability);
	void useEnemyAbility(Ability* ability);

	void setPlayer(artemis::Entity* player){ this->player = player; }
	void setEnemy(artemis::Entity* enemy){ this->enemy = enemy; }

private:
	artemis::World battleWorld;
	string subjectValue;

	BattleStates currentState;

	MenuComponentSystem* menuComponentSystem;
	MenuComponentGroupRenderingSystem* mcsRenderer;
	
	artemis::Entity* menuEntity;
	artemis::Entity* player;
	artemis::Entity* enemy;
	artemis::Entity* playerAttack;
	artemis::Entity* enemyAttack;

	//Managers
	artemis::EntityManager* entityManager;
	artemis::SystemManager* systemManager;
	artemis::GroupManager* groupManager;
	artemis::TagManager* tagManager;

	FileLoader* fileLoader;

	std::map<BattleStates, string> stateSpriteGroups;
	std::vector<artemis::Entity*> playerAttacks;

	artemis::ComponentMapper<SpriteComponent> spriteMapper;
	artemis::ComponentMapper<MenuTextComponent> menuTextMapper;
	artemis::ComponentMapper<AnimationComponent> animationMapper;
	artemis::ComponentMapper<CharacterRPGComponent> RPGattributesMapper;
};
#endif