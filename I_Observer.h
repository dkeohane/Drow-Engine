#ifndef I_OBSERVER_H_
#define I_OBSERVER_H_

#include "I_Subject.h"

class I_Subject;
class I_Observer
{
public:
	virtual void Update(I_Subject* theChangeSubject) = 0;
};

#endif