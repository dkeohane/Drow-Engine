#ifndef MAPLOADER_H_
#define MAPLOADER_H_

#include "Global.h"

//for file reading
#include "json\json.h"
#include <fstream>
//end file reading

#include "Tile.h"
#include "TextureManager.h"
#include "FontManager.h"

#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "PositionComponent.h"
#include "MenuTextComponent.h"
#include "PlayerInputComponent.h"

#include "Animation.h"

class MapLoader
{
public:
	MapLoader(artemis::World* world)
	{
		this->world = world;
		entityManager = world->getEntityManager();
		groupManager = world->getGroupManager();
		tagManager = world->getTagManager();
	}
	~MapLoader(){}

	void loadMap(const std::string filepath);
	void loadAnimations(Json::Value &root, artemis::Entity* entity);
	void loadTextComponents(const std::string filepath, bool viewRelativePositioning, std::vector<artemis::Entity*>* entityVector);
	void loadPlayerDetails(const std::string filepath);
	void loadAbilities(Json::Value &root, artemis::Entity* entity);

private:
	artemis::Entity* e;
	artemis::World* world;

	artemis::EntityManager* entityManager;
	artemis::GroupManager* groupManager;
	artemis::TagManager* tagManager;

	std::map<int, Tile*> backgroundData;
};

#endif