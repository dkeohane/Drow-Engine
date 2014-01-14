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

		if(count == 0)
			c.assignViewToWindow();

		

		sf::View* v = c.getView();
		v->setCenter(p.getPosition());
		//v->setRotation(v->getRotation() + 3);
		//c.assignViewToWindow();
	}

private:
	int count;

	artemis::ComponentMapper<CameraComponent> cameraMapper;
	artemis::ComponentMapper<PositionComponent> positionMapper;

};
#endif