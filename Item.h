#ifndef ITEM_H_
#define ITEM_H_

#include "Global.h"

class Item
{

public:
	string getName() const { return name; }
	void setName(string val) { name = val; }

	string getTextureFilePath() const { return textureFilePath; }
	void setTextureFilePath(string val) { textureFilePath = val; }

protected:
	int ID;
	string name;
	string textureFilePath;

};

#endif