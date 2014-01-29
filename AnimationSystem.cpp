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

	if(animationComponent.getCurrentAnimationClock()->getElapsedTime() >= animationComponent.getCurrentFramePtr()->duration)
	{
		if(animationComponent.getCurrentAnimation()->getRepeats() == true || (animationComponent.getCurrentAnimation()->getRepeats() == false && !animationComponent.onLastFrame()))
		{
			animationComponent.advanceFrame();
			animationComponent.setFrame(animationComponent.getCurrentFramePtr()->rect);
		}
		animationComponent.restartClock();
	}

}

void AnimationSystem::Draw(artemis::Entity &e)
{
	PositionComponent& p = *positionMapper.get(e);

	sf::Sprite* s = animationMapper.get(e)->getSprite();
	s->setPosition(p.getPosition());
	window->draw(*s);
}