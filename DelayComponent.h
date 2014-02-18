#ifndef DELAYCOMPONENT_H_
#define DELAYCOMPONENT_H_

#include "Global.h"
#include "PausableClock.h"

class DelayComponent: public artemis::Component
{	
public:
	DelayComponent(sf::Time lifeTime)
	{
		this->lifeTime = lifeTime;
		this->pClock = new PausableClock();
	}
	~DelayComponent(){}

	void setLifeTime(sf::Time &lifeTime) { this->lifeTime = lifeTime; }
	sf::Time getLifeTime(){ return lifeTime; }

	void pause() { pClock->pause(); }
	void resume() { pClock->resume(); }

	PausableClock* getClock() { return pClock; }
	void setClock(PausableClock* val) { pClock = val; }

private:
	PausableClock* pClock;
	sf::Time lifeTime;
};
#endif