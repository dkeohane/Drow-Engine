#ifndef VIEWRELATIVEUPDATESYSTEM_H_
#define VIEWRELATIVEUPDATESYSTEM_H_

#include "Global.h"
#include "PositionComponent.h"
#include "ViewPositionComponent.h"

class ViewRelativeUpdateSystem: public artemis::EntityProcessingSystem
{
public:
	ViewRelativeUpdateSystem(sf::RenderWindow* window)
	{
		this->window = window;
		addComponentType<PositionComponent>();
		addComponentType<ViewPositionComponent>();
	}
	~ViewRelativeUpdateSystem(){}

	virtual void initialize()
	{
		positionMapper.init(*world);
		viewPositionMapper.init(*world);
	}

	virtual void processEntity(artemis::Entity &e)
	{
		positionMapper.get(e)->setPosition(window->mapPixelToCoords(sf::Vector2i(viewPositionMapper.get(e)->getPosition())));
	}

private:
	sf::RenderWindow* window;
	artemis::ComponentMapper<PositionComponent> positionMapper;
	artemis::ComponentMapper<ViewPositionComponent> viewPositionMapper;
};

#endif