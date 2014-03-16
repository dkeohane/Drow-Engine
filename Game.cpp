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
		this->currentState->ProcessState();
		window.display();
	}
}

void Game::Load()
{
	window.setFramerateLimit(60);
	window.create(sf::VideoMode(1280, 800), "Drow Engine");
	window.setJoystickThreshold(30);
	window.setKeyRepeatEnabled(false);

	fadeSprite.setTexture(*TextureManager::getInstance()->getResource("Media/Images/background1.png"));
	sf::Color color = fadeSprite.getColor();
	color.a = 0;
	fadeSprite.setColor(color);

	sf::Music* music = new sf::Music();
	music->openFromFile("Media/Music/Opening.ogg");
	stateMusic["MainMenu"] = music;
	stateMusic["MainMenu"]->setLoop(true);
	
	music = new sf::Music();
	music->openFromFile("Media/Music/PalletTown.ogg");
	stateMusic["GameState"] = music;
	stateMusic["GameState"]->setLoop(true);

	music = new sf::Music();
	music->openFromFile("Media/Music/TrainerBattle.ogg");
	stateMusic["BattleState"] = music;
	stateMusic["BattleState"]->setLoop(true);


	availableStates["GameState"] = createState(std::string("GameState"));
	availableStates["BattleState"] = createState(std::string("BattleState"));
	availableStates["MainMenu"] = createState(std::string("MainMenu"));
	availableStates["CreditsMenu"] = createState(std::string("CreditsMenu"));

	availableStates["GameState"]->attach(this);
	availableStates["MainMenu"]->attach(this);
	availableStates["CreditsMenu"]->attach(this);
	availableStates["BattleState"]->attach(this);


	currentStateString = "MainMenu";

	this->currentState = availableStates[currentStateString];
	stateMusic[currentStateString]->play();
}

void Game::Update(I_Subject* theChangeSubject)
{
	if (theChangeSubject->getValue() != "Quit")
	{
		// Changes the menu state
		stateMusic[currentStateString]->pause();
		currentState->onStateDeparture();
		currentStateString = theChangeSubject->getValue();

		stateMusic[currentStateString]->play();
		fadeIn();
		setCurrentState(availableStates[currentStateString]);
		currentState->onStateEntry();
		fadeOut();
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

void Game::fadeIn()
{
	while((int)fadeSprite.getColor().a < 255)
	{
		sf::Color color = fadeSprite.getColor();
		color.a+=5;
		fadeSprite.setColor(color);
		
		window.clear();
		window.draw(fadeSprite);
		window.display();
	}
}

void Game::fadeOut()
{
	while(fadeSprite.getColor().a > 0)
	{
		sf::Color color = fadeSprite.getColor();
		color.a-=5;
		fadeSprite.setColor(color);

		window.clear();
		window.draw(fadeSprite);
		window.display();
	}
}
