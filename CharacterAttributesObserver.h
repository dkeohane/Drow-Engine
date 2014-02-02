#ifndef CHARACTERATTRIBUTESOBSERVER_H_
#define CHARACTERATTRIBUTESOBSERVER_H_

#include "CharacterAttributesSubject.h"

class CharacterAttributesSubject;
class CharacterAttributesObserver
{
public:
	virtual void Update(CharacterAttributesSubject* theChangeSubject) = 0;
};

#endif