#ifndef CHARACTERATTRIBUTESSUBJECT_H_
#define CHARACTERATTRIBUTESSUBJECT_H_

#include "Global.h"
#include "CharacterAttributesObserver.h"
#include "CharacterAttributes.h"

class CharacterAttributesObserver;
class CharacterAttributesSubject
{
public:
	virtual void attach(CharacterAttributesObserver*); 
	virtual void detach(CharacterAttributesObserver*); 
	virtual void notify();
	virtual std::map<CharacterAttributes::Attribute, double> getValue() = 0;

private:
	vector<CharacterAttributesObserver*> observers;

};

#endif