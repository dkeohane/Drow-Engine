#include "TextureManager.h"

TextureManager* TextureManager::getInstance()
{
	static TextureManager instance;
	return &instance;
}

TextureManager::~TextureManager()
{

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

sf::Texture* TextureManager::getResource(const std::string filepath)
{
	if(!isLoaded(filepath))
	{
		loadResource(filepath);
	}
	return resourceMap[filepath];
}