#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "SFML/graphics.hpp"
#include "Artemis\Component.h"

class SpriteComponent : public artemis::Component
{
public:
	SpriteComponent(sf::Texture& texture)
	{
		this->texture = texture;
		sprite.setTexture(this->texture);
	}

	sf::Sprite getSprite(){ return this->sprite; }
	void setSprite(sf::Sprite& sprite){ this->sprite = sprite; }

	void centreOnOrigin()
	{
		texture = *sprite.getTexture();
		sf::Vector2f origin((float)texture.getSize().x/2, (float)texture.getSize().y/2);
		sprite.setOrigin(origin);
	}

private:
	sf::Texture texture;
	sf::Sprite sprite;
};

#endif