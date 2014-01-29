#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "SFML/graphics.hpp"
#include "ActivatableComponent.h"

class SpriteComponent : public ActivatableComponent
{
public:
	SpriteComponent(sf::Texture& texture)
	{
		//this->setActive(true);
		this->texture = texture;
		sprite.setTexture(this->texture);
	}

	sf::Sprite* getSprite(){ return &sprite; }
	void setSprite(sf::Sprite& sprite){ this->sprite = sprite; }

	sf::Texture* getTexture(){ return &texture; }
	void setTexture(sf::Texture& val) { texture = val; }

	void centreOnOrigin()
	{
		texture = *sprite.getTexture();
		sf::Vector2f origin((float)texture.getSize().x/2, (float)texture.getSize().y/2);
		sprite.setOrigin(origin);
	}

protected:
	sf::Texture texture;
	sf::Sprite sprite;
};

#endif