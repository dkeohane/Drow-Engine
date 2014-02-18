#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "ScreenState.h"

// Systems
#include "SpriteSystem.h"
#include "CameraSystem.h"
#include "ParticleSystem.h"
#include "MovementSystem.h"
#include "CollisionSystem.h"
#include "AnimationSystem.h"
#include "InventoryMenuSystem.h"
#include "MenuComponentSystem.h"
#include "BattleEncounterSystem.h"
#include "CollisionboxUpdateSystem.h"
#include "MenuComponentRenderingSystem.h"
#include "ViewRelativePositionUpdateSystem.h"
#include "AnimationCollisionboxUpdateSystem.h"


#include "TextureManager.h"
#include "FileLoader.h"
#include "ParticleEmitter.h"

class GameState : public ScreenState, public I_Observer
{
public:
	GameState(sf::RenderWindow& window){ this->window = &window; this->Load(window); }
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
	virtual void ProcessState();
	virtual void Update(sf::Event& event);
	virtual void Update(I_Subject* theChangeSubject);
	virtual string getValue(){ return subjectValue; }

	void handlePlayerInput(sf::Event& event);
	void setStateSpriteComponents(GameplayStates state, bool enable);

private:
	sf::RenderWindow* window;

	string subjectValue;
	artemis::World gameWorld;

	// Gameplay states
	GameplayStates currentState;
	GameplayStates previousState;

	// Managers
	artemis::EntityManager* entityManager;
	artemis::SystemManager* systemManager;
	artemis::GroupManager* groupManager;
	artemis::TagManager* tagManager;
	
	// Mappers
	artemis::ComponentMapper<SpriteComponent> spriteMapper;
	artemis::ComponentMapper<InventoryComponent> inventoryMapper;

	FileLoader* fileLoader;
	ParticleEmitter* p;
	std::map<GameplayStates, string> stateSpriteGroups;
	std::map<GameplayStates, string> stateMenuTextGroups;
};

#endif