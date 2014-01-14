#include "SpriteSystem.h"

void SpriteSystem::processEntity(artemis::Entity &e)
{
	PositionComponent& p = *positionMapper.get(e);
	sf::Sprite& s = spriteMapper.get(e)->getSprite();
	s.setPosition(p.getPosition());
	window->draw(s);
};