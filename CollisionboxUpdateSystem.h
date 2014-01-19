#ifndef COLLISIONBOXUPDATESYSTEM_H_
#define COLLISIONBOXUPDATESYSTEM_H_

#include "Global.h"
#include "PositionComponent.h"
#include "CollisionComponent.h"
#include "SpriteComponent.h"

class CollisionboxUpdateSystem: public artemis::EntityProcessingSystem
{
public:
	CollisionboxUpdateSystem(sf::RenderWindow* window)
	{
		addComponentType<CollisionComponent>();
		addComponentType<PositionComponent>();
		width = height = 0;

		this->window = window;

	}
	~CollisionboxUpdateSystem(){}

	virtual void initialize() 
	{
		positionMapper.init(*world);
		collisionMapper.init(*world);
		spriteMapper.init(*world);
	}

	virtual void processEntity(artemis::Entity &e)
	{
		CollisionComponent& c = *collisionMapper.get(e);
		collisionBox = c.getCollisionBox();
		PositionComponent& p = *positionMapper.get(e);

		if(spriteMapper.get(e))
		{
			spriteMapper.get(e)->centreOnOrigin();
			sf::Sprite* sprite = spriteMapper.get(e)->getSprite();
			sf::IntRect r = spriteMapper.get(e)->getSprite()->getTextureRect();
			width = int(sprite->getTextureRect().width * sprite->getScale().x);
			height = int(sprite->getTextureRect().height * sprite->getScale().y);
		}
		else
		{
			width = collisionBox->width;
			height = collisionBox->height;
		}
		collisionBox->left = (int)(p.getPosX() - width / 2);
		collisionBox->top = (int)(p.getPosY() - height / 2);
		collisionBox->width = width;
		collisionBox->height = height;
		//debug(e);
	}

	void debug(artemis::Entity& e)
	{
		PositionComponent& p = *positionMapper.get(e);
		shape.setPosition(p.getPosX() - width / 2, p.getPosY() - height / 2);
		shape.setSize(sf::Vector2f(collisionBox->width, collisionBox->height));
		shape.setOutlineThickness(2.0f);
		shape.setOutlineColor(sf::Color::Red);
		shape.setFillColor(sf::Color::Transparent);
		window->draw(shape);
	}

private:
	float width, height;
	sf::RectangleShape shape;
	sf::RenderWindow* window;

	sf::IntRect* collisionBox;
	artemis::ComponentMapper<PositionComponent> positionMapper;
	artemis::ComponentMapper<CollisionComponent> collisionMapper;
	artemis::ComponentMapper<SpriteComponent> spriteMapper;
};

#endif