#include "Game.h"


Game::Game(){ }

Game::~Game(){ }

void Game::Start()
{
	this->Load();
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			
			// Update the current state with screen events
			currentState->Update(event);
		}

		// Draw logic
		window.clear();
		this->currentState->Draw(window);
		window.display();
	}
}

void Game::Load()
{
	window.setFramerateLimit(60);
	window.create(sf::VideoMode(1280, 800), "Drow Engine");
	window.setKeyRepeatEnabled(false);

	availableStates["GameState"] = createState(std::string("GameState"));
	availableStates["MainMenu"] = createState(std::string("MainMenu"));
	availableStates["CreditsMenu"] = createState(std::string("CreditsMenu"));
	availableStates["BattleState"] = createState(std::string("BattleState"));

	availableStates["GameState"]->attach(this);
	availableStates["MainMenu"]->attach(this);
	availableStates["CreditsMenu"]->attach(this);
	availableStates["BattleState"]->attach(this);

	//this->currentState = availableStates["MainMenu"];
	this->currentState = availableStates["BattleState"];
	//this->currentState = availableStates["GameState"];
}

void Game::Update(I_Subject* theChangeSubject)
{
	if (theChangeSubject->getValue() != "Quit")
	{
		// Changes the menu state
		currentStateString = theChangeSubject->getValue();
		setCurrentState(availableStates[currentStateString]);
	}else{
		//flush and close
		window.close();
	}
}

ScreenState* Game::createState(std::string& stateType)
{
	// Factory Method for creating new states
	ScreenState* returnState;
	if(stateType == "MainMenu")
	{
		returnState = new MainMenuState(this->window);
	}
	else if(stateType == "GameState")
	{
		returnState = new GameState(this->window);
	}
	else if(stateType == "CreditsMenu")
	{
		returnState =  new CreditsMenuState(this->window);
	}
	else if(stateType == "BattleState")
	{
		returnState = new BattleState(this->window);
	}
	return returnState;
}