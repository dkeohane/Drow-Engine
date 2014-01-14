#include "I_Subject.h"

void I_Subject::attach(I_Observer* o)
{
	this->observers.push_back(o);
}

void I_Subject::detach(I_Observer* o)
{
	observers.erase(std::remove(observers.begin(), observers.end(), o), observers.end());
}

void I_Subject::notify()
{
	std::for_each(observers.begin(), observers.end(), [&](I_Observer *o) { o->Update(this); });
}
