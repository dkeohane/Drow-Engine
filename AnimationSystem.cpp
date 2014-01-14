#include "AnimationSystem.h"
#include "Global.h"

void AnimationSystem::processEntity(artemis::Entity &e)
{
	this->Update(e);
	this->Draw(e);
}

void AnimationSystem::Update(artemis::Entity &e)
{
	AnimationComponent& animationComponent = *animationMapper.get(e);
	
	if(animationComponent.onLastFrame() || animationComponent.OnLoop())
	{
		if(animationComponent.getClock()->getElapsedTime() >= animationComponent.getCurrentFramePtr()->duration)
		{
			animationComponent.advanceFrame();
			animationComponent.setFrame(animationComponent.getCurrentFramePtr()->rect);
			animationComponent.getClock()->restart();
		}
	}
}

void AnimationSystem::Draw(artemis::Entity &e)
{
	PositionComponent& p = *positionMapper.get(e);

	sf::Sprite & s = animationMapper.get(e)->getSprite();
	s.setPosition(p.getPosition());
	window->draw(s);
}