#ifndef I_SUBJECT_H_
#define I_SUBJECT_H_

#include "Global.h"
#include "I_Observer.h"

class I_Observer;
class I_Subject
{
public:
	virtual void attach(I_Observer*); 
	virtual void detach(I_Observer*); 
	virtual void notify();
	virtual string getValue() = 0;

private:
	vector<I_Observer*> observers;

};

#endif