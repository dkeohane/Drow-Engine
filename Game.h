#ifndef GAME_H_
#define	GAME_H_

#include "Global.h"
#include "I_Observer.h"

#include "MainMenuState.h"
#include "GameState.h"
#include "CreditsMenuState.h"
#include "BattleState.h"

#include "TextureManager.h"

class Game : public I_Observer
{
public:
	Game();
	~Game();

	void Start();
	void Load();

	virtual void Update(I_Subject* theChangeSubject);
	void setCurrentState(ScreenState* state){ this->currentState = state; }
	ScreenState* createState(std::string& stateType);

	void fadeIn();
	void fadeOut();

private:
	std::string currentStateString;
	ScreenState* currentState;

	sf::Sprite fadeSprite;

	std::map<std::string, ScreenState*> availableStates;
	std::map<std::string, sf::Music*> stateMusic;
	sf::RenderWindow window;

};

#endif