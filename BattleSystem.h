#ifndef BATTLESYSTEM_H_
#define BATTLESYSTEM_H_

#include "Global.h"

#include "CharacterRPGAttributes.h"
#include "AnimationComponent.h"

class BattleSystem: public artemis::EntityProcessingSystem
{
public:
	BattleSystem(artemis::Entity* player, artemis::Entity* enemy)
	{
		addComponentType<CharacterRPGAttributes>();
		setPlayer(player);
		setEnemy(enemy);
	}
	~BattleSystem(){}

	static const enum Turn
	{
		PLAYERTURN,
		ENEMYTURN
	};


	virtual void initialize() 
	{
		RPGattributesMapper.init(*world);
		animationMapper.init(*world);
		groupManager = world->getGroupManager();
		tagManager = world->getTagManager();
		currentTurn = PLAYERTURN;
		attack = "";
	}

	virtual void processEntity(artemis::Entity &e){ /* blank */ }

	void setAttack(string attack)
	{
		abilitySelection(attack);
		abilitySelection("Ability_2");
	}
	
	void abilitySelection(string abilityName)
	{
		
		if(RPGattributesMapper.get(*player)->getCurrentHealth() > 0 && RPGattributesMapper.get(*enemy)->getCurrentHealth() > 0)
		{
			if(currentTurn == PLAYERTURN)
			{
				Ability* a = RPGattributesMapper.get(*player)->getAbility(abilityName);
				useAbility(a, player, enemy);
				currentTurn = ENEMYTURN;
			}
			else if(currentTurn == ENEMYTURN)
			{
				Ability* a = RPGattributesMapper.get(*enemy)->getAbility(abilityName);
				useAbility(a, enemy, player);
				currentTurn = PLAYERTURN;
			}
		}
		else
		{
			cout << "SOMEONE IS DEAD" << endl;
		}
	}

	double calculateDamage(Ability* ability, artemis::Entity* invoker, artemis::Entity* receiver)
	// Could replace later with the strategy pattern
	{
		return 10.0;
	}

	void useAbility(Ability* ability, artemis::Entity* invoker, artemis::Entity* receiver)
	{
		double damage = calculateDamage(ability, invoker, receiver);

		// Damage has already been calculated, now just take it from the receivers HP
		cout << "Health before" << RPGattributesMapper.get(*receiver)->getCurrentHealth() << endl;
		RPGattributesMapper.get(*receiver)->setCurrentHealth(RPGattributesMapper.get(*receiver)->getCurrentHealth() - damage);
		cout << "Health after" << RPGattributesMapper.get(*receiver)->getCurrentHealth() << endl;

		if(animationMapper.get(*invoker) != NULL)
		{
			animationMapper.get(*invoker)->setCurrentAnimation(ability->abilityAnimationFilepath);
		}
	}

	void reset()
	{
		Turn currentTurn = PLAYERTURN;
		string attack = "";

		artemis::Entity* player = NULL;
		artemis::Entity* enemy = NULL;
	}

	void setPlayer(artemis::Entity* player){ this->player = player; }
	void setEnemy(artemis::Entity* enemy){ this->enemy = enemy; }

private:
	Turn currentTurn;
	string attack;

	artemis::Entity* e;
	artemis::Entity* player;
	artemis::Entity* enemy;

	artemis::TagManager* tagManager;
	artemis::GroupManager* groupManager;
	artemis::ComponentMapper<CharacterRPGAttributes> RPGattributesMapper;
	artemis::ComponentMapper<AnimationComponent> animationMapper;
};
#endif