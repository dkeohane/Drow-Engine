#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include "GameResourceManager.h"
#include "SFML\Graphics\Texture.hpp"

class TextureManager : public GameResourceManager<sf::Texture>
{
public:
	// Singleton
	static TextureManager* getInstance();
	~TextureManager(){}

	virtual void loadResource(const std::string filepath);

private:
	TextureManager(){}
};

#endif

