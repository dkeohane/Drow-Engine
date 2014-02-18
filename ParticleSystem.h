#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include "Global.h"

#include "PositionComponent.h"
#include "VelocityComponent.h"
#include "DelayComponent.h"
#include "SpriteComponent.h"


class ParticleSystem: public artemis::EntityProcessingSystem
{
public:
	ParticleSystem()
	{
		addComponentType<PositionComponent>();
		addComponentType<VelocityComponent>();
		addComponentType<DelayComponent>();
		addComponentType<SpriteComponent>();
	}
	~ParticleSystem(){}

	virtual void initialize()
	{
		positionMapper.init(*world);
		velocityMapper.init(*world);
		delayMapper.init(*world);
		spriteMapper.init(*world);
	}

	virtual void processEntity(artemis::Entity &e)
	{
		positionMapper.get(e)->setPosition(positionMapper.get(e)->getPosition() + velocityMapper.get(e)->getVelocity());
		velocityMapper.get(e)->setAngle(velocityMapper.get(e)->getAngle() + velocityMapper.get(e)->getAngularVelocity());

		spriteMapper.get(e)->getSprite()->setRotation(spriteMapper.get(e)->getSprite()->getRotation() + 3);

		if(delayMapper.get(e)->getClock()->getElapsedTime() >= delayMapper.get(e)->getLifeTime())
		{
			world->getEntityManager()->remove(e);
		}
	}

private:
	artemis::ComponentMapper<PositionComponent> positionMapper;
	artemis::ComponentMapper<VelocityComponent> velocityMapper;
	artemis::ComponentMapper<DelayComponent> delayMapper;
	artemis::ComponentMapper<SpriteComponent> spriteMapper;

};
#endif