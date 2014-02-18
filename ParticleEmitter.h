#ifndef PARTICLE_EMITTER_H_
#define PARTICLE_EMITTER_H_

#include "Global.h"

#include "PositionComponent.h"
#include "VelocityComponent.h"
#include "DelayComponent.h"
#include "SpriteComponent.h"
#include "TextureManager.h"

class ParticleEmitter
{
public:
	ParticleEmitter(artemis::World* world)
	{
		this->world = world;
		srand((unsigned int)time(NULL));
	}
	~ParticleEmitter(){}


	void setMinLifeTime(const sf::Time &lifeTime) { this->minLifeTime = lifeTime; }
	void setMaxLifeTime(const sf::Time &lifeTime) { this->maxLifeTime = lifeTime; }

	void createParticle(unsigned int numParticles, sf::Vector2f speed, sf::Vector2f position, string textureFilePath, bool explode);

private:
	artemis::World* world;
	artemis::Entity* e;
	sf::Time minLifeTime;
	sf::Time maxLifeTime;

};
#endif