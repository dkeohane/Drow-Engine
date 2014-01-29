#ifndef SPRITESYSTEM_H
#define SPRITESYSTEM_H

#include "Global.h"
#include "PositionComponent.h"
#include "SpriteComponent.h"

class SpriteSystem : public artemis::EntityProcessingSystem
{
public:
	SpriteSystem(sf::RenderWindow* window)
	{
		this->window = window;
		addComponentType<SpriteComponent>();
		addComponentType<PositionComponent>();
	};

	virtual void initialize()
	{
		spriteMapper.init(*world);
		positionMapper.init(*world);
	};

	void processEntity(artemis::Entity &e)
	{
		PositionComponent& p = *positionMapper.get(e);
		if(spriteMapper.get(e)->isActive())
		{
			s = spriteMapper.get(e)->getSprite();
			s->setPosition(p.getPosition());
			window->draw(*s);
		}
	}

private:
	sf::Sprite* s;
	sf::RenderWindow* window;

	artemis::ComponentMapper<SpriteComponent> spriteMapper;
	artemis::ComponentMapper<PositionComponent> positionMapper;
};

#endif