#include "CharacterAttributesSubject.h"


void CharacterAttributesSubject::attach(CharacterAttributesObserver* o)
{
	this->observers.push_back(o);
}

void CharacterAttributesSubject::detach(CharacterAttributesObserver* o)
{
	observers.erase(std::remove(observers.begin(), observers.end(), o), observers.end());
}

void CharacterAttributesSubject::notify()
{
	std::for_each(observers.begin(), observers.end(), [&](CharacterAttributesObserver *o) { o->Update(this); });
}

