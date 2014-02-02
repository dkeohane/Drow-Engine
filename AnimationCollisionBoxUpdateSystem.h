#ifndef ANIMATIONCOLLISIONBOXUPDATESYSTEM_H_
#define ANIMATIONCOLLISIONBOXUPDATESYSTEM_H_

#include "Global.h"
#include "PositionComponent.h"
#include "CollisionComponent.h"
#include "AnimationComponent.h"

class AnimationCollisionboxUpdateSystem: public artemis::EntityProcessingSystem
{
public:
	AnimationCollisionboxUpdateSystem(sf::RenderWindow* window)
	{
		addComponentType<AnimationComponent>();
		addComponentType<CollisionComponent>();
		addComponentType<PositionComponent>();
		width = height = 0;

		this->window = window;

	}
	~AnimationCollisionboxUpdateSystem(){}

	virtual void initialize() 
	{
		positionMapper.init(*world);
		collisionMapper.init(*world);
		animationMapper.init(*world);
	}

	virtual void processEntity(artemis::Entity &e)
	{
		collisionBox = (*collisionMapper.get(e)).getCollisionBox();
		PositionComponent& p = *positionMapper.get(e);

		sf::Sprite* sprite = animationMapper.get(e)->getSprite();
		width = int(sprite->getTextureRect().width * sprite->getScale().x);
		height = int(sprite->getTextureRect().height * sprite->getScale().y);

		collisionBox->left = (int)(p.getPosX() - width / 2);
		collisionBox->top = (int)(p.getPosY() - height / 2);
		collisionBox->width = width;
		collisionBox->height = height;
	}

	void debug(artemis::Entity& e)
	{
		PositionComponent& p = *positionMapper.get(e);
		shape.setPosition(p.getPosX() - width / 2, p.getPosY() - height / 2);
		shape.setSize(sf::Vector2f((float)collisionBox->width, (float)collisionBox->height));
		shape.setOutlineThickness(2.0f);
		shape.setOutlineColor(sf::Color::Red);
		shape.setFillColor(sf::Color::Transparent);
		window->draw(shape);
	}

private:
	int width, height;
	sf::RectangleShape shape;
	sf::RenderWindow* window;

	sf::IntRect* collisionBox;
	artemis::ComponentMapper<PositionComponent> positionMapper;
	artemis::ComponentMapper<CollisionComponent> collisionMapper;
	artemis::ComponentMapper<AnimationComponent> animationMapper;
};

#endif