#ifndef GAME_H_
#define	GAME_H_

#include "Global.h"
#include "I_Observer.h"

#include "MainMenuState.h"
#include "GameState.h"
#include "CreditsMenuState.h"
#include "BattleState.h"

class Game : public I_Observer
{
public:
	Game();
	~Game();

	void Start();
	void Load();

	virtual void Update(I_Subject* theChangeSubject);
	void setCurrentState(I_State* state){ this->currentState = state; }
	ScreenState* createState(std::string& stateType);

private:
	std::string currentStateString;
	I_State* currentState;
	std::map<std::string, ScreenState*> availableStates;
	sf::RenderWindow window;

};

#endif