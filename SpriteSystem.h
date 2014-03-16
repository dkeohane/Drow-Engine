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
		position = (*positionMapper.get(e)).getPosition();
		if(spriteMapper.get(e)->isActive())
		{
			sprite = spriteMapper.get(e)->getSprite();
			sprite->setPosition(position);
			//if(isOnScreen())
			//{
				window->draw(*sprite);
			//}
		}
	}

	bool isOnScreen()
	{
		viewPort = window->getViewport(window->getView());
		viewPort.left = (int)(window->getView().getCenter().x - (window->getView().getSize().x / 2));
		viewPort.top = (int)(window->getView().getCenter().y - (window->getView().getSize().y / 2));

		width = int(sprite->getTextureRect().width * sprite->getScale().x);
		height = int(sprite->getTextureRect().height * sprite->getScale().y);

		entityBox.left = (int)(position.x - width / 2);
		entityBox.top = (int)(position.y - height / 2);
		entityBox.width = width;
		entityBox.height = height;

		return viewPort.intersects(entityBox);
	}

private:
	sf::Sprite* sprite;
	sf::Vector2f position;
	sf::RenderWindow* window;

	sf::IntRect entityBox;
	int width, height;
	sf::IntRect viewPort;

	artemis::ComponentMapper<SpriteComponent> spriteMapper;
	artemis::ComponentMapper<PositionComponent> positionMapper;
};

#endif