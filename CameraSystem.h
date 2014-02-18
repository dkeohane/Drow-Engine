#ifndef CAMERASYSTEM_H_
#define CAMERASYSTEM_H_

#include "CameraComponent.h"
#include "PositionComponent.h"

class CameraSystem: public artemis::EntityProcessingSystem
{
public:
	CameraSystem()
	{
		addComponentType<CameraComponent>();
		addComponentType<PositionComponent>();
	}

	~CameraSystem()
	{
	}

	virtual void initialize() 
	{
		count = 0;
		cameraMapper.init(*world);
		positionMapper.init(*world);
	}

	virtual void processEntity(artemis::Entity &e)
	{
		PositionComponent& p = *positionMapper.get(e);
		CameraComponent& c = *cameraMapper.get(e);

		c.getView()->setCenter(p.getPosition());
		c.assignViewToWindow();
	}

private:
	int count;
	sf::View* v;
	artemis::ComponentMapper<CameraComponent> cameraMapper;
	artemis::ComponentMapper<PositionComponent> positionMapper;

};
#endif