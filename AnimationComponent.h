#ifndef ANIMATIONCOMONENT_H_
#define ANIMATIONCOMONENT_H_

#include "Artemis\Artemis.h"
#include "Animation.h"
#include "PausableClock.h"

#include "I_Observer.h"

class AnimationComponent: public artemis::Component, public I_Observer
{
public:
	AnimationComponent(std::map<std::string, Animation*> *animationListPtr, std::map<std::string, sf::Texture*> textures) // textures should be removed once texture loader is added.
	{
		this->setAnimationList(animationListPtr);
		this->pClock.restart();
		this->onLoop = true;
		this->textures = textures;
		sprite.setTexture(*textures["idle"]);
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

	sf::Sprite getSprite() const { return sprite; }
	void setSprite(sf::Sprite& val) { sprite = val; }

	//sf::Texture getTexture() const { return texture; }
	//void setTexture(sf::Texture val) { texture = val; }
	
	bool OnLoop() const { return onLoop; }
	void OnLoop(bool val) { onLoop = val; }

	PausableClock* getClock() { return &pClock; }
	void setClock(PausableClock val) { pClock = val; }

	int getCurrentFrameIndex() const { return currentFrameIndex; }
	void setCurrentFrameIndex(int val) { currentFrameIndex = val; }

	void pauseCurrentAnimation(){ pClock.pause(); }
	void resumeCurrentAnimation(){ pClock.resume(); }

	Frame* getCurrentFramePtr() const { return currentFramePtr; }
	void setCurrentFramePtr(Frame* val) { currentFramePtr = val; }

	Animation* getCurrentAnimation() const { return currentAnimationPtr; }
	void setCurrentAnimation(Animation* val) { currentAnimationPtr = val; }

	void setCurrentAnimation(std::string animationName)
	{ 
		sprite.setTexture(*textures[animationName]);
		currentAnimationPtr = (*animationListPtr)[animationName];
		currentFramePtr = currentAnimationPtr->getFrameAt(0);
		currentFrameIndex = 0;
		this->setFrame(currentFramePtr->rect);
	}

	void AnimationComponent::setAnimationList(std::map<std::string, Animation*> *animationListPtr)
	{
		if(animationListPtr != NULL)
		{
			this->animationListPtr = animationListPtr;
			currentAnimationPtr = animationListPtr->begin()->second;
			currentFramePtr = currentAnimationPtr->getFrameAt(0);
			currentFrameIndex = 0;
			this->setFrame(currentFramePtr->rect);
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
		//updateCollisionBox();
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

private:
	sf::Sprite sprite;

	bool onLoop;
	PausableClock pClock;
	int currentFrameIndex;

	Frame* currentFramePtr;
	Animation* currentAnimationPtr;

	std::map<std::string, sf::Texture*> textures;

	std::map<std::string, Animation*>::const_iterator it;
	std::map<std::string, Animation*> *animationListPtr;
};
#endif