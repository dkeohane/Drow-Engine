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
	}

	virtual void processEntity(artemis::Entity &e){ /* blank */ }

	void setAttack(string attack)
	{
		abilitySelection(attack);
		//abilitySelection("Ability_2");
	}
	
	void abilitySelection(string abilityName)
	{
		
		if(RPGattributesMapper.get(*player)->getCurrentHealth() > 0 && RPGattributesMapper.get(*enemy)->getCurrentHealth() > 0)
		{
			if(currentTurn == PLAYERTURN)
			{
				Ability* a = RPGattributesMapper.get(*player)->getAbility(abilityName);
				usePlayerAbility(a);
				currentTurn = ENEMYTURN;
			}
			else if(currentTurn == ENEMYTURN)
			{
				Ability* a = RPGattributesMapper.get(*enemy)->getAbility(abilityName);
				useEnemyAbility(a);
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

	double calculateExperienceGain()
	{
		return 5.0;
	}

	void usePlayerAbility(Ability* ability)
	{
		double damage = calculateDamage(ability, player, enemy);

		//animationMapper.get(*playerAttack)->setCurrentAnimation(ability->abilityAnimationFilepath);

		/*
		AnimationComponent* animationComponent = animationMapper.get(*playerAttack);
		
		// will loop until we reach the end of the current animation
		while(!animationComponent->onLastFrame()) 
		{
			if(animationComponent->getCurrentAnimationClock()->getElapsedTime() >= animationComponent->getCurrentFramePtr()->duration)
			{
				if(animationComponent->getCurrentAnimation()->getRepeats() == true || (animationComponent->getCurrentAnimation()->getRepeats() == false && !animationComponent->onLastFrame()))
				{
					animationComponent->advanceFrame();
					animationComponent->setFrame(animationComponent->getCurrentFramePtr()->rect);
				}
				animationComponent->restartClock();
			}
			//window->draw(animationComponent->getSprite());
		}
		animationComponent->pauseCurrentAnimation();
		animationComponent->setFrame(sf::IntRect());
		*/

		// Damage has already been calculated, and the attack animation is over, now to take hp from the enemy
		RPGattributesMapper.get(*enemy)->setCurrentHealth(RPGattributesMapper.get(*enemy)->getCurrentHealth() - damage);
	}

	void useEnemyAbility(Ability* ability)
	{
		double damage = calculateDamage(ability, enemy, player);

		
		//animationMapper.get(*enemyAttack)->setCurrentAnimation(ability->abilityAnimationFilepath);
		/*
		AnimationComponent* animationComponent = animationMapper.get(*enemyAttack);
		// will loop until we reach the end of the current animation
		while(!animationComponent->onLastFrame()) 
		{
			if(animationComponent->getCurrentAnimationClock()->getElapsedTime() >= animationComponent->getCurrentFramePtr()->duration)
			{
				if(animationComponent->getCurrentAnimation()->getRepeats() == true || (animationComponent->getCurrentAnimation()->getRepeats() == false && !animationComponent->onLastFrame()))
				{
					animationComponent->advanceFrame();
					animationComponent->setFrame(animationComponent->getCurrentFramePtr()->rect);
				}
				animationComponent->restartClock();
			}
		}
		animationComponent->pauseCurrentAnimation();
		animationComponent->setFrame(sf::IntRect());
		*/
		RPGattributesMapper.get(*player)->setCurrentHealth(RPGattributesMapper.get(*player)->getCurrentHealth() - damage);
	}

	void reset()
	{
		Turn currentTurn = PLAYERTURN;
		player = enemy = NULL;
	}

	void setPlayer(artemis::Entity* player){ this->player = player; }
	void setEnemy(artemis::Entity* enemy){ this->enemy = enemy; }
	void setPlayerAttack(artemis::Entity* playerAttack){ this->playerAttack = playerAttack; }
	void setEnemyAttack(artemis::Entity* enemyAttack){ this->enemyAttack = enemyAttack; }

private:
	Turn currentTurn;

	artemis::Entity* player;
	artemis::Entity* enemy;
	artemis::Entity* playerAttack;
	artemis::Entity* enemyAttack;

	artemis::TagManager* tagManager;
	artemis::GroupManager* groupManager;
	artemis::ComponentMapper<CharacterRPGAttributes> RPGattributesMapper;
	artemis::ComponentMapper<AnimationComponent> animationMapper;
};
#endif