#include "TextureManager.h"

TextureManager* TextureManager::getInstance()
{
	static TextureManager instance;
	return &instance;
}

void TextureManager::loadResource(const std::string filepath)
{
	sf::Texture *texturePtr = new sf::Texture();
	if(texturePtr->loadFromFile(filepath))
	{
		resourceMap[filepath] = texturePtr;
	}
	else
	{
		delete texturePtr;
	}
}
