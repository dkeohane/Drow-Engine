#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "ScreenState.h"

#include "PositionComponent.h"
#include "SpriteComponent.h"
#include "VelocityComponent.h"
#include "PlayerInputComponent.h"
#include "AnimationComponent.h"
#include "CharacterRPGComponent.h"
#include "MenuTextComponent.h"
#include "MovementComponent.h"

#include "MovementSystem.h"
#include "SpriteSystem.h"
#include "AnimationSystem.h"
#include "MenuComponentRenderingSystem.h"
#include "MenuComponentSystem.h"

#include "CollisionboxUpdateSystem.h"
#include "AnimationCollisionboxUpdateSystem.h"
#include "CollisionComponent.h"
#include "CollisionSystem.h"
#include "CollisionType.h"

#include "CameraComponent.h"
#include "ViewRelativePositionUpdateSystem.h"
#include "CameraSystem.h"

#include "TextureManager.h"
#include "MapLoader.h"
#include "InventoryComponent.h"

class GameState : public ScreenState, public I_Observer
{
public:
	GameState(sf::RenderWindow& window){ this->Load(window); }
	~GameState(){};

	static const enum GameplayStates
	{
		PLAYING = 1,
		PAUSED,
		CHARACTER_DETAILS,
		INVENTORY,
		QUIT
	};

	virtual void Load(sf::RenderWindow& window);
	virtual void Draw(sf::RenderWindow& window);
	virtual void Update(sf::Event& event);
	virtual void Update(I_Subject* theChangeSubject);
	virtual string getValue(){ return subjectValue; }

	void handlePlayerInput(sf::Event& event);
	void setStateComponents(GameplayStates state, bool enable);

private:
	string subjectValue;
	artemis::World gameWorld;

	GameplayStates currentState;
	GameplayStates previousState;

	//Managers
	artemis::EntityManager* entityManager;
	artemis::SystemManager* systemManager;
	artemis::GroupManager* groupManager;
	artemis::TagManager* tagManager;
	
	MapLoader* mapLoader;
	std::map<GameplayStates, string> stateEntities;
};

#endif