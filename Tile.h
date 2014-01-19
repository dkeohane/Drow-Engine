#ifndef TILE_H_
#define TILE_H_

#include <SFML\Graphics.hpp>

struct Tile
{
	Tile(int ID, sf::Texture texture, bool collision)
		: ID(ID), texture(texture), collision(collision)
	{

	}
	
	int ID; 
	sf::Texture texture;
	bool collision;
};

#endif