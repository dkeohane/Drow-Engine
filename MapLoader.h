#ifndef MAPLOADER_H_
#define MAPLOADER_H_

#include "Global.h"

//for file reading
#include "json\json.h"
#include <fstream>
//end file reading

#include "Tile.h"
#include "TextureManager.h"

#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "PositionComponent.h"

class MapLoader
{
public:
	MapLoader(artemis::World* world)
	{
		this->world = world;
		entityManager = world->getEntityManager();
		groupManager = world->getGroupManager();
	}
	~MapLoader(){}


	void loadMap(const std::string filepath);

private:
	artemis::Entity* e;
	artemis::World* world;
	artemis::EntityManager* entityManager;
	artemis::GroupManager* groupManager;
	std::map<int, Tile*> backgroundData;
};

#endif