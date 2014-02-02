#ifndef CHARACTERATTRIBUTES_H_ 
#define CHARACTERATTRIBUTES_H_

#include "Global.h"

class CharacterAttributes
{
public:
	static const enum Attribute
	{
		CURRENTHEALTH,
		MAXHEALTH,

		STRENGTH,
		AGILITY,
		INTELLIGENCE,
		_Count
	};
};

#endif