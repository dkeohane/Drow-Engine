#ifndef MENUTEXTCOMPONENT_H
#define MENUTEXTCOMPONENT_H

#include "SFML/graphics.hpp"
#include "Artemis\Component.h"

class MenuTextComponent : public artemis::Component
{
public:
	MenuTextComponent(){ componentText.setString("DEFAULT"); }
	MenuTextComponent(std::string text){ componentText.setString(text); }
	MenuTextComponent(sf::Text text){ this->componentText = text; }

	MenuTextComponent(std::string componentText, sf::Font& font, sf::Color selected, sf::Color deselected, int charSize)
	{
		this->setComponentText(componentText);
		this->setComponentFont(font);
		this->setSelectedColor(selected);
		this->setDeselectedColor(deselected);
		this->setCharacterSize(charSize);
		this->notCurrentlySelected();
	}

	~MenuTextComponent(){}

	int getCharacterSize() const { return characterSize; }
	sf::Font getComponentFont() const { return componentFont; }
	sf::Text getComponentText() const { return componentText; }
	sf::Color getSelectedColor() const { return selectedColor; }
	sf::Color getDeselectedColor() const { return deselectedColor; }

	void setCharacterSize(int val) { characterSize = val; componentText.setCharacterSize(characterSize); }
	void setComponentFont(sf::Font& val) { componentFont = val; componentText.setFont(componentFont); }
	void setComponentText(sf::Text& val) { componentText = val; }
	void setComponentText(std::string val){ componentText.setString(val); }
	void setSelectedColor(sf::Color val) { selectedColor = val; }
	void setDeselectedColor(sf::Color val) { deselectedColor = val; }
	void setCurrentColor(sf::Color val){ componentText.setColor(val); }

	float getHeight(){ return this->componentText.getLocalBounds().height; }
	float getWidth(){ return this->componentText.getLocalBounds().width; }
	void onCurrentlySelected(){ componentText.setColor(selectedColor); }
	void notCurrentlySelected(){ componentText.setColor(deselectedColor); }

private:
	int characterSize;
	sf::Font componentFont;
	sf::Text componentText;
	sf::Color selectedColor;
	sf::Color deselectedColor;

};

#endif