#ifndef TILE_H_
#define TILE_H_

#include <SFML\Graphics.hpp>

struct Tile
{
	Tile(int ID, sf::Texture texture, bool collision, int encounterChance, int minLevel, int maxLevel)
		: ID(ID), texture(texture), collision(collision), encounterChance(encounterChance), minLevel(minLevel), maxLevel(maxLevel)
	{

	}
	
	int ID; 
	sf::Texture texture;
	bool collision;
	int encounterChance, minLevel, maxLevel;
};

#endif