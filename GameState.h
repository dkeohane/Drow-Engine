#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "ScreenState.h"

#include "PositionComponent.h"
#include "SpriteComponent.h"
#include "VelocityComponent.h"
#include "PlayerInputComponent.h"
#include "AnimationComponent.h"
#include "CharacterRPGAttributes.h"
#include "MenuTextComponent.h"
#include "MovementComponent.h"

#include "MovementSystem.h"
#include "SpriteSystem.h"
#include "AnimationSystem.h"
#include "MenuComponentRenderingSystem.h"
#include "MenuComponentSystem.h"


#include "TextureManager.h"

//for file reading
#include "json\json.h"
#include <fstream>
//end file reading


class GameState : public ScreenState
{
public:
	GameState(sf::RenderWindow& window){ this->Load(window); }
	~GameState(){};

	virtual void Load(sf::RenderWindow& window);
	virtual void Draw(sf::RenderWindow& window);
	virtual void Update(sf::Event& event);

	virtual string getValue(){ return subjectValue; }
	
	void loadMap(const std::string filepath);
	void addBackgroundEntity(int num, string& textureFilepath, bool collision);

private:
	artemis::World gameWorld;
	string subjectValue;

	sf::View v;

	//Managers
	artemis::EntityManager* entityManager;
	artemis::SystemManager* systemManager;
	artemis::GroupManager* groupManager;
	artemis::TagManager* tagManager;
	

	//TEMP
	std::map<int, artemis::Entity*> backgroundMapping;
	// END TEMP

};

#endif