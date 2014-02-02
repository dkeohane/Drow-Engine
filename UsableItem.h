#ifndef USABLEITEM_H_
#define USABLEITEM_H_

#include "Item.h"
#include "CharacterRPGComponent.h"

class UsableItem: public Item
{
public:
	virtual void Use(CharacterRPGComponent* c) = 0;

};

#endif