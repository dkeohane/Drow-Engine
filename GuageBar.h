#ifndef GUAGEBAR_H_
#define GUAGEBAR_H_

#include <SFML\Graphics.hpp>

struct GuageBar
{
	GuageBar(sf::RectangleShape& bar, sf::RectangleShape& barBackground)
		: bar(bar), barBackground(barBackground)
	{

	}

	sf::RectangleShape bar;
	sf::RectangleShape barBackground;
};

#endif