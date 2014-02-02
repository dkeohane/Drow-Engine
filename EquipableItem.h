#ifndef EQUIPABLEITEM_H_
#define EQUIPABLEITEM_H_

#include "Item.h"
#include "EquipableItemSlots.h"

class EquipableItem: public Item
{
public:
	EquipableItem(EquipableItemSlots::ItemSlots slotType, std::map<CharacterAttributes::Attribute, double>& itemValues)
	{
		this->slotType = slotType;
		this->itemValues = itemValues;
	}
	~EquipableItem(){}

	virtual void Equip()
	{
		cout << "EQUIP" << endl; 
		notify();
	}

	virtual void Unequip()
	{
		cout << "UNEQUIP" << endl; 
		// invert the values of the item to remove them from the character
		for(valueIterator = itemValues.begin(); valueIterator != itemValues.end(); valueIterator++)
		{
			valueIterator->second *= -1; 
		}
		notify(); // notify and remove values
		// return the item values to what they were.
		for(valueIterator = itemValues.begin(); valueIterator != itemValues.end(); valueIterator++)
		{
			valueIterator->second *= -1;
		}
	}

	virtual void Discard()
	{
		//
	}
	
	EquipableItemSlots::ItemSlots getSlotType() { return slotType; }
	void setSlotType(EquipableItemSlots::ItemSlots val) { slotType = val; }

private:
	EquipableItemSlots::ItemSlots slotType;
};

#endif