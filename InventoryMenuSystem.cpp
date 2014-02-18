#include "InventoryMenuSystem.h"


void InventoryMenuSystem::processEntity(artemis::Entity &e)
{
	if(inventoryEntity == NULL)
	{
		inventoryEntity = &e;
	}
	if(menuEntity == NULL)
	{
		menuEntity = groupManager->getEntities(menuComponentsGroup)->get(onScreenSelectedIndex);
	}
	else
	{
		if(currentState == ITEM_SELECTION)
		{
			for(int i = startDisplayIndex, j = 0; i < endDisplayIndex; i++, j++)
			{
				menuEntity = groupManager->getEntities(menuComponentsGroup)->get(j);
				(menuTextMapper.get(*menuEntity))->setComponentText(menuItemNames.at(i));
			}
		}

		if(time.getElapsedTime() >= delayTime)
		{
			//get the current values of the x and y axis
			xVal = sf::Joystick::getAxisPosition(gamePadID, sf::Joystick::X);
			yVal = sf::Joystick::getAxisPosition(gamePadID, sf::Joystick::Y);

			//if they are below the threshold, they might as well be 0
			if(std::abs(xVal) < axisThreshold) 
				xVal = 0;
			if(std::abs(yVal) < axisThreshold)
				yVal = 0;

			if(xVal != 0 || yVal != 0)
			{
				if(xVal < -axisThreshold || yVal < -axisThreshold)
				{
					menuUp();
				}
				else if(xVal > axisThreshold || yVal > axisThreshold)
				{
					menuDown();
				}
				time.restart();
			}
		}
		
		if(currentState == ITEM_SELECTION)
		{
			for(int i = startDisplayIndex, j = 0; i < endDisplayIndex; i++, j++)
			{
				menuEntity = groupManager->getEntities(menuComponentsGroup)->get(j);
				(menuTextMapper.get(*menuEntity))->setComponentText(menuItemNames.at(i));
			}
		}

	}
}

void InventoryMenuSystem::menuUp()
{
	if(groupManager->getEntities(menuComponentsGroup)->getCount() > 0 && maxOnScreen > 0)
	{
		menuEntity = groupManager->getEntities(menuComponentsGroup)->get(onScreenSelectedIndex);
		menuTextMapper.get(*menuEntity)->notCurrentlySelected();

		onScreenSelectedIndex--;
		totalSelectedIndex--;

		if(onScreenSelectedIndex < 0)
		{
			startDisplayIndex--;
			endDisplayIndex--;
			onScreenSelectedIndex = 0;
		}

		if(totalSelectedIndex < 0 || startDisplayIndex < 0)
		{
			onScreenSelectedIndex = maxOnScreen - 1;
			totalSelectedIndex = menuItemNames.size() - 1;

			startDisplayIndex = menuItemNames.size() - maxOnScreen;
			endDisplayIndex = menuItemNames.size();
		}

		menuEntity = groupManager->getEntities(menuComponentsGroup)->get(onScreenSelectedIndex);
		menuTextMapper.get(*menuEntity)->onCurrentlySelected();
	}
}

void InventoryMenuSystem::menuDown()
{
	if(groupManager->getEntities(menuComponentsGroup)->getCount() > 0 && maxOnScreen > 0)
	{
		menuEntity = groupManager->getEntities(menuComponentsGroup)->get(onScreenSelectedIndex);
		menuTextMapper.get(*menuEntity)->notCurrentlySelected();

		onScreenSelectedIndex++;
		totalSelectedIndex++;

		if(onScreenSelectedIndex >= maxOnScreen)
		{
			startDisplayIndex++;
			endDisplayIndex++;

			onScreenSelectedIndex = maxOnScreen -1;
		}

		if(totalSelectedIndex >= menuItemNames.size() || endDisplayIndex > menuItemNames.size())
		{
			onScreenSelectedIndex = 0;
			totalSelectedIndex = 0;
			startDisplayIndex = 0;
			endDisplayIndex = startDisplayIndex + maxOnScreen;
		}

		menuEntity = groupManager->getEntities(menuComponentsGroup)->get(onScreenSelectedIndex);
		menuTextMapper.get(*menuEntity)->onCurrentlySelected();
	}
}

void InventoryMenuSystem::Update(sf::Event& event)
{
	if(event.type == sf::Event::JoystickButtonReleased)
	{
		if(event.joystickButton.button == 0) // 0 is A on 360 controller
		{
			menuEntity = groupManager->getEntities(menuComponentsGroup)->get(onScreenSelectedIndex);
			if(menuEntity != NULL && inventoryEntity != NULL)
			{
				subjectValue = std::string(menuTextMapper.get(*menuEntity)->getComponentText().getString());
				cout << "subjectValue : " << subjectValue << endl;

				if(currentState == ITEM_SELECTION)
				{
					currentState = ITEM_ACTION;
					selectedItem = subjectValue;
					cout << "selectedItem : " << selectedItem << endl; 
					menuItemNames.clear();
					menuItemNames.push_back("Equip");
					menuItemNames.push_back("Use");

					// Default the variables
					maxOnScreen = 2;
					onScreenSelectedIndex = 0;
					totalSelectedIndex = 0;
					startDisplayIndex = 0;
					endDisplayIndex = startDisplayIndex + maxOnScreen;
					for(int i = 2; i < groupManager->getEntities(menuComponentsGroup)->getCount(); i++)
					{
						menuTextMapper.get(*(groupManager->getEntities(menuComponentsGroup))->get(i))->setActive(false);
					}
					for(int i = startDisplayIndex, j = 0; i < endDisplayIndex; i++, j++)
					{
						menuEntity = groupManager->getEntities(menuComponentsGroup)->get(j);
						(menuTextMapper.get(*menuEntity))->setComponentText(menuItemNames.at(i));
					}
					
				}
				else // it is item action
				{
					if(subjectValue == "Equip")
					{
						inventoryMapper.get(*inventoryEntity)->equipItem(selectedItem);
						menuItemNames = inventoryMapper.get(*inventoryEntity)->getUnequipedItemNames();
						inventoryEntity = NULL;
						currentState = ITEM_SELECTION;
					}
					else if(subjectValue == "Use")
					{
						inventoryMapper.get(*inventoryEntity)->useItem(selectedItem);
						menuItemNames = inventoryMapper.get(*inventoryEntity)->getUnequipedItemNames();
						inventoryEntity = NULL;
						currentState = ITEM_SELECTION;
					}

					maxOnScreen = (maxEntitiesOnScreen > maxOnScreen) ? menuItemNames.size() : maxOnScreen;
					onScreenSelectedIndex = 0;
					totalSelectedIndex = 0;
					startDisplayIndex = 0;
					endDisplayIndex = startDisplayIndex + maxOnScreen;
					for(int i = 0; i < groupManager->getEntities(menuComponentsGroup)->getCount(); i++)
					{
						menuTextMapper.get(*(groupManager->getEntities(menuComponentsGroup))->get(i))->setActive(true);
					}
					for(int i = startDisplayIndex, j = 0; i < endDisplayIndex; i++, j++)
					{
						menuEntity = groupManager->getEntities(menuComponentsGroup)->get(j);
						(menuTextMapper.get(*menuEntity))->setComponentText(menuItemNames.at(i));
					}
				}
			}
		}
		
		if(event.joystickButton.button == 1) // 1 is B on 360 controller
		{
			cout << "WE ARE HERE BACK WAS PRESSED " << endl; 
			subjectValue = "Back";
			menuItemNames = inventoryMapper.get(*inventoryEntity)->getUnequipedItemNames();
			inventoryEntity = NULL;
			selectedItem = "";
			currentState = ITEM_SELECTION;
			cout << "subjectValue : " << subjectValue << endl;

			maxOnScreen = (maxEntitiesOnScreen > maxOnScreen) ? menuItemNames.size() : maxOnScreen;
			onScreenSelectedIndex = 0;
			totalSelectedIndex = 0;
			startDisplayIndex = 0;
			endDisplayIndex = startDisplayIndex + maxOnScreen;
			for(int i = 0; i < groupManager->getEntities(menuComponentsGroup)->getCount(); i++)
			{
				menuTextMapper.get(*(groupManager->getEntities(menuComponentsGroup))->get(i))->setActive(true);
			}
			for(int i = startDisplayIndex, j = 0; i < endDisplayIndex; i++, j++)
			{
				menuEntity = groupManager->getEntities(menuComponentsGroup)->get(j);
				(menuTextMapper.get(*menuEntity))->setComponentText(menuItemNames.at(i));
			}

			this->notify();
		}
	}
}
