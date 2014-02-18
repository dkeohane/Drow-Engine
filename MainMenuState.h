#ifndef MAINMENUSTATE_H_
#define MAINMENUSTATE_H_

#include "ScreenState.h"
#include "I_Observer.h"
#include "Global.h"

#include "SpriteSystem.h"
#include "MenuComponentSystem.h"
#include "MenuComponentRenderingSystem.h"

#include "TextureManager.h"
#include "FileLoader.h"

class MainMenuState : public ScreenState, public I_Observer
{
public:
	MainMenuState(sf::RenderWindow& window);
	~MainMenuState();

	virtual void ProcessState();
	virtual void Update(sf::Event& event);
	virtual void Load(sf::RenderWindow& window);

	// Observer Update
	virtual void Update(I_Subject* theChangeSubject);
	// Subject getValue
	virtual string getValue(){ return subjectValue; }

private:
	artemis::World menuWorld;
	std::string subjectValue;
	FileLoader* fileLoader;

	//Managers
	artemis::EntityManager* entityManager;
	artemis::SystemManager* systemManager;
	artemis::GroupManager* groupManager;
};
#endif