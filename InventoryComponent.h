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
		unequipedItemsBag = new std::vector<EquipableItem*>();
		equipedItems = new std::map<EquipableItemSlots::ItemSlots, EquipableItem*>();
		
		for(int i = 0; i < EquipableItemSlots::_COUNT; i++)
		{
			(*equipedItems)[(EquipableItemSlots::ItemSlots)i] = NULL;
		}
	}
	~InventoryComponent(){}

	void equipItem(string itemName)
	{
		for(unequipedIterator = unequipedItemsBag->begin(); unequipedIterator != unequipedItemsBag->end(); unequipedIterator++)
		{
			if((*unequipedIterator)->getName() == itemName)
			{
				equipItem(*unequipedIterator);
				unequipedItemsBag->erase(unequipedIterator);
				break;
			}
		}
	}

	void equipItem(EquipableItem* item)
	{
		equipedIterator = equipedItems->find(item->getSlotType());
		if(equipedIterator->second != NULL) // something is already equiped
		{
			if(equipedIterator->second != item)
			{
				unequipItem(item->getSlotType()); // Unequip
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

	void unequipItem(EquipableItemSlots::ItemSlots itemSlot)
	{
		(*equipedItems)[itemSlot]->Unequip(); // Unequip the item
		unequipedItemsBag->push_back((*equipedItems)[itemSlot]); // place it in unequiped item bag
		(*equipedItems)[itemSlot] = NULL; // set item slot to null
	}

	void unequipItem(string itemName)
	{
		for(equipedIterator = equipedItems->begin(); equipedIterator != equipedItems->end(); equipedIterator++)
		{
			if(equipedIterator->second->getName() == itemName)
			{
				unequipItem(equipedIterator->first);
				break;
			}
		}
	}

	std::vector<string> getUnequipedItemNames()
	{
		std::vector<string> itemNames;
		for(unequipedIterator = unequipedItemsBag->begin(); unequipedIterator != unequipedItemsBag->end(); unequipedIterator++)
		{
			itemNames.push_back((*unequipedIterator)->getName());
		}
		return itemNames;
	}

	void useItem(string itemName)
	{
		cout << "!! NOT DOING ANYTHING !! - USE ITEM : " << itemName << endl;
	}

	void useItem(UsableItem* item)
	{

	}

	void discardItem(Item* item)
	{

	}

	void addItem(EquipableItem* item){ unequipedItemsBag->push_back(item); }
	void addItem(UsableItem* item){ items->push_back(item); }

	std::vector<EquipableItem*>* getUnequipedItems() const { return unequipedItemsBag; }
	void setUnequipedItems(std::vector<EquipableItem*>* val) { unequipedItemsBag = val; }

private:
	std::vector<Item*>* items;
	std::vector<Item*>::iterator itemIt;

	std::vector<EquipableItem*>* unequipedItemsBag;
	std::vector<EquipableItem*>::iterator unequipedIterator;

	std::map<EquipableItemSlots::ItemSlots, EquipableItem*>* equipedItems;
	std::map<EquipableItemSlots::ItemSlots, EquipableItem*>::iterator equipedIterator;
};
#endif