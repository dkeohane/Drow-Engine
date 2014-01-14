#ifndef SPRITE_SYSTEM_H
#define SPRITE_SYSTEM_H

#include "Artemis\EntityProcessingSystem.h"
#include "PositionComponent.h"
#include "SpriteComponent.h"
#include <SFML/Window.hpp>


class SpriteSystem : public artemis::EntityProcessingSystem
{
public:
	SpriteSystem(sf::RenderWindow* window)
	{
		this->window = window;
		addComponentType<SpriteComponent>();
		addComponentType<PositionComponent>();
	};

	void processEntity(artemis::Entity &e);

	virtual void initialize()
	{
		spriteMapper.init(*world);
		positionMapper.init(*world);
	};

private:
	sf::RenderWindow* window;
	artemis::ComponentMapper<SpriteComponent> spriteMapper;
	artemis::ComponentMapper<PositionComponent> positionMapper;

};

#endif // $(Guard token)