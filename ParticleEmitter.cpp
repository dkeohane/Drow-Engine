#include "ParticleEmitter.h"

void ParticleEmitter::createParticle(unsigned int numParticles, sf::Vector2f speed, sf::Vector2f position, string textureFilePath, bool explode)
{
	for(unsigned int i = 0; i < numParticles; i++)
	{
		artemis::Entity& e = world->createEntity();
	
		float x = 1.f * (float)((float)rand()/((float)RAND_MAX/ speed.x) * 2 - 1);
		float y = - (1.f * (float)((float)rand()/((float)RAND_MAX/ speed.y) * 2 - 1));

		if(explode)
		{
			if(rand() % 2 != 0)
				x= -x;
			if(rand()% 2 != 0)
				y = -y;
		}

		float angularVelocity = 0.1f * (float)(rand()/((float)RAND_MAX/100) * 2 - 1) ;
		int lifeTime = (rand() % (maxLifeTime - minLifeTime).asMilliseconds()) + minLifeTime.asMilliseconds();

		e.addComponent(new PositionComponent(position.x, position.y));
		e.addComponent(new VelocityComponent(x, y, angularVelocity));
		e.addComponent(new DelayComponent(sf::milliseconds(lifeTime)));
		e.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource(textureFilePath)));
		//world->getGroupManager()->set("Particle", e);
		world->getEntityManager()->refresh(e);
	}
}

