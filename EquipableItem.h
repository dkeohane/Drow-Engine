#ifndef EQUIPABLEITEM_H_
#endif EQUIPABLEITEM_H_

#include "Item.h"

class EquipableItem: public Item
{
public:
	virtual void Equip(CharacterRPGAttributes* c) = 0;
	virtual void Unequip(CharacterRPGAttributes* c) = 0;
	virtual void Discard(CharacterRPGAttributes* c) = 0;

	bool isEquiped() const { return Equiped; }
	void setEquiped(bool val) { Equiped = val; }

private:
	bool Equiped;
};

#endif