#include "FontManager.h"

FontManager* FontManager::getInstance()
{
	static FontManager instance;
	return &instance;
}


void FontManager::loadResource(const std::string filepath)
{
	sf::Font *fontPtr = new sf::Font();
	if(fontPtr->loadFromFile(filepath))
	{
		resourceMap[filepath] = fontPtr;
	}
	else
	{
		delete fontPtr;
	}
}
