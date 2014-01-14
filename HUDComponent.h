#ifndef HUDCOMPONENT_H_
#define HUDCOMPONENT_H_

#include "GuageBar.h"
#include "Global.h"

class HUDComponent: public artemis::Component
{
public:
	HUDComponent(std::vector<sf::Texture>& barTextures, std::vector<sf::Texture>& barBackgroundTextures,
				 std::vector<sf::Vector2f>& barPositions, std::vector<sf::Vector2f>& barBackgroundPositions)
	{

		this->barTextures = barTextures;
		this->barBackgroundTextures = barBackgroundTextures;

		for(unsigned int i = 0; i < barTextures.size() && i < barBackgroundTextures.size(); i++)
		{
			sf::RectangleShape bar(sf::Vector2f((float)barTextures.at(i).getSize().x, (float)barTextures.at(i).getSize().y));
			sf::RectangleShape barBG(sf::Vector2f((float)barBackgroundTextures.at(i).getSize().x, (float)barBackgroundTextures.at(i).getSize().y));

			bar.setTexture(&this->barTextures.at(i));
			barBG.setTexture(&this->barBackgroundTextures.at(i));
			bar.setPosition(barPositions.at(i));
			barBG.setPosition(barBackgroundPositions.at(i));

			hudBars.push_back(GuageBar(bar, barBG));
		}
	}

	~HUDComponent(){}

	std::vector<GuageBar>* getHudBars(){ return &hudBars; }
	void setHudBars(std::vector<GuageBar> val) { hudBars = val; }

	void addHUDComponent(GuageBar& bar){ hudBars.push_back(bar); }
	 
private:

	std::vector<sf::Texture> barTextures;
	std::vector<sf::Texture> barBackgroundTextures;
	std::vector<GuageBar> hudBars;

};

#endif