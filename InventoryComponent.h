#ifndef INVENTORYCOMPONENT_H_
#define INVENTORYCOMPONENT_H_

#include "Global.h"
#include "ActivatableComponent.h"
#include "EquipableItem.h"
#include "UsableItem.h"

class InventoryComponent: public ActivatableComponent
{
public:
	InventoryComponent()
	{
		items = new std::vector<Item*>();
		equipedItems = new std::map<EquipableItemSlots::ItemSlots, EquipableItem*>();

		for(int i = 0; i < EquipableItemSlots::_COUNT; i++)
		{
			(*equipedItems)[(EquipableItemSlots::ItemSlots)i] = NULL;
		}
	}
	~InventoryComponent(){}

	void equipItem(EquipableItem* item)
	{
		equipedIterator = equipedItems->find(item->getSlotType());
		if(equipedIterator->second != NULL) // something is already equiped
		{
			if(equipedIterator->second != item)
			{
				equipedIterator->second->Unequip(); // Unequip
				(*equipedItems)[item->getSlotType()] = item; // Assign new item to slot
				(*equipedItems)[item->getSlotType()]->Equip(); // Equip new item
			}
		}
		else // nothing is equiped yet
		{
			(*equipedItems)[item->getSlotType()] = item; // Assign new item to slot
			(*equipedItems)[item->getSlotType()]->Equip(); // Equip new item
		}	
	}

	void unequipItem(EquipableItemSlots::ItemSlots& itemSlot)
	{
		(*equipedItems)[itemSlot]->Unequip(); // Unequip the item
		items->push_back((*equipedItems)[itemSlot]); // place it in item bag
		(*equipedItems)[itemSlot] = NULL; // set item slot to null
	}

	void useItem(UsableItem* item)
	{

	}

	void discardItem(Item* item)
	{

	}

private:
	std::vector<Item*>* items;
	std::vector<Item*>::iterator itemIt;
	std::map<EquipableItemSlots::ItemSlots, EquipableItem*>* equipedItems;
	std::map<EquipableItemSlots::ItemSlots, EquipableItem*>::iterator equipedIterator;
};
#endif