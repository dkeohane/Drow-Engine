#ifndef BATTLESYSTEM_H_
#define BATTLESYSTEM_H_

#include "Global.h"

#include "CharacterRPGAttributes.h"

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
		groupManager = world->getGroupManager();
		tagManager = world->getTagManager();
		currentTurn = PLAYERTURN;
		attack = "";
	}

	virtual void processEntity(artemis::Entity &e){ /* blank */ }

	void setAttack(string attack)
	{
		this->attack = attack;
		update();
	}
	
	void update()
	{
		if(RPGattributesMapper.get(*player)->getCurrentHealth() > 0 && RPGattributesMapper.get(*enemy)->getCurrentHealth() > 0)
		{
			if(currentTurn == PLAYERTURN)
			{
				useAbility(attack, player, enemy);
				currentTurn = ENEMYTURN;
			}
			else if(currentTurn == ENEMYTURN)
			{
				// calculate enemy move
				attack = "Ability_1";
				useAbility(attack, enemy, player);
				currentTurn = PLAYERTURN;
			}
		}
		else
		{
			cout << "SOMEONE IS DEAD" << endl;
		}
	}

	void useAbility(std::string abilityName, artemis::Entity* invoker, artemis::Entity* receiver)
	{
		Ability* a = RPGattributesMapper.get(*invoker)->getAbility(abilityName);
		if(a != NULL)
		{
			cout << "Health before" << RPGattributesMapper.get(*receiver)->getCurrentHealth() << endl;
			RPGattributesMapper.get(*receiver)->setCurrentHealth(RPGattributesMapper.get(*receiver)->getCurrentHealth() - a->damageValue);
			cout << "Health after" << RPGattributesMapper.get(*receiver)->getCurrentHealth() << endl;
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
};
#endif