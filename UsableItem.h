#ifndef USABLEITEM_H_
#define USABLEITEM_H_

#include "Item.h"
#include "CharacterRPGAttributes.h"

class UsableItem: public Item
{
public:
	virtual void Use(CharacterRPGAttributes* c) = 0;

	string getItemDescription() const { return itemDescription; }
	void setItemDescription(string val) { itemDescription = val; }

private:
	string itemDescription;
};

#endif