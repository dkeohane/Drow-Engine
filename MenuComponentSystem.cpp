#include "MenuComponentSystem.h"

void MenuComponentSystem::processEntity(artemis::Entity &e)
{
	if(axisThreshold == -1)
		axisThreshold = playerInputMapper.get(e)->getAxisThreshold();

	if(gamePadID == -1)
		gamePadID = playerInputMapper.get(e)->getGamepadID();

	if(menuEntity == NULL)
	{
		menuEntity = groupManager->getEntities(menuComponentsGroup)->get(currentlySelected);
	}
	else
	{
		menuTextMapper.get(*menuEntity)->onCurrentlySelected();
		if(time.getElapsedTime() >= delayTime)
		{
			//get the current values of the x and y axis
			xVal = sf::Joystick::getAxisPosition(playerInputMapper.get(e)->getGamepadID(), sf::Joystick::X);
			yVal = sf::Joystick::getAxisPosition(playerInputMapper.get(e)->getGamepadID(), sf::Joystick::Y);

			//if they are below the threshold, they might as well be 0
			if(std::abs(xVal) < axisThreshold) 
				xVal = 0;
			if(std::abs(yVal) < axisThreshold)
				yVal = 0;

			if(xVal != 0 || yVal != 0)
			{
				if(xVal < -axisThreshold || yVal < -axisThreshold)
				{
					menuUpOrLeft();
				}
				else if(xVal > axisThreshold || yVal > axisThreshold)
				{
					menuDownOrRight();
				}
				time.restart();
			}
		}
	}
}

void MenuComponentSystem::menuDownOrRight()
{
	if(groupManager->getEntities(menuComponentsGroup)->getCount() > 0)
	{
		menuEntity = groupManager->getEntities(menuComponentsGroup)->get(currentlySelected);
		menuTextMapper.get(*menuEntity)->notCurrentlySelected();
		currentlySelected++;

		if (currentlySelected >= groupManager->getEntities(menuComponentsGroup)->getCount())
		{
			currentlySelected = 0;
		}

		menuEntity = groupManager->getEntities(menuComponentsGroup)->get(currentlySelected);
		menuTextMapper.get(*menuEntity)->onCurrentlySelected();
	}
}

void MenuComponentSystem::menuUpOrLeft()
{
	if(groupManager->getEntities(menuComponentsGroup)->getCount() > 0)
	{
		menuEntity = groupManager->getEntities(menuComponentsGroup)->get(currentlySelected);
		menuTextMapper.get(*menuEntity)->notCurrentlySelected();
		currentlySelected--;

		if (currentlySelected < 0)
		{
			currentlySelected = groupManager->getEntities(menuComponentsGroup)->getCount() - 1;
		}

		menuEntity = groupManager->getEntities(menuComponentsGroup)->get(currentlySelected);
		menuTextMapper.get(*menuEntity)->onCurrentlySelected();
	}
}

void MenuComponentSystem::Update(sf::Event& event)
{
	if(event.type == sf::Event::JoystickButtonReleased)
	{
		if(event.joystickButton.button == 0) // 0 is A on 360 controller
		{
			if(menuEntity != NULL)
			{
				subjectValue = std::string(menuTextMapper.get(*menuEntity)->getComponentText().getString());
				cout << "subjectValue : " << subjectValue << endl;
				this->currentlySelected = 0;
				this->notify();
			}
		}
		else if(event.joystickButton.button == 1) // 1 is B on 360 controller
		{
			subjectValue = "Back";
			cout << "subjectValue : " << subjectValue << endl;
			this->currentlySelected = 0;
			this->notify();
		}
	}
}

