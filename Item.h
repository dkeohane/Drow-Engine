#ifndef ITEM_H_
#define ITEM_H_

#include "Global.h"
#include "CharacterAttributesSubject.h"

class Item: public CharacterAttributesSubject
{

public:
	string getName() const { return name; }
	void setName(string val) { name = val; }

	string getItemDescription() const { return itemDescription; }
	void setItemDescription(string val) { itemDescription = val; }

	string getTextureFilePath() const { return textureFilePath; }
	void setTextureFilePath(string val) { textureFilePath = val; }

	virtual std::map<CharacterAttributes::Attribute, double> getValue()
	{
		return itemValues;
	}

protected:
	int ID;
	string name;
	string itemDescription;
	string textureFilePath;
	std::map<CharacterAttributes::Attribute, double> itemValues;
	std::map<CharacterAttributes::Attribute, double>::iterator valueIterator;
};

#endif