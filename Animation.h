#ifndef ANIMATION_H
#define ANIMATION_H

#include "Frame.h"
#include "PausableClock.h"

class Animation
{
public:
	Animation();
	~Animation();

	/*
	*Adds one frame to the animation
	*/
	void addFrame(const sf::Time &duration, const sf::IntRect &frame);

	/*
	*Adds a row to the animation, starting from the origin (top left of the first frame)
	*/
	void addRow(sf::Vector2i &origin, const unsigned int numFrames, const sf::Vector2i &frameSize, const sf::Time &frameDuration);

	/*
	*Gets the number of frames in the animation
	*/
	const unsigned int getNumFrames() const;

	/*
	*Gets the frame at a given index
	*/
	Frame *getFrameAt(const unsigned int index) const;


	bool getRepeats() const { return repeats; }
	void setRepeats(bool val) { repeats = val; }

	Frame* KeyFrame() const { return keyFrame; }
	void setKeyFrame(Frame* val) { keyFrame = val; }
	void setKeyFrame(unsigned int index){ if(index < numFrames) keyFrame = framePtrs.at(index); }

	PausableClock* getClock() { return &pClock; }
	void setClock(PausableClock val) { pClock = val; }

	void pauseCurrentAnimation(){ pClock.pause(); }
	void resumeCurrentAnimation(){ pClock.resume(); }
	void restartClock(){ pClock.restart(); }

private:
	bool repeats;
	Frame* keyFrame;
	PausableClock pClock;

	unsigned int numFrames;
	std::vector<Frame*> framePtrs;
	std::vector<Frame*>::const_iterator it;
};

#endif //ANIMATION_H