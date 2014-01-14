#ifndef ANIMATIONSYSTEM_H_
#define ANIMATIONSYSTEM_H_

#include "Global.h"

#include "AnimationComponent.h"
#include "PositionComponent.h"

class AnimationSystem: public artemis::EntityProcessingSystem
{

public:
	AnimationSystem(sf::RenderWindow* window)
	{
		this->window = window;
		addComponentType<AnimationComponent>();
		addComponentType<PositionComponent>();
	}

	~AnimationSystem()
	{
	
	}

	void processEntity(artemis::Entity &e);

	virtual void initialize()
	{
		animationMapper.init(*world);
		positionMapper.init(*world);
	};

	void Update(artemis::Entity &e);
	void Draw(artemis::Entity &e);


private:
	sf::RenderWindow* window;
	artemis::ComponentMapper<AnimationComponent> animationMapper;
	artemis::ComponentMapper<PositionComponent> positionMapper;
};

#endif