#ifndef ANIMATIONCOMONENT_H_
#define ANIMATIONCOMONENT_H_

#include "Artemis\Artemis.h"
#include "Animation.h"

#include "I_Observer.h"

class AnimationComponent: public artemis::Component, public I_Observer
{
public:
	AnimationComponent(std::map<std::string, Animation*> *animationListPtr, std::map<std::string, sf::Texture*> textures) // textures should be removed once texture loader is added.
	{
		currentAnimationPtr = NULL;
		this->textures = textures;
		this->setAnimationList(animationListPtr);
	}
	
	~AnimationComponent()
	{
		for(it = animationListPtr->begin(); it != animationListPtr->end(); it++)
		{
			delete it->second;
		}
		animationListPtr->clear();
		delete animationListPtr;
	}

	sf::Sprite* getSprite(){ return &sprite; }
	void setSprite(sf::Sprite& val) { sprite = val; }

	PausableClock* getCurrentAnimationClock(){ if(currentAnimationPtr != NULL) return this->currentAnimationPtr->getClock(); }

	int getCurrentFrameIndex() const { return currentFrameIndex; }
	void setCurrentFrameIndex(int val) { currentFrameIndex = val; }

	Frame* getCurrentFramePtr() const { return currentFramePtr; }
	void setCurrentFramePtr(Frame* val) { currentFramePtr = val; }

	Animation* getCurrentAnimation() const { return currentAnimationPtr; }
	void setCurrentAnimation(Animation* val) { currentAnimationPtr = val; }

	void setCurrentAnimation(std::string animationName)
	{
		if(currentAnimationPtr != NULL)
		{
			currentAnimationPtr->pauseCurrentAnimation();
		}

		sprite.setTexture(*textures[animationName]);
		it = (*animationListPtr).find(animationName);
		if(it != animationListPtr->end())
		{
			currentAnimationPtr = it->second;
		}
		currentFrameIndex = 0;
		currentFramePtr = currentAnimationPtr->getFrameAt(currentFrameIndex);
		this->setFrame(currentFramePtr->rect);

		currentAnimationPtr->resumeCurrentAnimation();
	}

	void AnimationComponent::setAnimationList(std::map<std::string, Animation*> *animationListPtr)
	{
		if(animationListPtr != NULL)
		{
			this->animationListPtr = animationListPtr;
			setCurrentAnimation((++animationListPtr->begin())->first);
		}
	}

	void AnimationComponent::advanceFrame()
	{
		if(onLastFrame()) currentFrameIndex = 0; else currentFrameIndex++;
		currentFramePtr = currentAnimationPtr->getFrameAt(currentFrameIndex);
	}

	bool AnimationComponent::onLastFrame()
	{
		return (currentAnimationPtr != NULL) ? currentFrameIndex == currentAnimationPtr->getNumFrames() - 1 : false;
	}

	void AnimationComponent::setFrame(const sf::IntRect &rect)
	{
		//set the texture rectangle in the sprite sheet
		sprite.setTextureRect(rect);
		sprite.setOrigin((float)rect.width / 2, (float)rect.height / 2);
	}

	void centreOnOrigin()
	{
		sf::Texture texture(*sprite.getTexture());
		sf::Vector2f origin((float)texture.getSize().x/2, (float)texture.getSize().y/2);
		sprite.setOrigin(origin);
	}

	virtual void Update(I_Subject* theChangeSubject)
	{
		string currentAnimationString = theChangeSubject->getValue(); // can make debugging easier
		this->setCurrentAnimation(currentAnimationString);
	}

	void pauseCurrentAnimation(){ currentAnimationPtr->pauseCurrentAnimation(); }
	void resumeCurrentAnimation(){ currentAnimationPtr->resumeCurrentAnimation(); }
	void restartClock(){ currentAnimationPtr->restartClock(); }

private:
	sf::Sprite sprite;

	int currentFrameIndex;

	Frame* currentFramePtr;
	Animation* currentAnimationPtr;

	std::map<std::string, sf::Texture*> textures;

	std::map<std::string, Animation*>::const_iterator it;
	std::map<std::string, Animation*> *animationListPtr;
};
#endif