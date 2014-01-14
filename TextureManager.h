#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include "Global.h"

class TextureManager
{
public:
	// Singleton
	static TextureManager* getInstance();

	~TextureManager();

	virtual void loadResource(const std::string filepath);
	sf::Texture* getResource(const std::string filepath);

	//Unloads a specific resource
	inline void unloadResource(const std::string filepath)
	{
		//can only unload a resource if it's already loaded
		if(isLoaded(filepath))
		{
			delete resourceMap[filepath];
			resourceMap.erase(filepath);
		}	
	}

	//Unloads all resources in the manager
	inline virtual void Flush()
	{
		for(it = resourceMap.begin(); it != resourceMap.end(); it++)
		{
			delete it->second;
		}
		resourceMap.clear();
	}

	//Checks to see if resource is already loaded
	inline bool isLoaded(const std::string filepath) const
	{
		//return true if we find it before reaching the end, false otherwise
		return resourceMap.find(filepath) != resourceMap.end();
	}
	

private:
	TextureManager(){}

	std::map<std::string, sf::Texture*> resourceMap;
	std::map<std::string, sf::Texture*>::const_iterator it;
};

#endif

