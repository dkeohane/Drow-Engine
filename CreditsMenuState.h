#ifndef CREDITSMENUSTATE_H_
#define CREDITSMENUSTATE_H_

#include "ScreenState.h"
#include "Global.h"
#include "I_Observer.h"

#include "PositionComponent.h"
#include "SpriteComponent.h"

#include "SpriteSystem.h"
#include "MenuComponentSystem.h"
#include "MenuComponentRenderingSystem.h"

#include "TextureManager.h"
#include "FontManager.h"

class CreditsMenuState: public ScreenState, public I_Observer
{
public:
	CreditsMenuState(sf::RenderWindow& window){ this->Load(window); }
	~CreditsMenuState(){}

	virtual void Load(sf::RenderWindow& window);
	virtual void Draw(sf::RenderWindow& window);
	virtual void Update(sf::Event& event);

	// Observer Update
	virtual void Update(I_Subject* theChangeSubject);
	// Subject getValue
	virtual string getValue(){ return "MainMenu"; }

private:
	artemis::World creditsWorld;
	string menuGrouping;

	//Managers
	artemis::EntityManager* entityManager;
	artemis::SystemManager* systemManager;
	artemis::GroupManager* groupManager;
};
#endif