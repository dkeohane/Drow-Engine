#ifndef INPUTHANDLINGSYSTEM_H_
#define INPUTHANDLINGSYSTEM_H_

#include "Global.h"
#include "Command.h"
#include "NoCommand.h"



#include "PlayerInputComponent.h"
#include "PositionComponent.h"
#include "VelocityComponent.h"

class InputHandlingSystem: public artemis::EntityProcessingSystem
{
public:
	InputHandlingSystem();
	~InputHandlingSystem();

	virtual void initialize()
	{
		addComponentType<PlayerInputComponent>();
	}


	static const enum buttonCommad
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		SELECT,
		BACK,
		_Count
	};

	void Update(sf::Event& event);
	void setUpCommand(Command* command){ commands.at(UP) = command; }
	void setDownCommand(Command* command){ commands.at(DOWN) = command; }
	void setLeftCommand(Command* command){ commands.at(LEFT) = command; }
	void setRightCommand(Command* command){ commands.at(RIGHT) = command; }
	void setSelectCommand(Command* command){ commands.at(SELECT) = command; }
	void setBackCommand(Command* command){ commands.at(BACK) = command; }
	void setAllCommand(Command* command);
	void setCommandIndexed(Command* command, int index);

	bool InputHandlingSystem::isControllerConnected(int controllerID)
	{
		return sf::Joystick::isConnected(controllerID);
	}

	void InputHandlingSystem::connectController()
	{
		/*if (event.type == sf::Event::JoystickConnected)
		{
			this->controllerID = event.joystickConnect.joystickId;
		}*/
	}

private:
	std::vector<Command*> commands;

	artemis::ComponentMapper<PlayerInputComponent> playerInputMapper;

};

#endif