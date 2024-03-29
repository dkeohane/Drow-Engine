#ifndef FONTMANAGER_H_
#define FONTMANAGER_H_

#include "GameResourceManager.h"
#include "SFML\Graphics\Font.hpp"

class FontManager : public GameResourceManager<sf::Font>
{
public:
	// Singleton
	static FontManager* getInstance();
	~FontManager(){}

	virtual void loadResource(const std::string filepath);
};

#endif

